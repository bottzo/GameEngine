#ifdef _WIN32
#include <windows.h>
#endif // WIN32
#include <stdio.h>
#include "Application.h"
#include "ModuleEditor.h"

//TODO: this log is just for windows, not portable
void log(const char file[], int line, const char* format, ...)
{
	static char tmp_string[LOG_SIZE];
	static char tmp_string2[LOG_SIZE];
	static va_list  ap;

	// Construct the string from variable arguments
	va_start(ap, format);
	vsprintf_s(tmp_string, LOG_SIZE, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, LOG_SIZE, "\n%s(%d) : %s", file, line, tmp_string);
#ifdef _WIN32
	OutputDebugString(tmp_string2);
#endif //_WIN32
	if(App != nullptr)
		App->editor->ConsoleLog(tmp_string2);
}