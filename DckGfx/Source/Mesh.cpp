//*****************************************************************************
//	File:   Mesh.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Mesh.h"

static GLint posAttribLocation = 0;
static GLint colorAttribLocation = 1;
static GLint normalAttribLocation = 2;

Mesh::Mesh() : posAttrib_(posAttribLocation),
	colorAttrib_(colorAttribLocation),
	vertices_(),
	colors_(),
	points_(),
	edges_(),
	faces_(),
	buffers_(),
	pointVao_(0),
	edgeVao_(0),
	faceVao_(0)
{
	glGenBuffers(1, &buffers_[VBO]);
	glGenBuffers(1, &buffers_[CBO]);
	glGenBuffers(1, &buffers_[pointEBO]);
	glGenBuffers(1, &buffers_[edgeEBO]);
	glGenBuffers(1, &buffers_[faceEBO]);
}

void Mesh::AddVertex(glm::vec4 position, glm::vec3 color)
{
	// Push the position and color onto the vectors
	vertices_.push_back(position);
	colors_.push_back(color);

	// Upload vertex data to the VBO
	int vertCount = GetVertexCount();
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * vertCount, &(vertices_[0]), GL_STATIC_DRAW);
	// glNamedBufferData(buffers_[VBO], vertCount * sizeof(glm::vec4), &(vertices_[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertCount, &(colors_[0]), GL_STATIC_DRAW);
	// glNamedBufferData(buffers_[CBO], vertCount * sizeof(glm::vec3), &(colors_[0]), GL_STATIC_DRAW);
}

void Mesh::AddPoint(unsigned int v)
{
	int vertCount = GetVertexCount();
	if (v > vertCount)
		return;

	points_.push_back(v);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[pointEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * GetPointCount(), &(points_[0]), GL_STATIC_DRAW);
}

void Mesh::AddEdge(unsigned int v1, unsigned int v2)
{
	int vertCount = GetVertexCount();
	if (v1 > vertCount || v2 > vertCount)
		return;

	edges_.push_back(Edge(v1, v2));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[edgeEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Edge) * GetEdgeCount(), &(edges_[0]), GL_STATIC_DRAW);
	// glNamedBufferData(buffers_[edgeEBO], GetEdgeCount() * sizeof(Edge), &(edges_[0]), GL_STATIC_DRAW);
}

void Mesh::AddFace(unsigned int v1, unsigned int v2, unsigned int v3)
{
	faces_.push_back(Face(v1, v2, v3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[faceEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * GetFaceCount(), &(faces_[0]), GL_STATIC_DRAW);
	// glNamedBufferData(buffers_[edgeEBO], GetFaceCount() * sizeof(Face), &(faces_[0]), GL_STATIC_DRAW);
}

std::pair<glm::vec4, glm::vec3> Mesh::GetVertex(unsigned int v)
{
	if (v < GetVertexCount())
		return std::pair<glm::vec4, glm::vec3>(vertices_[v], colors_[v]);
	return std::pair<glm::vec4, glm::vec3>(glm::vec4(0), glm::vec3(0));
}

int Mesh::GetVertexCount()
{
	return static_cast<int>(vertices_.size());
}

int Mesh::GetPointCount()
{
	return points_.size();
}

glm::vec4* Mesh::GetVertices()
{
	return &(vertices_[0]);
}

glm::vec3* Mesh::GetColors()
{
	return &(colors_[0]);
}

Mesh::Face* Mesh::GetFaces()
{
	return &(faces_[0]);
}

int Mesh::GetEdgeCount()
{
	return edges_.size();
}

int Mesh::GetFaceCount()
{
	return faces_.size();
}

Mesh::Face Mesh::GetFace(unsigned int i)
{
	if (i < GetFaceCount())
		return faces_[i];
	return Mesh::Face(0, 0, 0);
}

GLuint Mesh::GetBuffer(Buffers buff)
{
	return buffers_[buff];
}

GLuint Mesh::GetPointVAO()
{
	if (!pointVao_)
	{
		glGenVertexArrays(1, &pointVao_);

		glBindVertexArray(pointVao_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
		glVertexAttribPointer(posAttrib_, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
		glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib_);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[pointEBO]);

		glBindVertexArray(0);
	}
	return pointVao_;
}

GLuint Mesh::GetEdgeVAO()
{
	if (!edgeVao_)
	{
		glGenVertexArrays(1, &edgeVao_);

		glBindVertexArray(edgeVao_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
		glVertexAttribPointer(posAttrib_, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
		glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib_);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[edgeEBO]);

		glBindVertexArray(0);
		
		/*
		glEnableVertexArrayAttrib(edgeVao_, posAttrib);
		glVertexArrayAttribBinding(edgeVao_, posAttrib, 0);
		glVertexArrayAttribFormat(edgeVao_, posAttrib, 4, GL_FLOAT, GL_FALSE, 0);

		// Enable the color attribute for the vao
		glEnableVertexArrayAttrib(edgeVao_, colorAttrib);
		glVertexArrayAttribBinding(edgeVao_, colorAttrib, 0);
		glVertexArrayAttribFormat(edgeVao_, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		// Bind buffers to respective attributes 
		glVertexArrayVertexBuffer(edgeVao_, posAttrib, buffers_[VBO], 0, sizeof(glm::vec4));
		glVertexArrayVertexBuffer(edgeVao_, colorAttrib, buffers_[CBO], 0, sizeof(glm::vec3));

		// Lastly, bind ebo with vao
		glVertexArrayElementBuffer(edgeVao_, buffers_[edgeEBO]);
		*/
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

		glBindVertexArray(faceVao_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
		glVertexAttribPointer(posAttrib_, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib_);

		glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
		glVertexAttribPointer(colorAttrib_, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib_);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[faceEBO]);

		glBindVertexArray(0);

		/*
		// Enable the position attribute for the vao
		glEnableVertexArrayAttrib(faceVao_, posAttrib);
		glVertexArrayAttribBinding(faceVao_, posAttrib, 0);
		glVertexArrayAttribFormat(faceVao_, posAttrib, 4, GL_FLOAT, GL_FALSE, 0);

		// Enable the color attribute for the vao
		glEnableVertexArrayAttrib(faceVao_, colorAttrib);
		glVertexArrayAttribBinding(faceVao_, colorAttrib, 0);
		glVertexArrayAttribFormat(faceVao_, colorAttrib, 3, GL_FLOAT, GL_FALSE, 0);

		// Bind buffers to respective attributes 
		glVertexArrayVertexBuffer(faceVao_, posAttrib, buffers_[VBO], 0, sizeof(glm::vec4));
		glVertexArrayVertexBuffer(faceVao_, colorAttrib, buffers_[CBO], 0, sizeof(glm::vec3));

		// Lastly, bind ebo with vao
		glVertexArrayElementBuffer(faceVao_, buffers_[faceEBO]);
		*/
	}

	return faceVao_;
}

GLint Mesh::GetPositionAttrib()
{
	return posAttrib_;
}

GLint Mesh::GetColorAttrib()
{
	return colorAttrib_;
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

	faces_.clear();
	edges_.clear();
	colors_.clear();
	vertices_.clear();
}

NormalMesh::NormalMesh(Mesh* mesh) : Mesh(), normalAttrib_(normalAttribLocation), normals_(), normalBuffer_(0), normalFaceVao_(0)
{
	glGenBuffers(1, &normalBuffer_);
	int faceCount = mesh->GetFaceCount();
	for (int i = 0; i < faceCount; ++i)
	{
		// Add the new face
		AddFace(i * 3, i * 3 + 1, i * 3 + 2);

		// Get the vertices from the original mesh
		Face origMeshFace = mesh->GetFace(i);

		std::pair<glm::vec4, glm::vec3> v1 = mesh->GetVertex(origMeshFace.v1);
		std::pair<glm::vec4, glm::vec3> v2 = mesh->GetVertex(origMeshFace.v2);
		std::pair<glm::vec4, glm::vec3> v3 = mesh->GetVertex(origMeshFace.v3);

		AddVertex(v1.first, v1.second);
		AddVertex(v2.first, v2.second);
		AddVertex(v3.first, v3.second);

		// Calculate normals
		glm::vec4 firstVec = v2.first - v1.first;
		glm::vec4 secondVec = v3.first - v1.first;
		glm::vec4 normal = GfxMath::CrossProduct(firstVec, secondVec);
		normals_.push_back(normal);
		normals_.push_back(normal);
		normals_.push_back(normal);
	}
}

GLuint NormalMesh::GetNormalFaceVAO()
{
	if (!normalFaceVao_)
	{
		// Upload vertex, color, normal, and face data
		glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(VBO));
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * GetVertexCount(), GetVertices(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(CBO));
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * GetVertexCount(), GetColors(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer_);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * normals_.size(), &(normals_[0]), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetBuffer(faceEBO));
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * GetFaceCount(), GetFaces(), GL_STATIC_DRAW);

		// Generate the vertex array and bind it
		glGenVertexArrays(1, &normalFaceVao_);

		glBindVertexArray(normalFaceVao_);

		// Get position and color attrib from base
		GLint posAttrib = GetPositionAttrib();
		GLint colorAttrib = GetColorAttrib();

		// Enable attributes
		glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(VBO));
		glVertexAttribPointer(posAttrib, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, GetBuffer(CBO));
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib);

		glBindBuffer(GL_ARRAY_BUFFER, normalBuffer_);
		glVertexAttribPointer(normalAttrib_, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(normalAttrib_);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetBuffer(faceEBO));
		glBindVertexArray(0);
	}
	return normalFaceVao_;
}

NormalMesh::~NormalMesh()
{
	glDeleteBuffers(1, &normalBuffer_);

	if (normalFaceVao_)
		glDeleteVertexArrays(1, &normalFaceVao_);
}
