//*****************************************************************************
//	File:   Mesh.cpp
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Mesh.h"

// Attribute locations, which should always stay constant with layout
static GLint posAttribLocation = 0;
static GLint colorAttribLocation = 1;
static GLint normalAttribLocation = 2;

//*************************************************************************
//  Description:
//		Constructor for a mesh class, which generates buffers for meshes
//*************************************************************************
Mesh::Mesh(std::string name) : 
	name_(name),
	posAttrib_(posAttribLocation),
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

//*************************************************************************
//  Description:
//		Adds a vertex to the mesh
// 
//	Param position
//		The position of the vertex
// 
//	Param color
//		The color of the vertex
//*************************************************************************
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

//*************************************************************************
//  Description:
//		Adds a point index to the mesh
//	Param v:
//		The index of the point being added
//*************************************************************************
void Mesh::AddPoint(unsigned int v)
{
	int vertCount = GetVertexCount();
	if (v > vertCount)
		return;

	points_.push_back(v);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[pointEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * GetPointCount(), &(points_[0]), GL_STATIC_DRAW);
}

//*************************************************************************
//  Description:
//		Adds a pair of edge indices to the mesh
//	
//	Param v1:
//		The first index of the edge being added
//	
//	Param v2:
//		The second index of the edge being added
//*************************************************************************
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

//*************************************************************************
//  Description:
//		Adds a collection of face indices to the mesh
// 
//	Param v1:
//		The first index of the face being added
// 
//	Param v2:
//		The second index of the face being added
// 
//	Param v3:
//		The third index of the face being added
//*************************************************************************
void Mesh::AddFace(unsigned int v1, unsigned int v2, unsigned int v3)
{
	faces_.push_back(Face(v1, v2, v3));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[faceEBO]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * GetFaceCount(), &(faces_[0]), GL_STATIC_DRAW);
	// glNamedBufferData(buffers_[edgeEBO], GetFaceCount() * sizeof(Face), &(faces_[0]), GL_STATIC_DRAW);
}

std::string Mesh::GetName()
{
	return name_;
}

//*************************************************************************
//  Description:
//		Gets the vertex data of the vertex at the given index
// 
//	Param v:
//		The index of the vertex to get
// 
//	Return:
//		Returns a pair representing the vertex position and color
//*************************************************************************
std::pair<glm::vec4, glm::vec3> Mesh::GetVertex(unsigned int v)
{
	if (v < GetVertexCount())
		return std::pair<glm::vec4, glm::vec3>(vertices_[v], colors_[v]);
	return std::pair<glm::vec4, glm::vec3>(glm::vec4(0), glm::vec3(0));
}

//*************************************************************************
//  Description:
//		Gets how many vertices the mesh has
// 
//	Return:
//		The vertex count of the mesh
//*************************************************************************
int Mesh::GetVertexCount()
{
	return static_cast<int>(vertices_.size());
}

//*************************************************************************
//  Description:
//		Gets how many points the mesh has (different from vertices, this is
//		for GL_POINTS rendering
// 
//	Return:
//		The point count of the mesh
//*************************************************************************
int Mesh::GetPointCount()
{
	return points_.size();
}

//*************************************************************************
//  Description:
//		Gets the vertex array of the mesh
// 
//	Return:
//		Returns a pointer to the first vertex in the array
//*************************************************************************
glm::vec4* Mesh::GetVertices()
{
	if(vertices_.size() > 0)
		return &(vertices_[0]);
	return nullptr;
}

//*************************************************************************
//  Description:
//		Gets the color array of the mesh
// 
//	Return:
//		Returns a pointer to the first color in the array
//*************************************************************************
glm::vec3* Mesh::GetColors()
{
	if(colors_.size() > 0)
		return &(colors_[0]);
	return nullptr;
}

//*************************************************************************
//  Description:
//		Gets the point array of the mesh
// 
//	Return:
//		Returns a pointer to the first point in the array
//*************************************************************************
unsigned int* Mesh::GetPoints()
{
	if(points_.size() > 0)
		return &(points_[0]);
	return nullptr;
}

//*************************************************************************
//  Description:
//		Gets the edge array of the mesh
// 
//	Return:
//		Returns a pointer to the first edge in the array
//*************************************************************************
Mesh::Edge* Mesh::GetEdges()
{
	if(edges_.size() > 0)
		return &(edges_[0]);
	return nullptr;
}

//*************************************************************************
//  Description:
//		Gets the face array of the mesh
// 
//	Return:
//		Returns a pointer to the first face in the array
//*************************************************************************
Mesh::Face* Mesh::GetFaces()
{
	if(faces_.size() > 0)
		return &(faces_[0]);
	return nullptr;
}

//*************************************************************************
//  Description:
//		Gets the edge array of the mesh
// 
//	Return:
//		Returns a pointer to the first edge in the array
//*************************************************************************
int Mesh::GetEdgeCount()
{
	return edges_.size();
}

//*************************************************************************
//  Description:
//		Gets the face count of the mesh
// 
//	Return:
//		Returns the number of faces of the mesh
//*************************************************************************
int Mesh::GetFaceCount()
{
	return faces_.size();
}

//*************************************************************************
//  Description:
//		Gets the face at the specified index
// 
//	Param i:
//		The index of the face values to get
//*************************************************************************
Mesh::Face Mesh::GetFace(unsigned int i)
{
	if (i < GetFaceCount())
		return faces_[i];
	return Mesh::Face(0, 0, 0);
}

//*************************************************************************
//  Description:
//		Gets the buffer value of the specified buffer
// 
//	Param buff:
//		The buffer to get from the mesh
//		
//	Return:
//		Returns the value of the buffer of the mesh
//*************************************************************************
GLuint Mesh::GetBuffer(Buffers buff)
{
	return buffers_[buff];
}

//*************************************************************************
//  Description:
//		Generates (if needed) and gets the VAO for points
// 
//	Return:
//		Returns the vao of the mesh for rendering points
//*************************************************************************
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

//*************************************************************************
//  Description:
//		Generates (if needed) and gets the edge vao of the mesh
// 
//	Return:
//		Returns the vao of the mesh for rendering edges
//*************************************************************************
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
	}

	return edgeVao_;
}

//*************************************************************************
//  Description:
//		Generates (if needed) and gets the face vao of the mesh
// 
//	Return:
//		Returns the vao of the mesh for rendering faces
//*************************************************************************
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
	}

	return faceVao_;
}

//*************************************************************************
//  Description:
//		Gets the position attribute location
// 
//	Return:
//		Returns the location of the position attribute
//*************************************************************************
GLint Mesh::GetPositionAttrib()
{
	return posAttrib_;
}

//*************************************************************************
//  Description:
//		Gets the color attribute location
// 
//	Return:
//		Returns the location of the color attribute
//*************************************************************************
GLint Mesh::GetColorAttrib()
{
	return colorAttrib_;
}

//*************************************************************************
//  Description:
//		Destructor for a mesh, which deletes all the buffers, as well as
//		any vaos that were generated
//*************************************************************************
Mesh::~Mesh()
{
	// Delete all the buffers
	glDeleteBuffers(1, &buffers_[faceEBO]);
	glDeleteBuffers(1, &buffers_[edgeEBO]);
	glDeleteBuffers(1, &buffers_[pointEBO]);
	glDeleteBuffers(1, &buffers_[CBO]);
	glDeleteBuffers(1, &buffers_[VBO]);

	// Delete any vertex arrays that were generated here
	if (faceVao_)
		glDeleteVertexArrays(1, &faceVao_);

	if (edgeVao_)
		glDeleteVertexArrays(1, &edgeVao_);

	if (pointVao_)
		glDeleteVertexArrays(1, &pointVao_);

	// Clear all the data structures
	faces_.clear();
	edges_.clear();
	points_.clear();
	colors_.clear();
	vertices_.clear();
}

//*************************************************************************
//  Description:
//		Constructor for a Normal Mesh, which generates a normal mesh from
//		a provided mesh
//*************************************************************************
NormalMesh::NormalMesh(Mesh* mesh) : Mesh("Norm" + mesh->GetName()), normalAttrib_(normalAttribLocation), normals_(), normalBuffer_(0), normalFaceVao_(0)
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

//*************************************************************************
//  Description:
//		Gets the normal array of the mesh
// 
//	Return:
//		Returns a pointer to the first normal in the array
//*************************************************************************
glm::vec4* NormalMesh::GetNormals()
{
	return &(normals_[0]);
}

//*************************************************************************
//  Description:
//		Generates (if needed) and gets the normal face vao for rendering
//		a mesh with normals
// 
//	Return:
//		Returns the vao for rendering a mesh with faces and normals
//*************************************************************************
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

//*************************************************************************
//  Description:
//		Destructor for the normal mesh, which only needs to handle the
//		normal buffer, since base mesh can handle the rest
//*************************************************************************
NormalMesh::~NormalMesh()
{
	glDeleteBuffers(1, &normalBuffer_);

	if (normalFaceVao_)
		glDeleteVertexArrays(1, &normalFaceVao_);
}
