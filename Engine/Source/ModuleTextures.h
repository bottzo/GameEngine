#ifndef MODULETEXTURES
#define MODULETEXTURES

#include "Module.h"

class ModuleTextures : public Module {
public:
	unsigned int LoadTexture(const char* path);
};

#endif //MODULETEXTURES