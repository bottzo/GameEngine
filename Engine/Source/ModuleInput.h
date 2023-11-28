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

enum MouseButtons {
	BUTTON_LEFT,  
	BUTTON_MIDDLE,
	BUTTON_RIGHT, 
	BUTTON_X1,    
	BUTTON_X2,

	NUM_MOUSE_BUTTONS
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
	KeyState GetMouseKey(MouseButtons id) const { return mouse[id]; };
	void GetMouseMorion(int& x, int& y) const { x = mX; y = mY; }
private:
	KeyState mouse[MouseButtons::NUM_MOUSE_BUTTONS] = {};
	KeyState* keyboard = NULL;
	int mX;
	int mY;
	unsigned int mouseBitmask;
};

#endif // !MODULEINPUT