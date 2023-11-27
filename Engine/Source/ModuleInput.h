#ifndef MODULEINPUT
#define MODULEINPUT

#include "Module.h"
#include "Globals.h"
#include "SDL_scancode.h"

typedef unsigned __int8 Uint8;

enum class KeyState: unsigned char
{
	KEY_IDLE = 0,
	KEY_DOWN,
	KEY_REPEAT,
	KEY_UP
};

class ModuleInput : public Module
{
public:
	
	ModuleInput();
	~ModuleInput();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	KeyState GetKey(int id) const { return keyboard[id]; };
	void GetMouseMorion(int& x, int& y) const { x = mX; y = mY; }
private:
	KeyState* keyboard = NULL;
	int mX;
	int mY;
	unsigned int mouseBitmask;
};

#endif // !MODULEINPUT