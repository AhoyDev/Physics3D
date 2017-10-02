
#include "GeometryImporter.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleEditor.h"
#include "Gui_Console.h"

using namespace std;

GeometryImporter::GeometryImporter()
{
	LOG("Exporter Initializing");
}




RMesh* GeometryImporter::LoadMesh(const char* path, RMesh* mesh)
{

	static int id_Mesh = 0;

	if (!path)
	{

		LOG("Error loading geometry");
	}

	const aiScene* scene = aiImportFile(path, 0);

	RMesh* tmpMesh;

	if (scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			//Copy mesh info
			tmpMesh = new RMesh(scene->mMeshes[i], "object_", i);
			meshes.push_back(tmpMesh);
		

			//Storing mesh info
		
		}

		//UnloadMesh
		aiReleaseImport(scene);
	}
	return tmpMesh;
}

update_status GeometryImporter::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool GeometryImporter::Init()
{

	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
	
	//CONSOLE ADD LOG

	RMesh* mesh = nullptr;
	LoadMesh("Assets/Streetenvironment_V01.FBX", mesh);


	return true;
}

bool GeometryImporter::CleanUp()
{
	/*
	for (vector<riffMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++)
	{
	delete it._Ptr;
	}
	*/
	meshes.clear();

	aiDetachAllLogStreams();
	return true;
}

