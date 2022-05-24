#pragma once
//*****************************************************************************
//	File:   Mesh.h
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Shader.h"
#include "GfxMath.h"
#include <vector>

class Mesh {
public:
	enum Buffers {
		VBO,
		CBO,
		edgeEBO,
		faceEBO,
		BuffCount
	};

	struct Edge {
		unsigned int v1;
		unsigned int v2;
		Edge(unsigned int v1, unsigned int v2) : v1(v1), v2(v2) {}
	};

	struct Face {
		unsigned int v1;
		unsigned int v2;
		unsigned int v3;
		Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) {}
	};

	Mesh(Shader* shader);

	void AddVertex(glm::vec4 position, glm::vec3 color);
	void AddEdge(unsigned int v1, unsigned int v2);
	void AddFace(unsigned int v1, unsigned int v2, unsigned int v3);

	std::pair<glm::vec4, glm::vec3> GetVertex(unsigned int v);
	Face GetFace(unsigned int i);

	int GetVertexCount();
	int GetEdgeCount();
	int GetFaceCount();

	glm::vec4* GetVertices();
	glm::vec3* GetColors();
	Mesh::Face* GetFaces();

	GLuint GetEdgeVAO();
	GLuint GetFaceVAO();

	GLuint GetBuffer(Buffers buff);

	Shader* GetShader();

	virtual ~Mesh();

private:

	Shader* shader_;
	
	std::vector<glm::vec4> vertices_;
	std::vector<glm::vec3> colors_;

	std::vector<Edge> edges_;
	std::vector<Face> faces_;

	GLuint buffers_[BuffCount];

	GLuint edgeVao_, faceVao_;
};

class NormalMesh : public Mesh {
public:

	NormalMesh(Mesh* mesh);

	GLuint GetNormalFaceVAO();

	~NormalMesh();

private:

	std::vector<glm::vec4> normals_;
	GLuint normalBuffer_;
	GLuint normalFaceVao_;

};