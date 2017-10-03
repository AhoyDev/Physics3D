#ifndef __SFRUSTUM_H__
#define __SFRUSTUM_H__

#include "SPrimitive.h"
#include "MathGeoLib\include\MathGeoLib.h"
class SFrustum : public SPrimitive<Frustum>
{
public:
	SFrustum(const float near_plane_dist = 0.1f, const float far_plane_dist = 1000.f);
	SFrustum(const vec pos, const float near_plane_dist = 0.1f, const float far_plane_dist = 1000.f);

	void SetPos(const float3 pos);
	void SetRotation(const Quat rot);
	void SetPerspective(const float horizontal, const float vertical);
	void SetOrthographic(const float ortho_width, const float ortho_height);
	void ChangeSettings(const bool space_gl, const bool right_handed);
	void SetPlanesDist(const float near_plane_dist = 0.1f, const float far_plane_dist = 1000.f);
	void SetNearPlaneDist(const float dist = 0.1f);
	void SetFarPlaneDist(const float dist = 1000.f);

	float GetFOV() const;
	float GetAspectRatio() const;
	float GetNearPlaneDist() const;
	float GetFarPlaneDist() const;

	bool IsPerspective() const;
	bool IsOrthographic() const;

	void Render() const;

	/*bool Intersects(const SPoint* other = nullptr) const;
	bool Intersects(const SSegment* other = nullptr) const;
	bool Intersects(const SLine* other = nullptr) const;
	bool Intersects(const SRay* other = nullptr) const;
	bool Intersects(const SPlane* other = nullptr) const;
	bool Intersects(const STriangle* other = nullptr) const;
	bool Intersects(const SCircle* other = nullptr) const;
	bool Intersects(const SPolygon* other = nullptr) const;
	//bool Intersects(const SCube* other = nullptr) const;
	bool Intersects(const SFrustum* other = nullptr) const;
	bool Intersects(const SSphere* other = nullptr) const;
	bool Intersects(const SCapsule* other = nullptr) const;
	bool Intersects(const SPolyhedron* other = nullptr) const;*/
};

#endif