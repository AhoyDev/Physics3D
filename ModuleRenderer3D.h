#ifndef __MODULERENDERER3D_H__
#define __MODULERENDERER3D_H__

#include "Module.h"

#include "Light.h"
#include "SDL\include\SDL_video.h"
#include "MathGeoLib\include\MathGeoLib.h"
#include "SCilinder.h"
#include "SSphere.h"
#include "GeometryImporter.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(const char* name, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
	void Save(JSONNode* config) const;
	void Load(JSONNode* config);

	void OnResize(int width, int height);
	void DrawCubeDirectMode();
	void DrawCubeGLDrawElements();
	void DrawCubeGLDrawArrays();


	bool DrawMesh(RMesh* mesh);


	bool getDepthTest();
	void setDepthTest();

	bool getCullFace();
	void setCullFace();

	bool getGLLightning();
	void setGLLightning();

	bool getGLColorMaterial();
	void setGLColorMaterial();

	bool getGLTexture2D();
	void setGLTexture2D();

	bool getWireFrame();
	void setWireFrame();


public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	float3*  vertices;
	int		num_vertices;
	uint	my_id;

	SCilinder* cilinder;
	SSphere* sphere;

	GeometryImporter* geometry_importer;
	RMesh* mesh = nullptr;


private:
	//OpenGL config window bools (temporary)
	bool isDepthTest = false;
	bool isDepthTestOnce = false;
	bool isDepthTestDOnce = false;

	bool isCullFace = false;
	bool isCullFaceOnce = false;
	bool isCullFaceDOnce = false;

	bool isGLLightning = false;
	bool isGLLightningOnce = false;
	bool isGLLightningDOnce = false;

	bool isGLColorMaterial = false;
	bool isGLColorMaterialOnce = false;
	bool isGLColorMaterialDOnce = false;

	bool isGLTexture2D = false;
	bool isGLTexture2DOnce = false;
	bool isGLTexture2DDOnce = false;

	bool isWireFramed = false;
};

#endif