#ifndef MODULEEDITORCAMERA
#define MODULEEDITORCAMERA

#include "Module.h"
#include "Math/float4x4.h"
#include "Math/float3.h"

class ModuleEditorCamera : public Module {
	bool Init() override;

	float4x4 LookAt(float3 eyePos, float3 targetPos, float3 upVector);
private:
	float4x4 cameraMatrix;
	float4x4 projectionMatrixGL;
};

#endif // !MODULEEDITORCAMERA
