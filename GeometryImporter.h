#ifndef __ModuleMeshExporter_H__
#define __ModuleMeshExporter_H__

#include <vector>

struct RMesh;

class GeometryImporter
{
public:
	GeometryImporter();
	~GeometryImporter();

	RMesh* LoadMesh_TMP(const char* path, RMesh* mesh);
	RMesh* LoadMesh(const char* path, RMesh* mesh);

public:

	std::vector<RMesh*> meshes;
};

class aiMesh;

struct RMesh
{
	RMesh(aiMesh* m, const char* name, int num);

	void Reset();

	char* name;
	int mesh_id;

	unsigned int* indices;
	float* vertices;
	float* normals;

	unsigned int id_indices;
	unsigned int id_vertices;
	unsigned int id_normals;

	unsigned int numFaces;
	unsigned int numVertex;
};

#endif