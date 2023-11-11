#ifndef MODULEOPENGL
#define MODULEOPENGL

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleOpenGL : public Module
{
public:
	ModuleOpenGL();
	~ModuleOpenGL();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;
	bool CleanUp() override;
	void WindowResized(unsigned width, unsigned height);

private:
	void* context;
};

#endif // !MODULEOPENGL