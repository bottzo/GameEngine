//TODO: secure no warnings no es portable
#define _CRT_SECURE_NO_WARNINGS

#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleWindow.h"
#include "GL/glew.h"
#include <stdlib.h>
#include <stdio.h>

//TODO: on poso aquesta funcio!!!
bool FileToBuffer(const char* filePath, char** buffer)
{
	//Note: necesito posar tambe el mode de obrir el file??
	FILE* file = fopen(filePath, "rb");
	if (file == NULL)
		return false;
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	*buffer = (char*)malloc(size + 1);
	fseek(file, 0, SEEK_SET);
	fread(*buffer, 1, size, file);
	(*buffer)[size] = '\0';
	fclose(file);
	return true;
}

//TODO: on poso aquesta funcio
static unsigned int CompileShader(const char* sourcePath, GLenum type)
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
static unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath)
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

//TODO: Uniform struct for mandlebrot bad
typedef struct {
	unsigned int colorPeriod;
	unsigned int maxIterations;
} Uniforms;

ModuleRenderExercise::ModuleRenderExercise() 
{
}

ModuleRenderExercise::~ModuleRenderExercise() 
{
}

bool ModuleRenderExercise::Init()
{
	//programId = CreateProgram("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	programId = CreateProgram("Shaders/MandelbrotVertex.glsl", "Shaders/MandelbrotFragment.glsl");

	if (programId == 0)
		return false;
	glUseProgram(programId);
	double centerX = -0.55;
	double centerY = 0.0;
	double cLength = 2.0;
	Uniforms unis = { 24, 1000 };
	glUniform1ui(0, unis.maxIterations);
	glUniform1ui(1, unis.colorPeriod);
	int w, h;
	App->GetWindow()->GetWindowSize(&w, &h);
	glUniform2i(2, w, h);
	glUniform1d(3, cLength);
	glUniform2d(4, centerX, centerY);


	float vertex[] = {
	-0.95f, -0.95f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.95f, -0.95f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.95f,  0.95f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.95f, 0.95f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBOEBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	unsigned int indices[6] = { 0,1,2,3,2,1 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderExercise::Update()
{
	glBindVertexArray(VAO);
	glUseProgram(programId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBOEBO);
	return true;
}