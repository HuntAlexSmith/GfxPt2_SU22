#pragma once
//*****************************************************************************
//	File:   Mesh.h
//  Author: Hunter Smith
//  Date:   05/22/2022
//  Description: Wrapper class for managing OpenGL meshes (mainly the vao)
//*****************************************************************************

#include "Shader.h"
#include "GfxMath.h"
#include <string>
#include <vector>

//*****************************************************************************
//  Description:
//		Enum for what types of rendering there are for a mesh
//*****************************************************************************
enum RenderType {
	Points,
	Lines,
	Triangles
};

//*****************************************************************************
//  Description:
//		Class for meshes, which manages the OpenGL buffers, as well as all the
//		vertex, point, edge, and face data needed for the mesh
//*****************************************************************************
class Mesh {
public:
	//*************************************************************************
	//  Description:
	//		Enum for what buffers are where for the array
	//*************************************************************************
	enum Buffers {
		VBO,
		CBO,
		pointEBO,
		edgeEBO,
		faceEBO,
		BuffCount
	};

	//*************************************************************************
	//  Description:
	//		Edge struct for holding Edge vertex ordering data
	//*************************************************************************
	struct Edge {
		unsigned int v1;
		unsigned int v2;
		Edge(unsigned int v1, unsigned int v2) : v1(v1), v2(v2) {}
	};

	//*************************************************************************
	//  Description:
	//		Face struct for holding Face vertex ordering data
	//*************************************************************************
	struct Face {
		unsigned int v1;
		unsigned int v2;
		unsigned int v3;
		Face(unsigned int v1, unsigned int v2, unsigned int v3) : v1(v1), v2(v2), v3(v3) {}
	};

	Mesh(std::string name = "Mesh");

	void AddVertex(glm::vec4 position, glm::vec3 color);
	void AddPoint(unsigned int v);
	void AddEdge(unsigned int v1, unsigned int v2);
	void AddFace(unsigned int v1, unsigned int v2, unsigned int v3);

	std::pair<glm::vec4, glm::vec3> GetVertex(unsigned int v);
	Face GetFace(unsigned int i);

	std::string GetName();

	int GetVertexCount();
	int GetPointCount();
	int GetEdgeCount();
	int GetFaceCount();

	glm::vec4* GetVertices();
	glm::vec3* GetColors();
	unsigned int* GetPoints();
	Mesh::Edge* GetEdges();
	Mesh::Face* GetFaces();

	GLuint GetPointVAO();
	GLuint GetEdgeVAO();
	GLuint GetFaceVAO();

	GLint GetPositionAttrib();
	GLint GetColorAttrib();

	GLuint GetBuffer(Buffers buff);

	virtual ~Mesh();

private:

	std::string name_;

	GLint posAttrib_;
	GLint colorAttrib_;
	
	std::vector<glm::vec4> vertices_;
	std::vector<glm::vec3> colors_;

	std::vector<unsigned int> points_;
	std::vector<Edge> edges_;
	std::vector<Face> faces_;

	GLuint buffers_[BuffCount];

	GLuint pointVao_, edgeVao_, faceVao_;
};

//*************************************************************************
//  Description:
//		Normal mesh class, which derives from the mesh class and handles
//		normals for a mesh as well as everything the mesh class handles
//*************************************************************************
class NormalMesh : public Mesh {
public:

	NormalMesh(Mesh* mesh);

	glm::vec4* GetNormals();

	GLuint GetNormalFaceVAO();

	~NormalMesh();

private:

	GLint normalAttrib_;
	std::vector<glm::vec4> normals_;
	GLuint normalBuffer_;
	GLuint normalFaceVao_;

};