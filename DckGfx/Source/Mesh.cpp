//*****************************************************************************
//	File:   Mesh.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Mesh.h"

Mesh::Mesh(Shader* shader) : shader_(shader),
	edgeIsDirty_(false),
	faceIsDirty_(false),
	vertices_(),
	colors_(),
	edges_(),
	faces_(),
	buffers_(),
	edgeVao_(0),
	faceVao_(0)
{
	glGenBuffers(1, &buffers_[VBO]);
	glGenBuffers(1, &buffers_[CBO]);
	glGenBuffers(1, &buffers_[edgeEBO]);
	glGenBuffers(1, &buffers_[faceEBO]);
}

void Mesh::AddVertex(glm::vec4 position, glm::vec3 color)
{
	// Push the position and color onto the vectors
	vertices_.push_back(position);
	colors_.push_back(color);

	// Upload vertex data to the VBO
	glNamedBufferData(buffers_[VBO], GetVertexCount() * sizeof(glm::vec4), &(vertices_[0]), GL_STATIC_DRAW);

	// Upload color data to the CBO
	glNamedBufferData(buffers_[CBO], GetVertexCount() * sizeof(glm::vec3), &(colors_[0]), GL_STATIC_DRAW);
}

void Mesh::AddEdge(unsigned int v1, unsigned int v2)
{
	edges_.push_back(Edge(v1, v2));
	glNamedBufferData(buffers_[edgeEBO], GetEdgeCount() * sizeof(Edge), &(edges_[0]), GL_STATIC_DRAW);
}

void Mesh::AddFace(unsigned int v1, unsigned int v2, unsigned int v3)
{
	faces_.push_back(Face(v1, v2, v3));
	glNamedBufferData(buffers_[faceEBO], GetFaceCount() * sizeof(Face), &(faces_[0]), GL_STATIC_DRAW);
}

int Mesh::GetVertexCount()
{
	return vertices_.size();
}

int Mesh::GetEdgeCount()
{
	return edges_.size();
}

int Mesh::GetFaceCount()
{
	return faces_.size();
}

GLuint Mesh::GetEdgeVAO()
{
	if (!edgeVao_)
	{
		glGenVertexArrays(1, &edgeVao_);

		GLint posAttrib = shader_->GetAttribLocation("position");
		GLint colorAttrib = shader_->GetAttribLocation("color");

		glEnableVertexArrayAttrib(edgeVao_, posAttrib);
		glVertexArrayAttribBinding(edgeVao_, posAttrib, 0);
		glVertexArrayAttribFormat(edgeVao_, posAttrib, 4, GL_FLOAT, GL_FALSE, 0);

		// Enable the color attribute for the vao
		glEnableVertexArrayAttrib(edgeVao_, colorAttrib);
		glVertexArrayAttribBinding(edgeVao_, colorAttrib, 1);
		glVertexArrayAttribFormat(edgeVao_, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		// Bind buffers to respective attributes 
		glVertexArrayVertexBuffer(edgeVao_, posAttrib, buffers_[VBO], 0, sizeof(glm::vec4));
		glVertexArrayVertexBuffer(edgeVao_, colorAttrib, buffers_[CBO], 0, sizeof(glm::vec3));

		// Lastly, bind ebo with vao
		glVertexArrayElementBuffer(edgeVao_, buffers_[edgeEBO]);
	}

	return edgeVao_;
}

GLuint Mesh::GetFaceVAO()
{
	// If there is no vao created yet, get one created for the mesh
	if (!faceVao_)
	{
		// Generate the VAO and all the buffers
		glGenVertexArrays(1, &faceVao_);

		// Get position and color attrib locations
		GLint posAttrib = shader_->GetAttribLocation("position");
		GLint colorAttrib = shader_->GetAttribLocation("color");

		// Enable the position attribute for the vao
		glEnableVertexArrayAttrib(faceVao_, posAttrib);
		glVertexArrayAttribBinding(faceVao_, posAttrib, 0);
		glVertexArrayAttribFormat(faceVao_, posAttrib, 4, GL_FLOAT, GL_FALSE, 0);

		// Enable the color attribute for the vao
		glEnableVertexArrayAttrib(faceVao_, colorAttrib);
		glVertexArrayAttribBinding(faceVao_, colorAttrib, 1);
		glVertexArrayAttribFormat(faceVao_, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		// Bind buffers to respective attributes 
		glVertexArrayVertexBuffer(faceVao_, posAttrib, buffers_[VBO], 0, sizeof(glm::vec4));
		glVertexArrayVertexBuffer(faceVao_, colorAttrib, buffers_[CBO], 0, sizeof(glm::vec3));

		// Lastly, bind ebo with vao
		glVertexArrayElementBuffer(faceVao_, buffers_[faceEBO]);
	}

	return faceVao_;
}

Mesh::~Mesh()
{
	// Delete all the buffers here if needed
	glDeleteBuffers(1, &buffers_[faceEBO]);
	glDeleteBuffers(1, &buffers_[edgeEBO]);
	glDeleteBuffers(1, &buffers_[CBO]);
	glDeleteBuffers(1, &buffers_[VBO]);

	if (faceVao_)
		glDeleteVertexArrays(1, &faceVao_);

	if (edgeVao_)
		glDeleteVertexArrays(1, &edgeVao_);
}