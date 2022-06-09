#pragma once
//*****************************************************************************
//	File:   FileReader.h
//  Author: Hunter Smith
//  Date:   05/20/2022
//  Description: Functions for reading the contents of a file and outputting
//		what was in the files
//*****************************************************************************

#include <string>
#include "Mesh.h"

std::string ReadShaderFile(const char* filepath);

void WriteMeshFile(Mesh* mesh);
Mesh* ReadMeshFile(const char* filepath);