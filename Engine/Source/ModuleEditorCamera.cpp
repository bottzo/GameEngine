#include "ModuleEditorCamera.h"
#include "Math/MathConstants.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

bool ModuleEditorCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	int w, h;
	App->GetWindow()->GetWindowSize(&w, &h);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (float)w / (float)h);
	LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	return true;
}

update_status ModuleEditorCamera::Update()
{
	//bool M_Input::GetKey(int id, KeyState state) const
	//{
	//	return (keyboard[id] == state);
	//}
	if (App->GetInput()->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
		Transform(float3(0, 0.01f, 0));
	if (App->GetInput()->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
		Transform(float3(0, -0.01f, 0));
	if (App->GetInput()->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
		Transform(float3(0, 0, 0.01f));
	if (App->GetInput()->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
		Transform(float3(0, 0, -0.01f));
	if (App->GetInput()->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
		Transform(float3(-0.01f, 0, 0));
	if (App->GetInput()->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
		Transform(float3(0.01f, 0, 0));
	return UPDATE_CONTINUE;
}

void ModuleEditorCamera::Transform(float3 vec)
{
	vec.z = -vec.z;
	float3 newTrans = cameraMatrix.TransformDir(vec);
	//float3x3 rotation = cameraMatrix;
	//float3 newTrans = { cameraMatrix.ptr()[3] + vec.x, cameraMatrix.ptr()[7] + vec.y, cameraMatrix.ptr()[11] - vec.z };
	cameraMatrix.ptr()[3] += newTrans.x;
	cameraMatrix.ptr()[7] += newTrans.y;
	cameraMatrix.ptr()[11] += newTrans.z;
	view = cameraMatrix;
	view.Inverse();
	frustum.pos = { cameraMatrix.ptr()[3], cameraMatrix.ptr()[7], cameraMatrix.ptr()[11] };
	proj = frustum.ProjectionMatrix();
}

void ModuleEditorCamera::LookAt(float3 eyePos, float3 targetPos, float3 upVector) {
	float3 forward = (targetPos - eyePos);
	forward.Normalize();
	float3 right = math::Cross(forward, upVector);
	right.Normalize();
	float3 up = math::Cross(right, forward);
	up.Normalize();
	cameraMatrix = { right.x, up.x, -forward.x, eyePos.x, right.y, up.y, -forward.y, eyePos.y, right.z, up.z, -forward.z, eyePos.z, 0.0f, 0.0f, 0.0f, 1.0f };
	view = cameraMatrix;
	view.Inverse();

	frustum.pos = eyePos;
	frustum.front = -forward;
	frustum.up = up;

	proj = frustum.ProjectionMatrix();
}