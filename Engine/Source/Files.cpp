//TODO: secure no warnings no es portable
#define _CRT_SECURE_NO_WARNINGS

#include "Globals.h"
#include "GL/glew.h"
#include <stdlib.h>
#include <stdio.h>
#include "Files.h"

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

unsigned int SizeFromGlType(int glDefineType)
{
	switch (glDefineType)
	{
		case GL_BYTE:
		case GL_UNSIGNED_BYTE:
			return 1;
		case GL_SHORT:
		case GL_UNSIGNED_SHORT:
		case GL_2_BYTES:
			return 2;
		case GL_3_BYTES:
			return 3;
		case GL_INT:
		case GL_UNSIGNED_INT:
		case GL_FLOAT:
		case GL_4_BYTES:
			return 4;
		case GL_DOUBLE:
			return 8;
		default:
			LOG("WARNING: Could not identify GLTypeDefine");
	}
	return 0;
}

unsigned int TinyGltfAttributNumElements(int tinyDefineType)
{
	switch (tinyDefineType)
	{
	case TINYGLTF_TYPE_SCALAR:
		return 1;
	case TINYGLTF_TYPE_VEC2:
		return 2;
	case TINYGLTF_TYPE_VEC3:
		return 3;
	case TINYGLTF_TYPE_VEC4:
	case TINYGLTF_TYPE_MAT2:
		return 4;
	case TINYGLTF_TYPE_MAT3:
		return 9;
	case TINYGLTF_TYPE_MAT4:
		return 16;
	default:
		//case TINYGLTF_TYPE_VECTOR
		//case TINYGLTF_TYPE_MATRIX
		LOG("WARNING: Could not identify TinyGtfTypeDefine");
		assert(false && "WARNING: Could not identify TinyGtfTypeDefine");
	}
	return 0;
}

//bool StrStartsWith(const char* str, const char* start)
//{
//	assert(start != nullptr);
//	assert(str != nullptr);
//	while (start != '\0')
//		if (*start++ != *str++)
//			return false;
//	return true;
//}
//
//bool CharIsNum(char character)
//{
//	return (character <= '0' && character >= '9');
//}
//
//unsigned int CharToNum(char character)
//{
//	return (unsigned int)(character -= '0');
//}
//
//unsigned int AttributNumElements(const char* type)
//{
//	if(strcmp(type, "SCALAR") == 0)
//		return 1;
//	if (StrStartsWith(type, "VEC"))
//	{
//		assert(CharIsNum(type[3]));
//		return CharToNum(type[3]);
//	}
//	if (StrStartsWith(type, "MAT"))
//	{
//		assert(CharIsNum(type[3]));
//		return CharToNum(type[3]) * CharToNum(type[3]);
//	}
//	assert(false && "Unknown type");
//	return 0;
//}

//TODO: Create vertex attribute information
//typedef struct {
//	unsigned int offset;
//	unsigned int stride;
//} Attribute;

#include "MikkTSpace/mikktspace.h"
#include "MikkTSpace/weldmesh.h"
typedef struct {
	int numVertices;
	int posOffset;
	int texCoordOffset;
	int normOffset;
	int vertexSize;
	char* vertices;
	char* tVertices;
} MikkTSpaceStruct;

static int GetNumFaces(const SMikkTSpaceContext* pContext)
{
	MikkTSpaceStruct* ptr = (MikkTSpaceStruct*)pContext->m_pUserData;
	return ptr->numVertices / 3;
}
static int GetNumVerticesOfFace(const SMikkTSpaceContext* pContext, const int iFace) {
	return 3;
}
static void GetPosition(const SMikkTSpaceContext* pContext, float fvPosOut[], const int iFace, const int iVert)
{
	MikkTSpaceStruct* ptr = (MikkTSpaceStruct*)pContext->m_pUserData;
	float* posOut = (float*)&ptr->vertices[(iFace * 3 + iVert) * ptr->vertexSize + ptr->posOffset];
	fvPosOut[0] = posOut[0];
	fvPosOut[1] = posOut[1];
	fvPosOut[2] = posOut[2];
}
static void GetTexCoord(const SMikkTSpaceContext* pContext, float fvTexcOut[], const int iFace, const int iVert)
{
	MikkTSpaceStruct* ptr = (MikkTSpaceStruct*)pContext->m_pUserData;
	float* texCOut = (float*)&ptr->vertices[(iFace * 3 + iVert) * ptr->vertexSize + ptr->texCoordOffset];
	fvTexcOut[0] = texCOut[0];
	fvTexcOut[1] = texCOut[1];
}
static void GetNormal(const SMikkTSpaceContext* pContext, float fvNormOut[], const int iFace, const int iVert)
{
	MikkTSpaceStruct* ptr = (MikkTSpaceStruct*)pContext->m_pUserData;
	float* normalOut = (float*)&ptr->vertices[(iFace * 3 + iVert) * ptr->vertexSize + ptr->normOffset];
	fvNormOut[0] = normalOut[0];
	fvNormOut[1] = normalOut[1];
	fvNormOut[2] = normalOut[2];
}

static void SetTSpaceBasic(const SMikkTSpaceContext* pContext, const float fvTangent[], const float fSign, const int iFace, const int iVert)
{
	MikkTSpaceStruct* ptr = (MikkTSpaceStruct*)pContext->m_pUserData;
	//Escriure tota la info del vertex + les tangents
	const unsigned int vertexIdx = (iFace * 3 + iVert) * ptr->vertexSize;
	const unsigned int vertexTIdx = (iFace * 3 + iVert) * (ptr->vertexSize + sizeof(float) * 4);
	memcpy(&ptr->tVertices[vertexTIdx], &ptr->vertices[vertexIdx], ptr->vertexSize);
	memcpy(&ptr->tVertices[vertexTIdx + ptr->vertexSize], fvTangent, 3*sizeof(float));
	memcpy(&ptr->tVertices[vertexTIdx + ptr->vertexSize + 3*sizeof(float)], &fSign, sizeof(float));
}

void GenerateTangents(unsigned int indexType, unsigned int VBOEBO[1], unsigned int numIndices, unsigned int vertexSize)
{
	const unsigned int indexSize = SizeFromGlType(indexType);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	const char* indices = (const char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	const char* vertices = (const char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	char* unweldedVertices = (char*)malloc(numIndices * vertexSize);
	for (int i = 0; i < numIndices; ++i)
	{
		unsigned long long vertexIdx = 0;
		memcpy(&vertexIdx, &indices[i * indexSize], indexSize);
		//TODO: possible error en la endianes
		//vertexIdx >>= sizeof(vertexIdx) - indexSize;
		memcpy(&unweldedVertices[i * vertexSize], &vertices[vertexIdx * vertexSize], vertexSize);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	SMikkTSpaceInterface interfaceInput = {};
	interfaceInput.m_getNumFaces = GetNumFaces;
	interfaceInput.m_getNumVerticesOfFace = GetNumVerticesOfFace;
	interfaceInput.m_getNormal = GetNormal;
	interfaceInput.m_getPosition = GetPosition;
	interfaceInput.m_getTexCoord = GetTexCoord;
	interfaceInput.m_setTSpaceBasic = SetTSpaceBasic;
	MikkTSpaceStruct mikkInput = {};
	mikkInput.numVertices = numIndices;
	mikkInput.posOffset = 0;
	mikkInput.texCoordOffset = 3 * sizeof(float);
	mikkInput.normOffset = 5 * sizeof(float);
	mikkInput.vertexSize = 8 * sizeof(float);
	mikkInput.vertices = unweldedVertices;
	//Les mikktangents son vec4
	char* unweldedTVertices = (char*)malloc(numIndices * (vertexSize + 4 * sizeof(float)));
	mikkInput.tVertices = unweldedTVertices;
	SMikkTSpaceContext tangContext = {};
	tangContext.m_pInterface = &interfaceInput;
	tangContext.m_pUserData = &mikkInput;
	if (!genTangSpaceDefault(&tangContext))
		LOG("ERROR: Could not generate the tangent space");

	int* piRemapTable = (int*)malloc(mikkInput.numVertices * sizeof(int));
	float* pfVertexDataOut = (float*)malloc(mikkInput.numVertices * 12 * sizeof(float));
	unsigned int uniqueVertices =  WeldMesh(piRemapTable, pfVertexDataOut,
		(float*)mikkInput.tVertices, mikkInput.numVertices, 12);
	free(unweldedTVertices);
	free(unweldedVertices);

	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, uniqueVertices * sizeof(float), pfVertexDataOut, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, uniqueVertices * sizeof(int), piRemapTable, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), 0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 12  * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 12 * sizeof(float), (void*)(5 * sizeof(float)));
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 12 * sizeof(float),(void*)(8*sizeof(float)));
	glEnableVertexAttribArray(3);
	free(piRemapTable);
	free(pfVertexDataOut);
}

void Mesh::GenerateTangents()
{
	const unsigned int indexSize = SizeFromGlType(indexType);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	const char* indices = (const char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	const char * vertices = (const char*)glMapBuffer(GL_ARRAY_BUFFER, GL_READ_ONLY);
	char* unweldedVertices = (char*)malloc(numIndices * vertexSize);
	for (int i = 0; i < numIndices; ++i)
	{
		unsigned long long vertexIdx = 0;
		memcpy(&vertexIdx, &indices[i * indexSize], indexSize);
		//TODO: possible error en la endianes
		//vertexIdx >>= sizeof(vertexIdx) - indexSize;
		memcpy(&unweldedVertices[i * vertexSize], &vertices[vertexIdx * vertexSize], vertexSize);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	SMikkTSpaceInterface interfaceInput = {};
	interfaceInput.m_getNumFaces = GetNumFaces;
	interfaceInput.m_getNumVerticesOfFace = GetNumVerticesOfFace;
	interfaceInput.m_getNormal = GetNormal;
	interfaceInput.m_getPosition = GetPosition;
	interfaceInput.m_getTexCoord = GetTexCoord;
	interfaceInput.m_setTSpaceBasic = SetTSpaceBasic;
	MikkTSpaceStruct mikkInput = {};
	mikkInput.numVertices = numIndices;
	mikkInput.posOffset = 0;
	mikkInput.texCoordOffset = 3*sizeof(float);
	mikkInput.normOffset = 5 * sizeof(float);
	mikkInput.vertexSize = 8 * sizeof(float);
	mikkInput.vertices = unweldedVertices;
	//Les mikktangents son vec4
	char* unweldedTVertices = (char*)malloc(numIndices * (vertexSize + 4 * sizeof(float)));
	mikkInput.tVertices = unweldedTVertices;
	SMikkTSpaceContext tangContext = {};
	tangContext.m_pInterface = &interfaceInput;
	tangContext.m_pUserData = &mikkInput;
	if (!genTangSpaceDefault(&tangContext))
		LOG("ERROR: Could not generate the tangent space");
}

#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleEditorCamera.h"
#include "Math/float4x4.h"
static const char* primitivesToFind[] = { "POSITION", "TEXCOORD_0", "NORMAL" };
#define MAX_PRIMITIVES sizeof(primitivesToFind) / sizeof(*primitivesToFind)
void Mesh::LoadBufferData(const tinygltf::Model& model, const tinygltf::Accessor** accessors, const unsigned int numAccessors, char* ptr) {
	const tinygltf::BufferView* bufferViews[MAX_PRIMITIVES];
	const tinygltf::Buffer* buffers[MAX_PRIMITIVES];
	unsigned int ptrSize = 0;
	vertexSize = 0;
	for (int i = 0; i < numAccessors; ++i)
	{
		bufferViews[i] = &model.bufferViews[accessors[i]->bufferView];
		buffers[i] = &model.buffers[bufferViews[i]->buffer];
		ptrSize += accessors[i]->count * SizeFromGlType(accessors[i]->componentType) * TinyGltfAttributNumElements(accessors[i]->type);
		const unsigned int elementSize = SizeFromGlType(accessors[i]->componentType);
		const unsigned int attribElements = TinyGltfAttributNumElements(accessors[i]->type);
		vertexSize += attribElements * elementSize;
	}

	unsigned int accessorIdxs[MAX_PRIMITIVES] = {};
	for (unsigned int i = 0; i < ptrSize;)
	{
		for (unsigned int j = 0; j < numAccessors; ++j)
		{
			unsigned int offset = bufferViews[j]->byteOffset + accessors[j]->byteOffset;
			const unsigned int elementSize = SizeFromGlType(accessors[j]->componentType);
			const unsigned int attribElements = TinyGltfAttributNumElements(accessors[j]->type);
			const unsigned int sizeToCopy = attribElements * elementSize;
			memcpy(&ptr[i], &buffers[j]->data[accessorIdxs[j] + offset], sizeToCopy);
			accessorIdxs[j] += sizeToCopy;
			i += sizeToCopy;
		}

	}
	unsigned int attributeOffset = 0;
	for (int i = 0; i < numAccessors; ++i)
	{
		glVertexAttribPointer(i, TinyGltfAttributNumElements(accessors[i]->type), accessors[i]->componentType, GL_FALSE, vertexSize, (void*)attributeOffset);
		glEnableVertexAttribArray(i);
		const unsigned int elementSize = SizeFromGlType(accessors[i]->componentType);
		const unsigned int attribElements = TinyGltfAttributNumElements(accessors[i]->type);
		attributeOffset += attribElements * elementSize;
	}

}

void Mesh::Load(const tinygltf::Model& model, const tinygltf::Mesh& mesh, const tinygltf::Primitive& primitive)
{
	unsigned int numAccessors = 0;
	const tinygltf::Accessor* accessors[MAX_PRIMITIVES];
	for (int i = 0; i < MAX_PRIMITIVES; ++i)
	{
		const std::map<std::string, int>::const_iterator it = primitive.attributes.find(primitivesToFind[i]);
		if (it != primitive.attributes.end())
			accessors[numAccessors++] = &model.accessors[it->second];
	}
	unsigned int sizeOfData = 0;
	for (int i = 0; i < numAccessors; ++i)
		sizeOfData += accessors[i]->count * SizeFromGlType(accessors[i]->componentType) * TinyGltfAttributNumElements(accessors[i]->type);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, VBOEBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeOfData, nullptr, GL_STATIC_DRAW);
	char* ptr = (char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	LoadBufferData(model, accessors, numAccessors, ptr);
	glUnmapBuffer(GL_ARRAY_BUFFER);

	const tinygltf::Accessor& idxAccessor = model.accessors[primitive.indices];
	const tinygltf::BufferView& idxView = model.bufferViews[idxAccessor.bufferView];
	const tinygltf::Buffer& idxBuffer = model.buffers[idxView.buffer];
	const unsigned int idxSize = idxAccessor.count * SizeFromGlType(idxAccessor.componentType);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxSize, nullptr, GL_STATIC_DRAW);
	ptr = (char*)glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &idxBuffer.data[idxView.byteOffset], idxSize);
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	numIndices = idxAccessor.count;
	indexType = idxAccessor.componentType;

	const tinygltf::Texture& tex = model.textures[model.materials[primitive.material].pbrMetallicRoughness.baseColorTexture.index];
	const tinygltf::Image& img = model.images[tex.source];
	texIdx = App->textures->GetTexture(img.uri.c_str());

	glBindVertexArray(0);

	GenerateTangents();

	//TODO: evitar crear programa a cada load
	//Com pasar els uniforms per a cada prorama?
	//Unifrom buffer per lighting
	programId = CreateProgram("Shaders/MeshVertex.glsl", "Shaders/MeshFragment.glsl");
	
	assert(programId && "Failed to create mesh program");
	glUseProgram(programId);
	//uniforms
	float4x4 modelMat = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f), float4x4::identity, float3(100.0f, 100.0f, 100.0f));
	glUniformMatrix4fv(0, 1, GL_TRUE, modelMat.ptr());
	glUniform3f(4, 0.0f, 0.0f, 0.1f);
	glUniform3f(5, 1.f, 1.f, 1.f);
	glUniform3f(6, 0.3f, 0.4f, 0.6f);
	glUniform3fv(7, 1, App->editorCamera->GetFront().ptr());
	glUniform1f(8, 0.1f);
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glUseProgram(programId);
	if (texIdx != 0)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texIdx);
		glUniform1i(3, 0);

		glUniformMatrix4fv(1, 1, GL_TRUE, App->editorCamera->GetViewMatrix().ptr());
		glUniformMatrix4fv(2, 1, GL_TRUE, App->editorCamera->GetProjectionMatrix().ptr());
	}
	glDrawElements(GL_TRIANGLES, numIndices, indexType, 0);
}

bool LoadGLTFModel(const char* assetPath, std::vector<Mesh*>& out)
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
			out.push_back(myMesh);
		}
	}

	return true;
}