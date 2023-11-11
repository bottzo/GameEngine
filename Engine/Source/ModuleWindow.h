#ifndef MODULEWINDOW
#define MODULEWINDOW

#include "Module.h"

class Application;
struct SDL_Window;
struct SDL_Surface;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	~ModuleWindow();

	// Called before quitting
	bool Init() override;

	// Called before quitting
	bool CleanUp() override;

	void GetWindowSize(int* w, int* h);

	//The window we'll be rendering to
	SDL_Window* window = NULL;

private:
	//TODO: dont like this public but needed to create the opengl context
	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;
};

#endif // MODULEWINDOW