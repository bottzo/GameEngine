#ifndef MODULERENDEREXERCISE
#define MODULERENDEREXERCISE

#include "Module.h"
#include "Globals.h"

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
};

#endif // !MODULERENDEREXERCISE