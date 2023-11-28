//TODO: secure no warnings no es portable
#define _CRT_SECURE_NO_WARNINGS

#include "Globals.h"
#include "GL/glew.h"
#include <stdlib.h>
#include <stdio.h>

#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_EXTERNAL_IMAGE
#define TINYGLTF_IMPLEMENTATION /* Only in one of the includes */
#include "tiny_gltf.h"

//TODO: on poso aquesta funcio!!!
long FileToBuffer(const char* filePath, char** buffer)
{
	//Note: necesito posar tambe el mode de obrir el file??
	FILE* file = fopen(filePath, "rb");
	if (file == NULL)
		return 0;
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	*buffer = (char*)malloc(size + 1);
	fseek(file, 0, SEEK_SET);
	fread(*buffer, 1, size, file);
	(*buffer)[size] = '\0';
	fclose(file);
	return size;
}

//TODO: on poso aquesta funcio
unsigned int CompileShader(const char* sourcePath, GLenum type)
{
	char* sourceBuffer = nullptr;
	unsigned int shaderId = glCreateShader(type);
	if (!FileToBuffer(sourcePath, &sourceBuffer))
	{
		glDeleteShader(shaderId);
		return 0;
	}
	glShaderSource(shaderId, 1, &sourceBuffer, NULL);
	free(sourceBuffer);
	sourceBuffer = nullptr;
	glCompileShader(shaderId);
	int shaderErr = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		if (length == 0)
			return 0;
		char* errMessage = (char*)malloc(length);
		glGetShaderInfoLog(shaderId, length, NULL, errMessage);
		LOG("%s\n", errMessage);
		free(errMessage);
		return 0;
	}
	return shaderId;
}

//TODO: on poso aquesta funcio
//Varios paths input
//funcio ha de poder deduir de qiun tipus de shader es tracta (vertex/geometry/fragment)
//construir el programa apartir de tots els shaders de input
unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath)
{
	unsigned int vShaderId = CompileShader(vShaderPath, GL_VERTEX_SHADER);
	if (!vShaderId)
		return 0;
	unsigned int fShaderId = CompileShader(fShaderPath, GL_FRAGMENT_SHADER);
	if (!fShaderId)
	{
		glDeleteShader(vShaderId);
		return 0;
	}
	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	int shaderErr = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		if (length == 0)
			return 0;
		char* errMessage = (char*)malloc(length);
		glGetProgramInfoLog(programId, length, NULL, errMessage);
		LOG("%s", errMessage);
		free(errMessage);
		return 0;
	}
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);
	return programId;
}


class Mesh {
	unsigned int VBOEBO[2];
	unsigned int materialIdx;

public:
	void Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
	{
		const std::map<std::string,int>::const_iterator posIt = primitive.attributes.find("POSITION");
		if (posIt != primitive.attributes.end())
		{
			const tinygltf::Accessor& accessor = model.accessors[posIt->second];
			const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
			const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

			bufferView.byteStride;
			glGenBuffers(2, VBOEBO);
			glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
			//GL_UNSIGNED_SHORT
			glBufferData(GL_ARRAY_BUFFER, accessor.count * accessor.componentType, nullptr, GL_STATIC_DRAW);
			//glMapBuffer()
		}
	}
};

bool LoadGLTFModel(const char* assetPath)
{
	tinygltf::TinyGLTF gltfContext;
	tinygltf::Model model;

	std::string errors;
	std::string warnings;
	bool loaded = gltfContext.LoadASCIIFromFile(&model, &errors, &warnings, assetPath);

	if (!loaded)
	{
		LOG("Error loading %s: %s", assetPath, errors.c_str());
		return false;
	}
	for (const tinygltf::Mesh& mesh : model.meshes)
	{
		for (const tinygltf::Primitive& primitive : mesh.primitives)
		{
			Mesh* myMesh = new Mesh;
			myMesh->Load(model, mesh, primitive);
		}
	}

	return true;
}