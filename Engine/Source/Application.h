#ifndef APP
#define APP

#include<vector>
#include "Globals.h"

class Module;
class ModuleOpenGL;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleEditor;
class ModuleRenderMandelbrot;
class ModuleRenderExercise;
class ModuleDebugDraw;
class ModuleTextures;
class ModuleEditorCamera;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

    ModuleOpenGL* GetOpenGL() { return render; }
    ModuleWindow* GetWindow() { return window; }
    ModuleInput*  GetInput() { return input; }
    ModuleEditor*  GetEditor() { return editor; }
    ModuleRenderMandelbrot*  GetMandelbrot() { return mandelbrot; }
    ModuleRenderExercise*  GetExercise() { return exercise; }
    ModuleDebugDraw*  GetDebugDraw() { return debugDraw; }
    ModuleTextures*  GetTextures() { return textures; }
    ModuleEditorCamera* GetEditorCamera() { return editorCamera; }

private:

    ModuleOpenGL* render = nullptr;
    ModuleWindow* window = nullptr;
    ModuleInput* input = nullptr;
    ModuleEditor* editor = nullptr;
    ModuleRenderMandelbrot* mandelbrot = nullptr;
    ModuleRenderExercise* exercise = nullptr;
    ModuleDebugDraw* debugDraw = nullptr;
    ModuleTextures* textures = nullptr;
    ModuleEditorCamera* editorCamera = nullptr;

    std::vector<Module*> modules;

};

extern Application* App;

#endif // !APP