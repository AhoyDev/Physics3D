#include "SFrustum.h"

#include "SSegment.h"
#include "SLine.h"
#include "SRay.h"
#include "SPlane.h"
#include "STriangle.h"
#include "SCircle.h"
#include "SPolygon.h"
#include "SSphere.h"
#include "SCapsule.h"
#include "SPolyhedron.h"

SFrustum::SFrustum(float near_plane_dist, float far_plane_dist) : SPrimitive(FRUSTUM)
{
	shape = new Frustum();
	shape->SetKind(FrustumSpaceGL, FrustumRightHanded); // defaults for gl
	shape->SetViewPlaneDistances(near_plane_dist, far_plane_dist);
	shape->SetFrame(vec(0.f), vec(0.f, 0.f, 1.f), vec(0.f, 1.f, 0.f));
}

SFrustum::SFrustum(vec pos, float near_plane_dist, float far_plane_dist) : SPrimitive(pos, FRUSTUM)
{
	shape = new Frustum();
	shape->SetKind(FrustumSpaceGL, FrustumRightHanded); // defaults for gl
	shape->SetViewPlaneDistances(near_plane_dist, far_plane_dist);
	shape->SetFrame(pos, vec(0.f, 0.f, 1.f), vec(0.f, 1.f, 0.f));
}

void SFrustum::Translate(const vec pos)
{
	transform.Translate(pos);
	shape->Translate(pos);
}

void SFrustum::TranslateForward(const float dist)
{
	Translate(shape->Front() * dist);
}

void SFrustum::SetPos(const float3 pos)
{
	transform.v[3][0] = pos.x;
	transform.v[3][1] = pos.y;
	transform.v[3][2] = pos.z;
	shape->SetPos(pos);
}

void SFrustum::SetRotation(const Quat rot)
{
	transform.SetRotatePart(rot);
	shape->SetFront(shape->Front() * rot.ToFloat3x3());
}

void SFrustum::SetAR(float ratio)
{
	/* 
	At the projection plane you have
	ytop / near = tan (fovy/2) and also
	yright / near = tan (fovx/2)

	From the definition of A.R., you have
	yright = aspect * ytop

	Algebra gives:
	tan(fovx/2) = aspect * tan(fovy/2) and then
	fovx = 2 * atan(aspect * tan(fovy/2))
	From: https://www.opengl.org/discussion_boards/showthread.php/168009-How-to-get-the-horizontal-fov */

	float h_fov = 2.f * atanf(tanf(shape->VerticalFov() * 0.5f) * ratio);
	shape->SetHorizontalFovAndAspectRatio(h_fov, ratio);
}

void SFrustum::SetPerspective(const float horizontal, const float vertical)
{
	shape->SetPerspective(horizontal, vertical);
}

void SFrustum::SetOrthographic(const float orthographicWidth, const float orthographicHeight)
{
	shape->SetOrthographic(orthographicWidth, orthographicHeight);
}

void SFrustum::ChangeSettings(const bool space_gl, const bool right_handed)
{
	shape->SetKind(
		(space_gl ? FrustumSpaceGL : FrustumSpaceD3D), // space
		(right_handed ? FrustumRightHanded : FrustumLeftHanded)); // handedness
}

void SFrustum::SetPlanesDist(const float near_plane_dist, const float far_plane_dist)
{
	if(near_plane_dist >= 0 && near_plane_dist < far_plane_dist)
		shape->SetViewPlaneDistances(near_plane_dist, far_plane_dist);
}

void SFrustum::SetNearPlaneDist(const float dist)
{
	if(dist < shape->FarPlaneDistance())
		shape->SetViewPlaneDistances(dist, shape->FarPlaneDistance());
}

void SFrustum::SetFarPlaneDist(const float dist)
{
	if (dist > shape->NearPlaneDistance())
		shape->SetViewPlaneDistances(shape->NearPlaneDistance(), dist);
}

float SFrustum::GetFOV() const
{
	return shape->HorizontalFov() / shape->VerticalFov();
}

float SFrustum::GetAspectRatio() const
{
	return shape->AspectRatio();
}

float SFrustum::GetNearPlaneDist() const
{
	return shape->NearPlaneDistance();
}

float SFrustum::GetFarPlaneDist() const
{
	return shape->FarPlaneDistance();
}

vec SFrustum::GetUp() const
{
	return shape->Up();
}

vec SFrustum::GetRight() const
{
	return Cross(shape->Up(), shape->Front());
}

vec SFrustum::GetLeft() const
{
	return GetRight() * -1.f;
}

vec SFrustum::GetForward() const
{
	return shape->Front();
}

vec SFrustum::GetBack() const
{
	return GetForward() * -1.f;
}

float* SFrustum::GetViewMatrix() const
{
	static float4x4 view = shape->ViewMatrix();
	view.Transpose();
	return (float*)view.v;
}

float* SFrustum::GetProjMatrix() const
{
	static float4x4 proj = shape->ProjectionMatrix();
	proj.Transpose();
	return (float*)proj.v;
}

bool SFrustum::IsPerspective() const
{
	return shape->Type() == PerspectiveFrustum;
}

bool SFrustum::IsOrthographic() const
{
	return shape->Type() == OrthographicFrustum;
}
void SFrustum::Render() const
{
	/*/ Get near and far from the Projection matrix.
	const double f_near = GetNearPlaneDist();
	const double f_far = GetFarPlaneDist();

	// Get the sides of the near plane.
	const double nLeft = near * (proj[2] - 1.0) / proj[0];
	const double nRight = near * (1.0 + proj[2]) / proj[0];
	const double nTop = near * (1.0 + proj[6]) / proj[5];
	const double nBottom = near * (proj[6] - 1.0) / proj[5];

	// Get the sides of the far plane.
	const double fLeft = far * (proj[2] - 1.0) / proj[0];
	const double fRight = far * (1.0 + proj[2]) / proj[0];
	const double fTop = far * (1.0 + proj[6]) / proj[5];
	const double fBottom = far * (proj[6] - 1.0) / proj[5];

	/*
	0	glVertex3f(0.0f, 0.0f, 0.0f);
	1	glVertex3f(nLeft, nBottom, -near);
	2	glVertex3f(nRight, nBottom, -near);
	3	glVertex3f(nRight, nTop, -near);
	4	glVertex3f(nLeft, nTop, -near);
	5	glVertex3f(fLeft, fBottom, -far);
	6	glVertex3f(fRight, fBottom, -far);
	7	glVertex3f(fRight, fTop, -far);
	8	glVertex3f(fLeft, fTop, -far);
	*

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	//glLoadIdentity ();

	// TODO - Update: You need to invert the mv before multiplying it with the current mv!

	glMultMatrixf(mv);

	glLineWidth(2);
	glBegin(GL_LINES);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fLeft, fBottom, -far);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fRight, fBottom, -far);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fRight, fTop, -far);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(fLeft, fTop, -far);

	//far
	glVertex3f(fLeft, fBottom, -far);
	glVertex3f(fRight, fBottom, -far);

	glVertex3f(fRight, fTop, -far);
	glVertex3f(fLeft, fTop, -far);

	glVertex3f(fRight, fTop, -far);
	glVertex3f(fRight, fBottom, -far);

	glVertex3f(fLeft, fTop, -far);
	glVertex3f(fLeft, fBottom, -far);

	//near
	glVertex3f(nLeft, nBottom, -near);
	glVertex3f(nRight, nBottom, -near);

	glVertex3f(nRight, nTop, -near);
	glVertex3f(nLeft, nTop, -near);

	glVertex3f(nLeft, nTop, -near);
	glVertex3f(nLeft, nBottom, -near);

	glVertex3f(nRight, nTop, -near);
	glVertex3f(nRight, nBottom, -near);

	glEnd();
	glLineWidth(1);
	glPopMatrix();*/
}


/*
// Clear screen
glClear(GL_COLOR_BUFFER_BIT, GL_DEPTH_BUFFER_BIT);
// Set up projection
glMatrixMode(GL_PROJECTION);
glLoadIdentity();
gluPerspective(fov, aspect, nearclip, farclip);
// Set up camera view
glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
gluLookAt(eye.x, eye.y, eye.z, target.x, target.y, target.z, 0, 1, 0);
// Draw all objects
for (each object) {
	glPushMatrix();
	glTranslatef(pos[i].x, pos[i].y, pos[i].z);
	glRotatef(axis[i].x, axis[i].y, axis[i].z, angle[i]);
	Model[i]->Draw();
	glPopMatrix();
}
// Finish
glFlush();
glSwapBuffers();

*/