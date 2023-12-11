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

	unsigned int baboonTex;

	std::vector<Mesh*>meshes;
};

#endif // !MODULERENDEREXERCISE