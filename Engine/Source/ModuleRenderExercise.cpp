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
#include "imgui.h"

ModuleRenderExercise::ModuleRenderExercise() 
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

bool ModuleRenderExercise::Init()
{
	programId = CreateProgram("Shaders/MeshVertex.glsl", "Shaders/MeshFragment.glsl");

	if (programId == 0)
		return false;
	glUseProgram(programId);
	//uniforms
	//float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),float4x4::RotateZ(pi / 4.0f),float3(1.0f, 1.0f, 1.0f));
	float4x4 model = float4x4::FromTRS(float3(1.0f, 0.0f, 0.0f), float4x4::RotateX(-pi / 4.0f), float3(2.5f, 2.5f, 2.5f));
	glUniformMatrix4fv(0, 1, GL_TRUE, model.ptr());
	//glUniformMatrix4fv(1, 1, GL_TRUE, App->editorCamera->GetViewMatrix().ptr());
	//glUniformMatrix4fv(2, 1, GL_TRUE, App->editorCamera->GetProjectionMatrix().ptr());


	glUniform3fv(4, 1, lightDir);
	glUniform3fv(5, 1, lightCol);
	glUniform3fv(6, 1, ambientCol);
	glUniform3fv(7, 1, App->editorCamera->GetFront().ptr());
	glUniform1f(8, kD);
	glUniform1f(10, brightness);

	//unsigned int index = glGetUniformBlockIndex(programId, "CameraMatrices");
	//glUniformBlockBinding(programId, index, 0);
	//glUniformBlockBinding(programId, 1, 0);

	float vertex[] = {
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_UNIFORM_BUFFER, App->editorCamera->GetCameraUniffromsId());

	glGenBuffers(2, VBOEBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	unsigned int indices[6] = { 0,1,2,3,2,1 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	GenerateTangents(GL_UNSIGNED_INT, VBOEBO, 6, 8 * sizeof(float));

	glBindVertexArray(0);
	wallTex = App->textures->GetTexture("brickwall.jpg");
	wallNormTex = App->textures->GetTexture("brickwall_normal.jpg");
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wallTex);
	//glUseProgram(programId);
	glUniform1i(3, 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, wallNormTex);
	glUniform1i(9, 3);

	//LoadGLTFModel("BakerHouse.gltf", meshes);


	return true;
}

update_status ModuleRenderExercise::Update()
{
	glBindVertexArray(VAO);
	glUseProgram(programId);

	//glUniformMatrix4fv(1, 1, GL_TRUE, App->editorCamera->GetViewMatrix().ptr());
	//glUniformMatrix4fv(2, 1, GL_TRUE, App->editorCamera->GetProjectionMatrix().ptr());

	glUniform3fv(7, 1, App->editorCamera->GetFront().ptr());
	ImGui::Begin("Lights");
	if (ImGui::DragFloat("KD", &kD, 0.0f, 1.0f))
		glUniform1f(8, kD);
	if (ImGui::DragFloat("Brightness", &brightness, 0.0f, 1.0f))
		glUniform1f(10, brightness);
	if (ImGui::DragFloat3("LightDir", lightDir, 0.0f, 1.0f))
		glUniform3fv(4, 1, lightDir);
	if (ImGui::ColorPicker3("LightCol", lightCol))
		glUniform3fv(5, 1, lightCol);
	if (ImGui::ColorPicker3("AmbientCol", ambientCol))
		glUniform3fv(6, 1, ambientCol);
	ImGui::End();


	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, wallTex);
	//glUniform1i(3, 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, wallNormTex);

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
	glDeleteTextures(1, &wallTex);
	glDeleteTextures(1, &wallNormTex);
	return true;
}

void ModuleRenderExercise::LoadModel(const char* path)
{
	LOG("Loading model %s", path);
	LoadGLTFModel(path, meshes);
}