//*****************************************************************************
//	File:   MeshLib.cpp
//  Author: Hunter Smith
//  Date:   06/06/2022
//  Description: Mesh Library which will keep track of any meshes loaded to it
//*****************************************************************************

#include "MeshLib.h"
#include "FileReader.h"

static GLint posAttrib = 0;
static GLint colorAttrib = 1;
static GLint normalAttrib = 2;

static const glm::vec3 black(0.0f, 0.0f, 0.0f);

// Vertices of the cube
static const glm::vec4 cubeVertices[] = {
	GfxMath::Point(1.0f, 1.0f, 1.0f),
	GfxMath::Point(1.0f, 1.0f, -1.0f),
	GfxMath::Point(1.0f, -1.0f, 1.0f),
	GfxMath::Point(1.0f, -1.0f, -1.0f),
	GfxMath::Point(-1.0f, 1.0f, 1.0f),
	GfxMath::Point(-1.0f, 1.0f, -1.0f),
	GfxMath::Point(-1.0f, -1.0f, 1.0f),
	GfxMath::Point(-1.0f, -1.0f, -1.0f)
};

// Colors of vertices of cube
static const glm::vec3 cubeColors[] = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 0.0f, 1.0f),
	glm::vec3(0.0f, 1.0f, 1.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

// Edges of the cube
static const Mesh::Edge cubeEdges[] = {
	Mesh::Edge(0, 1),
	Mesh::Edge(1, 3),
	Mesh::Edge(2, 3),
	Mesh::Edge(0, 2),
	Mesh::Edge(0, 4),
	Mesh::Edge(2, 6),
	Mesh::Edge(3, 7),
	Mesh::Edge(1, 5),
	Mesh::Edge(4, 5),
	Mesh::Edge(5, 7),
	Mesh::Edge(6, 7),
	Mesh::Edge(4, 6)
};

// Faces of the cube
static const Mesh::Face cubeFaces[] = {
	Mesh::Face(0, 3, 1),
	Mesh::Face(0, 2, 3),
	Mesh::Face(0, 5, 4),
	Mesh::Face(0, 1, 5),
	Mesh::Face(3, 7, 5),
	Mesh::Face(1, 3, 5),
	Mesh::Face(2, 6, 7),
	Mesh::Face(2, 7, 3),
	Mesh::Face(0, 6, 2),
	Mesh::Face(0, 4, 6),
	Mesh::Face(4, 7, 6),
	Mesh::Face(4, 5, 7)
};

// Camera orientation mesh stuff
static const glm::vec3 red(1, 0, 0);
static const glm::vec3 green(0, 1, 0);
static const glm::vec3 blue(0, 0, 1);
static const glm::vec4 center = GfxMath::Point(0, 0, 0);
static const glm::vec4 xDir = GfxMath::Point(1, 0, 0);
static const glm::vec4 yDir = GfxMath::Point(0, 1, 0);
static const glm::vec4 zDir = GfxMath::Point(0, 0, 1);

static MeshLib meshLibrary;

// Mesh Object that handles purely VAO stuff

DckMesh::DckMesh(Mesh* mesh) : hasNormals_(false),
	buffers_(),
	pointVao_(0),
	edgeVao_(0),
	faceVao_(0),
	pointCount_(0),
	edgeCount_(0),
	faceCount_(0)
{
	// Upload Vertex and Color data
	glGenBuffers(1, &buffers_[VBO]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * mesh->GetVertexCount(), mesh->GetVertices(), GL_STATIC_DRAW);

	glGenBuffers(1, &buffers_[CBO]);
	glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * mesh->GetVertexCount(), mesh->GetColors(), GL_STATIC_DRAW);

	// Upload Face Data if there are faces
	if (mesh->GetFaceCount() > 0)
	{
		faceCount_ = mesh->GetFaceCount();
		glGenBuffers(1, &buffers_[FaceEBO]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[FaceEBO]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Face) * mesh->GetFaceCount(), mesh->GetFaces(), GL_STATIC_DRAW);
	}

	// Now decide what vao data we need to generate
	NormalMesh* normalMesh = dynamic_cast<NormalMesh*>(mesh);
	if (normalMesh)
	{
		// Mesh does have normals
		hasNormals_ = true;

		// Generate normal buffer and upload normals
		glGenBuffers(1, &buffers_[NBO]);
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[NBO]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * normalMesh->GetVertexCount(), normalMesh->GetNormals(), GL_STATIC_DRAW);

		// Create vao and upload data
		glGenVertexArrays(1, &faceVao_);

		// Bind the vertex array
		glBindVertexArray(faceVao_);

		// Bind the VBO, pass it to position attribute, and enable the attrib
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
		glVertexAttribPointer(posAttrib, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		// Bind the CBO, pass it to color attrib, and enable the attrib
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib);

		// Bind the NBO, pass it to normal attrib, and enable the attrib
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[NBO]);
		glVertexAttribPointer(normalAttrib, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(normalAttrib);

		// Bind the face ebo then unbind the vao
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[FaceEBO]);
		glBindVertexArray(0);
	}
	else
	{
		// If there are point elements, generate the buffer and create the vao
		if (mesh->GetPointCount() > 0)
		{
			// Make sure to save the point count for later
			pointCount_ = mesh->GetPointCount();

			glGenBuffers(1, &buffers_[PointEBO]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[PointEBO]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh->GetPointCount(), mesh->GetPoints(), GL_STATIC_DRAW);

			// Create point vao
			glGenVertexArrays(1, &pointVao_);
			glBindVertexArray(pointVao_);

			// Upload the Vertex Data to the respective Attribute
			glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
			glVertexAttribPointer(posAttrib, 4, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(posAttrib);

			// Upload the color data to the respective attrib
			glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
			glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(colorAttrib);

			// Bind the Point EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[PointEBO]);
			glBindVertexArray(0);
		}

		// If there are edge elements, generate the buffer and the vao
		if (mesh->GetEdgeCount() > 0)
		{
			// Make sure to get the edge count for later
			edgeCount_ = mesh->GetEdgeCount();

			glGenBuffers(1, &buffers_[EdgeEBO]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[EdgeEBO]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Mesh::Edge) * mesh->GetEdgeCount(), mesh->GetEdges(), GL_STATIC_DRAW);

			// Generate the edge vao and upload data
			glGenVertexArrays(1, &edgeVao_);
			glBindVertexArray(edgeVao_);

			// Upload vertex data to respective attrib
			glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
			glVertexAttribPointer(posAttrib, 4, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(posAttrib);

			// Upload the color data to the respective attrib
			glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
			glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, false, 0, 0);
			glEnableVertexAttribArray(colorAttrib);

			// Bind the Edge EBO
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[EdgeEBO]);
			glBindVertexArray(0);
		}

		// Create the face vao
		glGenVertexArrays(1, &faceVao_);
		glBindVertexArray(faceVao_);

		// Upload Vertex data
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[VBO]);
		glVertexAttribPointer(posAttrib, 4, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(posAttrib);

		// Upload Color data
		glBindBuffer(GL_ARRAY_BUFFER, buffers_[CBO]);
		glVertexAttribPointer(colorAttrib, 3, GL_FLOAT, false, 0, 0);
		glEnableVertexAttribArray(colorAttrib);

		// Bind the FaceEBO
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers_[FaceEBO]);
		glBindVertexArray(0);
	}
}

GLuint DckMesh::GetPointVAO()
{
	return pointVao_;
}

GLuint DckMesh::GetEdgeVAO()
{
	return edgeVao_;
}

GLuint DckMesh::GetFaceVAO()
{
	return faceVao_;
}

int DckMesh::GetPointCount()
{
	return pointCount_;
}

int DckMesh::GetEdgeCount()
{
	return edgeCount_;
}

int DckMesh::GetFaceCount()
{
	return faceCount_;
}

bool DckMesh::HasNormals()
{
	return hasNormals_;
}

DckMesh::~DckMesh()
{
	if (pointVao_)
		glDeleteVertexArrays(1, &pointVao_);
	if (edgeVao_)
		glDeleteVertexArrays(1, &edgeVao_);
	if (faceVao_)
		glDeleteVertexArrays(1, &faceVao_);
	
	for (int i = 0; i < BufferCount; ++i)
	{
		if (buffers_[i])
			glDeleteBuffers(1, &buffers_[i]);
	}
}


// Functions for interacting with the library
void MeshLibraryInit()
{
	meshLibrary.Initialize();
}

void MeshLibraryLoad(std::string meshName, Mesh* meshToLoad)
{
	meshLibrary.LoadMesh(meshName, meshToLoad);
}

DckMesh* MeshLibraryGet(std::string meshName)
{
	return meshLibrary.GetObject(meshName);
}

void MeshLibraryShutdown()
{
	meshLibrary.Shutdown();
}


// Mesh Data Library, which is managing the VAO data
void MeshLib::Initialize()
{
	// Load any base meshes we want here. Going to make this read from a file at some point
	// 
	// Create the orientation mesh
	Mesh* orientationMesh_ = new Mesh();

	orientationMesh_->AddVertex(center, red);
	orientationMesh_->AddVertex(center, green);
	orientationMesh_->AddVertex(center, blue);
	orientationMesh_->AddVertex(xDir, red);
	orientationMesh_->AddVertex(yDir, green);
	orientationMesh_->AddVertex(zDir, blue);

	orientationMesh_->AddPoint(3);
	orientationMesh_->AddPoint(4);
	orientationMesh_->AddPoint(5);

	orientationMesh_->AddEdge(0, 3);
	orientationMesh_->AddEdge(1, 4);
	orientationMesh_->AddEdge(2, 5);

	// Load the mesh to the lib and delete the old mesh (don't need it)
	LoadMesh("CamOMesh", orientationMesh_);
	delete orientationMesh_;

	// Create and add a cube mesh to the library
	Mesh* cube = ReadMeshFile("Data/Shapes/Cube.txt");

	// Create a cube for inverse normals
	Mesh* invCube = new Mesh("InvCube");

	// Create the 3D Mesh
	for (int i = 0; i < 8; ++i)
		invCube->AddVertex(cubeVertices[i], black);	
	for (int i = 0; i < 12; ++i)
		invCube->AddFace(cubeFaces[i].v1, cubeFaces[i].v3, cubeFaces[i].v2);

	// Create a normal mesh from the previous mesh
	Mesh* normCube = new NormalMesh(cube);

	// Create normal inv mesh
	Mesh* invNormCube = new NormalMesh(invCube);

	// Add the meshes to the data library
	LoadMesh(cube->GetName(), cube);
	LoadMesh(normCube->GetName(), normCube);
	LoadMesh(invNormCube->GetName(), invNormCube);

	WriteMeshFile(cube);

	delete invNormCube;
	delete normCube;
	delete invCube;
	delete cube;
}

void MeshLib::AddObject(std::string name, DckMesh* obj)
{
	auto search = meshes_.find(name);
	if (search == meshes_.end())
		meshes_.insert(std::pair<std::string, DckMesh*>(name, obj));
}

DckMesh* MeshLib::GetObject(std::string objName)
{
	auto result = meshes_.find(objName);
	if (result == meshes_.end())
		return nullptr;
	return result->second;
}

void MeshLib::Shutdown()
{
	for (auto mesh : meshes_)
	{
		if (mesh.second)
			delete mesh.second;
	}
	meshes_.clear();
}

void MeshLib::LoadMesh(std::string meshName, Mesh* meshToLoad)
{
	auto search = meshes_.find(meshName);
	if (search == meshes_.end())
	{
		DckMesh* newMesh = new DckMesh(meshToLoad);
		AddObject(meshName, newMesh);
	}
}
