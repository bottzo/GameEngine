#include "ModuleRenderExercise.h"
#include "GL/glew.h"
#include "Files.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "Geometry/Frustum.h"
#include "Math/MathAll.h"
#include "ModuleDebugDraw.h"

ModuleRenderExercise::ModuleRenderExercise() 
{
}

ModuleRenderExercise::~ModuleRenderExercise()
{
}

float4x4 LookAt(float3 eyePos, float3 targetPos, float3 upVector) {
	float3 forward = (targetPos - eyePos);
	forward.Normalize();
	float3 right = math::Cross(forward, upVector);
	right.Normalize();
	float3 up = math::Cross(right, forward);
	up.Normalize();
	return { right.x, up.x, -forward.x, eyePos.x, right.y, up.y, -forward.y, eyePos.y, right.z, up.z, -forward.z, eyePos.z, 0.0f, 0.0f, 0.0f, 1.0f };
}

bool ModuleRenderExercise::Init()
{
	programId = CreateProgram("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");

	if (programId == 0)
		return false;
	glUseProgram(programId);
	//uniforms
	Frustum frustum;
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	int w, h;
	App->GetWindow()->GetWindowSize(&w, &h);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (float)w/(float)h);
	float4x4 model = float4x4::FromTRS(float3(2.0f, 0.0f, 0.0f),float4x4::RotateZ(pi / 4.0f),float3(1.0f, 1.0f, 1.0f));
	view = LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	if (!view.Inverse())
		LOG("AAAAAAAAAA");
	//float4x4 view = frustum.ViewMatrix();
	proj = frustum.ProjectionMatrix();

	glUniformMatrix4fv(0, 1, GL_TRUE, model.ptr());
	glUniformMatrix4fv(1, 1, GL_TRUE, view.ptr());
	glUniformMatrix4fv(2, 1, GL_TRUE, proj.ptr());

	float vertex[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBOEBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

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
	int w, h;
	App->GetWindow()->GetWindowSize(&w, &h);
	App->GetDebugDraw()->Draw(view, proj, w, h);
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