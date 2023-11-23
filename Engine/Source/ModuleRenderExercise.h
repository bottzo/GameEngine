#ifndef MODULERENDEREXERCISE
#define MODULERENDEREXERCISE

#include "Module.h"
#include "Globals.h"
#include "Math/float4x4.h"

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

private:
	unsigned int programId;
	unsigned int VAO;
	unsigned int VBOEBO[2];

	float4x4 view;
	float4x4 proj;

	unsigned int baboonTex;
};

#endif // !MODULERENDEREXERCISE