#ifndef GLOBALS
#define GLOBALS

//TODO: posar CMAKE per fer l'app portable i compilable optimitzadament amb clang

#define LOG(format, ...) log(__FILE__, __LINE__, format, __VA_ARGS__);

#define LOG_SIZE 4096
void log(const char file[], int line, const char* format, ...);

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define FULLSCREEN false
#define RESIZEABLE true
#define VSYNC true
#define TITLE "Super Awesome Engine"

#endif // !GLOBALS