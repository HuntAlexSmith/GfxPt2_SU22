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