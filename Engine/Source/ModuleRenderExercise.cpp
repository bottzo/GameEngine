#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Files.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Geometry/Frustum.h"
#include "Math/MathAll.h"
#include "ModuleTextures.h"
#include "ModuleEditorCamera.h"
#include "Files.h"

ModuleRenderExercise::ModuleRenderExercise() 
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	programId = CreateProgram("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");

	if (programId == 0)
		return false;
	glUseProgram(programId);
	//uniforms
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),float4x4::RotateZ(pi / 4.0f),float3(1.0f, 1.0f, 1.0f));
	glUniformMatrix4fv(0, 1, GL_TRUE, model.ptr());
	glUniformMatrix4fv(1, 1, GL_TRUE, App->editorCamera->GetViewMatrix().ptr());
	glUniformMatrix4fv(2, 1, GL_TRUE, App->editorCamera->GetProjectionMatrix().ptr());

	float vertex[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBOEBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	unsigned int indices[6] = { 0,1,2,3,2,1 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	baboonTex = App->textures->GetTexture("Test-image-Baboon.tga");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, baboonTex);
	//glUseProgram(programId);
	glUniform1i(3, 1);

	//LoadGLTFModel("BakerHouse.gltf", meshes);


	return true;
}

update_status ModuleRenderExercise::Update()
{
	glBindVertexArray(VAO);
	glUseProgram(programId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);

	glUniformMatrix4fv(1, 1, GL_TRUE, App->editorCamera->GetViewMatrix().ptr());
	glUniformMatrix4fv(2, 1, GL_TRUE, App->editorCamera->GetProjectionMatrix().ptr());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, baboonTex);
	//glUniform1i(3, 1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);

	for (Mesh* mesh : meshes)
		mesh->Draw();

	return UPDATE_CONTINUE;
}

bool ModuleRenderExercise::CleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBOEBO);
	glDeleteTextures(1, &baboonTex);
	return true;
}

void ModuleRenderExercise::LoadModel(const char* path)
{
	LOG("Loading model %s", path);
	LoadGLTFModel(path, meshes);
}