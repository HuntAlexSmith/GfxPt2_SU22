//*****************************************************************************
//	File:   FileReader.cpp
//  Author: Hunter Smith
//  Date:   05/20/2022
//  Description: Functions for reading the contents of a file and outputting
//		what was in the files
//*****************************************************************************

#include <fstream>
#include <sstream>
#include <iostream>
#include "FileReader.h"

static const std::string shapesPath = "Data/Shapes/";
static const glm::vec3 black(0, 0, 0);

//*****************************************************************************
//	Description:
//		This functions reads from a file that should be a shader and
//		outputs the code read from the file.
//	
//  Param filepath
//		The filepath of the file we want to read form
// 
//  Return
//		Returns a std::string that contains the contents of the read file
//*****************************************************************************
std::string ReadShaderFile(const char* filepath)
{
	// Make sure we have the file object and enable exceptions
	std::ifstream shaderFile;
	std::stringstream shaderContents;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// Open the given file
		shaderFile.open(filepath);

		// Have a string stream and dump everything from the file to it
		shaderContents << shaderFile.rdbuf();

		// Remember to close the file
		shaderFile.close();
	}
	catch (std::ifstream::failure error) {
		std::cout << "Failed to read from file: " << filepath << std::endl;
		return std::string();
	}
	return shaderContents.str();
}

void WriteMeshFile(Mesh* mesh)
{
	// If there is no mesh to write, just return
	if (!mesh)
		return;

	// Figure out the filepath of the mesh
	std::string fullFilepath = shapesPath + mesh->GetName() + ".txt";

	// Create the stream object
	std::ofstream meshFile;
	meshFile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	try {
		// Open the file the mesh will be at
		meshFile.open(fullFilepath);
		if (meshFile.is_open())
		{
			// Write that this file is of a mesh
			meshFile << "Mesh\n";

			// Write the name of the mesh into the file
			meshFile << "Name\n";
			meshFile << mesh->GetName() << "\n";

			// Now to write everything else about the mesh into the file, starting with vertex data
			glm::vec4* vertices = mesh->GetVertices();
			glm::vec3* colors = mesh->GetColors();
			int vertexCount = mesh->GetVertexCount();
			meshFile << "Vertices\n";
			meshFile << vertexCount << "\n";
			for (int i = 0; i < vertexCount; ++i)
			{
				glm::vec4 currVertex = vertices[i];
				glm::vec3 currColor = colors[i];
				meshFile << currVertex.x << " " << currVertex.y << " " << currVertex.z << "\n";
				meshFile << currColor.r << " " << currColor.g << " " << currColor.b << "\n";
			}

			// Now write point data into the file
			unsigned int* points = mesh->GetPoints();
			int pointCount = mesh->GetPointCount();
			meshFile << "Points\n";
			meshFile << pointCount << "\n";
			for (int i = 0; i < pointCount; ++i)
			{
				meshFile << points[i] << "\n";
			}

			// Now write edge data into the file
			Mesh::Edge* edges = mesh->GetEdges();
			int edgeCount = mesh->GetEdgeCount();
			meshFile << "Edges\n";
			meshFile << edgeCount << "\n";
			for (int i = 0; i < edgeCount; ++i)
			{
				Mesh::Edge currEdge = edges[i];
				meshFile << currEdge.v1 << " " << currEdge.v2 << "\n";
			}

			// Now write face data into the file
			Mesh::Face* faces = mesh->GetFaces();
			int faceCount = mesh->GetFaceCount();
			meshFile << "Faces\n";
			meshFile << faceCount << "\n";
			for (int i = 0; i < faceCount; ++i)
			{
				Mesh::Face currFace = faces[i];
				meshFile << currFace.v1 << " " << currFace.v2 << " " << currFace.v3 << "\n";
			}
		
			// Close the file
			meshFile.close();
		}
	}
	catch (std::ofstream::failure error) {
		std::cout << "Failed to open or create file: " << fullFilepath << std::endl;
		return;
	}
	return;
}

Mesh* ReadMeshFile(const char* filepath)
{
	Mesh* readMesh = nullptr;
	std::ifstream meshFile;
	meshFile.exceptions(std::ifstream::badbit);
	try {
		meshFile.open(filepath);
		if (meshFile.is_open())
		{
			// Start reading in data from the file
			std::string currLine;
			std::getline(meshFile, currLine);
			if (currLine == "Mesh")
			{
				// Read in each line
				while (std::getline(meshFile, currLine))
				{
					// If current line is name, read next line as name for mesh
					if (currLine == "Name")
					{
						std::getline(meshFile, currLine);
						readMesh = new Mesh(currLine);
						continue;
					}

					// Start reading in vertex of the mesh
					if (currLine == "Vertices")
					{
						std::getline(meshFile, currLine);
						std::istringstream vertCountText(currLine);
						int vertexCount;
						vertCountText >> vertexCount;
						for (int i = 0; i < vertexCount; ++i)
						{
							std::getline(meshFile, currLine);
							std::istringstream vertData(currLine);
							std::getline(meshFile, currLine);
							std::istringstream colorData(currLine);
							float x, y, z;
							float r, g, b;
							vertData >> x >> y >> z;
							colorData >> r >> g >> b;
							if (readMesh)
								readMesh->AddVertex(GfxMath::Point(x, y, z),glm::vec3(r, g, b));
						}
						continue;
					}

					// Start reading in point data of the mesh
					if (currLine == "Points")
					{
						std::getline(meshFile, currLine);
						std::istringstream pointCountText(currLine);
						int pointCount;
						pointCountText >> pointCount;
						for (int i = 0; i < pointCount; ++i)
						{
							std::getline(meshFile, currLine);
							std::istringstream pointData(currLine);
							unsigned int v;
							pointData >> v;
							if (readMesh)
								readMesh->AddPoint(v);
						}
						continue;
					}

					// Start reading in edge data of the mesh
					if (currLine == "Edges")
					{
						std::getline(meshFile, currLine);
						std::istringstream edgeCountText(currLine);
						int edgeCount;
						edgeCountText >> edgeCount;
						for (int i = 0; i < edgeCount; ++i)
						{
							std::getline(meshFile, currLine);
							std::istringstream edgeData(currLine);
							unsigned int v1, v2;
							edgeData >> v1 >> v2;
							if (readMesh)
								readMesh->AddEdge(v1, v2);
						}
						continue;
					}

					// start reading in face data of the mesh
					if (currLine == "Faces")
					{
						std::getline(meshFile, currLine);
						std::istringstream faceCountText(currLine);
						int faceCount;
						faceCountText >> faceCount;
						for (int i = 0; i < faceCount; ++i)
						{
							std::getline(meshFile, currLine);
							std::istringstream faceData(currLine);
							unsigned int v1, v2, v3;
							faceData >> v1 >> v2 >> v3;
							if (readMesh)
								readMesh->AddFace(v1, v2, v3);
						}
						continue;
					}
				}
			}

			meshFile.close();
		}
	}
	catch (std::ifstream::failure error) {
		std::cout << "Failed to read from file: " << filepath << std::endl;
		if (readMesh)
			delete readMesh;
		return nullptr;
	}
	return readMesh;
}
