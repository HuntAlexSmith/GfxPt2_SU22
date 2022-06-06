#pragma once
//*****************************************************************************
//	File:   Mesh.h
//  Author: Hunter Smith
//  Date:   06/06/2022
//  Description: Mesh Library which will keep track of any meshes loaded to it
//*****************************************************************************

#include "Library.h"
#include "Mesh.h"
#include "glad/glad.h"

class DckMesh {
public:

	enum MeshBuffs {
		VBO,
		CBO,
		NBO,
		PointEBO,
		EdgeEBO,
		FaceEBO,
		BufferCount
	};

	DckMesh(Mesh* mesh);

	GLuint GetPointVAO();
	GLuint GetEdgeVAO();
	GLuint GetFaceVAO();

	int GetPointCount();
	int GetEdgeCount();
	int GetFaceCount();

	bool HasNormals();

	~DckMesh();

private:

	bool hasNormals_;

	GLuint buffers_[BufferCount];

	GLuint pointVao_;
	GLuint edgeVao_;
	GLuint faceVao_;

	int pointCount_;
	int edgeCount_;
	int faceCount_;

};

void MeshLibraryInit();
void MeshLibraryLoad(std::string meshName, Mesh* meshToLoad);
DckMesh* MeshLibraryGet(std::string meshName);
void MeshLibraryShutdown();

class MeshLib : public Library<DckMesh*> {
public:

	void Initialize() override;
	void AddObject(std::string name, DckMesh* obj) override;
	DckMesh* GetObject(std::string objName) override;
	void Shutdown() override;

	void LoadMesh(std::string meshName, Mesh* meshToLoad);

private:

	std::map<std::string, DckMesh*> meshes_;

};