#ifndef MODULETEXTURES
#define MODULETEXTURES

#include "Module.h"

class ModuleTextures : public Module {
public:
	void* LoadTexture(const char* path);
};

#endif //MODULETEXTURES