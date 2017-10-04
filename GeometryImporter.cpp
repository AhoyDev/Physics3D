#include "GeometryImporter.h"

#include "Application.h"
#include "Gui_Console.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#pragma comment (lib, "Devil/libx86/DevIL.lib")
#pragma comment (lib, "Devil/libx86/ILU.lib")
#pragma comment (lib, "Devil/libx86/ILUT.lib")

#include "Devil/include/il.h"
#include "Devil/include/ilu.h"
#include "Devil/include/ilut.h"

RMesh::RMesh(aiMesh* m, const char* name, int num)
{
	Reset(); //initialize values

	this->name = new char(strlen(name) + 1);
	strcpy(this->name, name);
	char* tmp = new char(3);
	strcat(this->name, itoa(num, tmp, 10));
	strcat(this->name, "\0");
	numVertex = m->mNumVertices;

	if (m->mNumVertices > 0)
	{
		vertices = new float[(m->mNumVertices * 3)];

		memcpy(vertices, m->mVertices, sizeof(float) * m->mNumVertices * 3);

		if (m->HasFaces())
		{
			numFaces = m->mNumFaces * 3;

			indices = new uint[numFaces];

			for (uint i = 0; i < m->mNumFaces; ++i)
			{

				if (m->mFaces[i].mNumIndices != 3)
				{
					LOG("Error: Face != 3 indices");
				}
				else
				{
					memcpy(&indices[i * 3], m->mFaces[i].mIndices, 3 * sizeof(uint));
				}

			}

		}
		if (m->HasNormals())
		{
			normals = new float[m->mNumVertices * 3];
			memcpy(normals, m->mNormals, sizeof(float) * m->mNumVertices * 3);
		}
	}

	numFaces = m->mNumFaces;
}

void RMesh::Reset()
{
	name = "";
	mesh_id = 0;
	indices = nullptr;
	vertices = nullptr;
	normals = nullptr;
	id_indices = id_vertices = id_normals = numFaces = numVertex = 0;
}


GeometryImporter::GeometryImporter()
{
	struct aiLogStream stream;
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
	aiAttachLogStream(&stream);
}

GeometryImporter::~GeometryImporter()
{
	std::vector<RMesh*>::reverse_iterator rit = meshes.rbegin();
	for (; rit != meshes.rend(); ++rit)
		delete (*rit);
		
	meshes.clear();

	aiDetachAllLogStreams();
}

RMesh* GeometryImporter::LoadMesh_TMP(const char* path, RMesh* mesh)
{
	static int id_Mesh = 0;

	if (!path)
	{

		LOG("Error loading geometry");
	}

	const aiScene* scene = aiImportFile(path, 0);
	LOG("%s", aiGetErrorString());
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

RMesh* GeometryImporter::LoadMesh(const char* path, RMesh* mesh)
{
	RMesh* ret = nullptr;

	if (path != nullptr)
	{
		const aiScene* scene = aiImportFile(path, 0);

		if (scene != NULL)
		{
			static int id_Mesh = 0;

			if (scene->HasMeshes())
			{
				for (int i = 0; i < scene->mNumMeshes; i++)
				{
					ret = new RMesh(scene->mMeshes[i], "object_", i);
					meshes.push_back(ret);
				}
				
				aiReleaseImport(scene); //Unload Mesh from Assimp
			}
		}
		else
			LOG("Error importing geometry: %s", aiGetErrorString());
	}
	else
		LOG("Error loading geometry: empty path");

	return ret;
}
