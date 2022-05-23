//*****************************************************************************
//	File:   Mesh.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Mesh.h"

Mesh::Mesh(Shader* shader) : shader_(shader), vertices_(), colors_(), faces_(), buffers_(), vao_(0)
{
}

void Mesh::AddVertex(glm::vec4 position, glm::vec3 color)
{
	vertices_.push_back(position);
	colors_.push_back(color);
}

void Mesh::AddFace(unsigned int v1, unsigned int v2, unsigned int v3)
{
	faces_.push_back(Face(v1, v2, v3));
}

int Mesh::GetVertexCount()
{
	return vertices_.size();
}

int Mesh::GetFaceCount()
{
	return faces_.size();
}

GLuint Mesh::GetVAO()
{
	// If there is no vao created yet, get one created for the mesh
	if (!vao_)
	{
		// Generate the VAO and all the buffers
		glGenVertexArrays(1, &vao_);
		glGenBuffers(1, &buffers_[VBO]);
		glGenBuffers(1, &buffers_[CBO]);
		glGenBuffers(1, &buffers_[EBO]);

		// Upload vertex data to the VBO
		glNamedBufferData(buffers_[VBO], GetVertexCount()*sizeof(glm::vec4), &(vertices_[0]), GL_STATIC_DRAW);

		// Upload color data to the CBO
		glNamedBufferData(buffers_[CBO], GetVertexCount()*sizeof(glm::vec3), &(colors_[0]), GL_STATIC_DRAW);

		// Upload face data to the EBO
		glNamedBufferData(buffers_[EBO], GetFaceCount() * sizeof(Face), &(faces_[0]), GL_STATIC_DRAW);

		// Get position and color attrib locations
		GLint posAttrib = shader_->GetAttribLocation("position");
		GLint colorAttrib = shader_->GetAttribLocation("color");

		// Enable the position attribute for the vao
		glEnableVertexArrayAttrib(vao_, posAttrib);
		glVertexArrayAttribBinding(vao_, posAttrib, 0);
		glVertexArrayAttribFormat(vao_, posAttrib, 4, GL_FLOAT, GL_FALSE, 0);

		// Enable the color attribute for the vao
		glEnableVertexArrayAttrib(vao_, colorAttrib);
		glVertexArrayAttribBinding(vao_, colorAttrib, 1);
		glVertexArrayAttribFormat(vao_, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		// Bind buffers to respective attributes 
		glVertexArrayVertexBuffer(vao_, posAttrib, buffers_[VBO], 0, sizeof(glm::vec4));
		glVertexArrayVertexBuffer(vao_, colorAttrib, buffers_[CBO], 0, sizeof(glm::vec3));

		// Lastly, bind ebo with vao
		glVertexArrayElementBuffer(vao_, buffers_[EBO]);
	}

	return vao_;
}

Mesh::~Mesh()
{
	// Delete all the buffers here
	glDeleteBuffers(1, &buffers_[EBO]);
	glDeleteBuffers(1, &buffers_[CBO]);
	glDeleteBuffers(1, &buffers_[VBO]);
	glDeleteVertexArrays(1, &vao_);
}