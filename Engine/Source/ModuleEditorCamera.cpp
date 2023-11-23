#include "ModuleEditorCamera.h"
#include "Math/MathConstants.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

bool ModuleEditorCamera::Init()
{
	frustum.type = FrustumType::PerspectiveFrustum;
	frustum.pos = float3::zero;
	frustum.front = -float3::unitZ;
	frustum.up = float3::unitY;
	frustum.nearPlaneDistance = 0.1f;
	frustum.farPlaneDistance = 100.0f;
	frustum.verticalFov = math::pi / 4.0f;
	int w, h;
	App->GetWindow()->GetWindowSize(&w, &h);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (float)w / (float)h);
	LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	proj = frustum.ProjectionMatrix();
	return true;
}

update_status ModuleEditorCamera::Update()
{
	//bool M_Input::GetKey(int id, KeyState state) const
	//{
	//	return (keyboard[id] == state);
	//}
	if(App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
	return UPDATE_CONTINUE;
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
}