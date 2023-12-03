#ifndef MODULETEXTURES
#define MODULETEXTURES

#include "Module.h"
#include <map>
class ModuleTextures : public Module {
public:
	unsigned int GetTexture(const char* path);
private:
	unsigned int LoadTexture(const char* path);
	std::map<const char*, unsigned int> textures;
};

#endif //MODULETEXTURES