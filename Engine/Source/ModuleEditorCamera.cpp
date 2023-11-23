#include "ModuleEditorCamera.h"

bool ModuleEditorCamera::Init()
{
	
	return true;
}

void ModuleEditorCamera::LookAt(float3 eyePos, float3 targetPos, float3 upVector) {
	float3 forward = (targetPos - eyePos);
	forward.Normalize();
	float3 right = math::Cross(forward, upVector);
	right.Normalize();
	float3 up = math::Cross(right, forward);
	up.Normalize();
	cameraMatrix = { right.x, up.x, -forward.x, eyePos.x, right.y, up.y, -forward.y, eyePos.y, right.z, up.z, -forward.z, eyePos.z, 0.0f, 0.0f, 0.0f, 1.0f };
}