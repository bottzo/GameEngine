#ifndef MODULEEDITOR
#define MODULEEDITOR

#include "Module.h"
#include "Globals.h"
#include <list>
#include <string>

class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init() override;
	update_status PreUpdate() override;
	update_status Update() override;
	update_status PostUpdate() override;

	void ConsoleLog(const char* log);
	bool CleanUp() override;

private:
	std::list<std::string> logs;
	unsigned int logSize = 0;
};

#endif // !MODULEEDITOR