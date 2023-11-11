#include "Module.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleOpenGL.h"
#include "ModuleInput.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"

Application::Application()
{
	modules.reserve(4);
	// Order matters: they will Init/start/update in this order
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(render = new ModuleOpenGL());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(exercise = new ModuleRenderExercise());
}

Application::~Application()
{
	for(std::vector<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
        delete *it;
}

bool Application::Init()
{
	bool ret = true;

	for(std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;

	for(std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PreUpdate();

	for(std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->Update();

	for(std::vector<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
		ret = (*it)->PostUpdate();

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(std::vector<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
