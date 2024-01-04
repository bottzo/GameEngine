#ifndef MODULEEDITORCAMERA
#define MODULEEDITORCAMERA

#include "Module.h"
#include "Math/float4x4.h"
#include "Math/float3.h"
#include "Geometry/Frustum.h"

class ModuleEditorCamera : public Module {
public:
	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void LookAt(float3 eyePos, float3 targetPos, float3 upVector);
	void Transform(float3 vec);
	void Rotate(const float3& axix, float angleRad);
	const float3& GetFront() const { return frustum.front; }
	float4x4 GetViewMatrix() const { return frustum.ViewMatrix(); }
	float4x4 GetProjectionMatrix() const { return frustum.ProjectionMatrix(); }
private:
	Frustum frustum;
	unsigned int cameraUnis;
};

#endif // !MODULEEDITORCAMERA
