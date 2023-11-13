#ifndef MODULERENDERMANDELBROT
#define MODULERENDERMANDELBROT

#include "Module.h"
#include "Globals.h"

//TODO: Uniform struct for mandlebrot bad
typedef struct {
	unsigned int colorPeriod;
	unsigned int maxIterations;
} UintUniforms;

class ModuleRenderMandelbrot : public Module
{
public:
	ModuleRenderMandelbrot();
	~ModuleRenderMandelbrot();

	bool Init() override;
	update_status Update() override;
	bool CleanUp() override;

	void WindowUniform(int width, int height);

private:
	unsigned int VAO;
	unsigned int VBOEBO[2];
	unsigned int programId;

	double centerX = -0.55;
	double centerY = 0.0;
	double cLength = 2.0;
	UintUniforms unis = { 24, 1000 };
	double stepCenter = cLength * 0.01;
	double stepCLength = 0.1;
	double prevCLength = cLength;
	float colors[6] = { 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.f };
};

#endif // !MODULERENDERMANDELBROT