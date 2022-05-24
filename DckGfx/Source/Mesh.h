#pragma once
//*****************************************************************************
//	File:   Mesh.h
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Shader.h"
#include "glm/glm.hpp"
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

	int GetVertexCount();
	int GetEdgeCount();
	int GetFaceCount();

	GLuint GetEdgeVAO();
	GLuint GetFaceVAO();

	~Mesh();

private:

	Shader* shader_;
	
	std::vector<glm::vec4> vertices_;
	std::vector<glm::vec3> colors_;

	std::vector<Edge> edges_;
	std::vector<Face> faces_;

	GLuint buffers_[BuffCount];

	GLuint edgeVao_, faceVao_;
};