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
	App->window->GetWindowSize(&w, &h);
	frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov * 0.5f) * (float)w / (float)h);
	LookAt(float3(0.0f, 4.0f, 8.0f), float3(0.0f, 0.0f, 0.0f), float3::unitY);
	return true;
}

update_status ModuleEditorCamera::Update()
{
	//Fer state machine amb els inputs !!!!
	//Camera velocity variable independent of framerate
	//state moving/rot camera
	if(App->input->GetMouseWheelMotion() != 0)
		Transform(float3(0, 0, 0.05f* App->input->GetMouseWheelMotion()));
	if (App->input->GetMouseKey(MouseButtons::BUTTON_RIGHT) == KeyState::KEY_REPEAT)
	{
		int mX, mY;
		App->input->GetMouseMotion(mX, mY);
		Rotate(float3::unitY, -mX*0.002);
		//TODO: save the right vector myself??
		Rotate(frustum.WorldRight(), -mY*0.002);
		if (App->input->GetKey(SDL_SCANCODE_Q) == KeyState::KEY_REPEAT)
			Transform(float3(0, -0.01f, 0));
		if (App->input->GetKey(SDL_SCANCODE_E) == KeyState::KEY_REPEAT)
			Transform(float3(0, 0.01f, 0));
		if (App->input->GetKey(SDL_SCANCODE_W) == KeyState::KEY_REPEAT)
			Transform(float3(0, 0, 0.01f));
		if (App->input->GetKey(SDL_SCANCODE_S) == KeyState::KEY_REPEAT)
			Transform(float3(0, 0, -0.01f));
		if (App->input->GetKey(SDL_SCANCODE_A) == KeyState::KEY_REPEAT)
			Transform(float3(-0.01f, 0, 0));
		if (App->input->GetKey(SDL_SCANCODE_D) == KeyState::KEY_REPEAT)
			Transform(float3(0.01f, 0, 0));
	}
	//state paning camera
	if (App->input->GetMouseKey(MouseButtons::BUTTON_MIDDLE) == KeyState::KEY_REPEAT)
	{
		int mX, mY;
		App->input->GetMouseMotion(mX, mY);
		Transform(float3(-mX * 0.01f, 0, 0));
		Transform(float3(0, mY * 0.01f, 0));
	}
	//state orbiting camera
	if (App->input->GetMouseKey(MouseButtons::BUTTON_LEFT) == KeyState::KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_LALT) == KeyState::KEY_REPEAT)
	{
		float3 right = frustum.WorldRight();
		float3 focus = frustum.pos; //(cameraPos - targetPos)
		int mX, mY;
		App->input->GetMouseMotion(mX, mY);
		float3x3 rotationMatrix = float3x3::RotateAxisAngle(frustum.up, -mX*0.002f) * float3x3::RotateAxisAngle(right, -mY * 0.002f);
		focus = rotationMatrix.MulDir(focus);
		frustum.pos = 
	}

	return UPDATE_CONTINUE;
}

void ModuleEditorCamera::Rotate(const float3& axis, float angleRad)
{
	//float3x4 world = frustum.WorldMatrix();
	//frustum.SetWorldMatrix(world.Mul(float3x4::RotateAxisAngle(axis, angleRad)));
	// 
	//float3x3 worldRot = world.Float3x3Part();
	//world.Set3x3Part(worldRot.Mul(float3x3::RotateAxisAngle(axis, angleRad)));
	//frustum.SetWorldMatrix(world);
	float3x3 rotationMatrix = float3x3::RotateAxisAngle(axis, angleRad);
	frustum.up = rotationMatrix.Mul(frustum.up);
	frustum.front = rotationMatrix.Mul(frustum.front);
}

void ModuleEditorCamera::Transform(float3 vec)
{
	vec.z = -vec.z;
	float3x4 world = frustum.WorldMatrix();
	float3 newTrans = world.TransformDir(vec);
	world.SetTranslatePart(world.TranslatePart() + newTrans);
	frustum.SetWorldMatrix(world);
}


void ModuleEditorCamera::LookAt(float3 eyePos, float3 targetPos, float3 upVector) {
	float3 forward = (targetPos - eyePos);
	forward.Normalize();
	float3 right = math::Cross(forward, upVector);
	right.Normalize();
	float3 up = math::Cross(right, forward);
	up.Normalize();

	frustum.pos = eyePos;
	frustum.front = forward;
	frustum.up = up;
}