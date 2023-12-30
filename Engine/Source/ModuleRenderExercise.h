#ifndef MODULERENDEREXERCISE
#define MODULERENDEREXERCISE

#include "Module.h"
#include "Globals.h"
#include "Math/float4x4.h"

#include <vector>
class Mesh;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void LoadModel(const char* path);

private:
	unsigned int programId;
	unsigned int VAO;
	unsigned int VBOEBO[2];

	unsigned int wallTex;
	unsigned int wallNormTex;

	float brightness = 20;
	float kD = 0.2;
	float lightDir[3] = { 0.0f, -1.0f, -1.0f };
	float lightCol[3] = { 1.f, 1.f, 1.f };
	float ambientCol[3] = { 0.3f, 0.4f, 0.6f };

	std::vector<Mesh*>meshes;
};

#endif // !MODULERENDEREXERCISE