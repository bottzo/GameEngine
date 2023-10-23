#include "Globals.h"
#include "Application.h"
#include "ModuleOpenGL.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL/glew.h"

ModuleOpenGL::ModuleOpenGL()
{
}

// Destructor
ModuleOpenGL::~ModuleOpenGL()
{
}

// Called before render is available
bool ModuleOpenGL::Init()
{
	LOG("Creating Renderer context");

	context = SDL_GL_CreateContext(App->GetWindow()->window);

	GLenum err = glewInit();
	// … check for errors
	if (err != GLEW_OK)
		return false;
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	// Should be 2.0
	
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));



	int shaderErr = 0;
	char log[500];
	unsigned int vShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vShaderSource = "#version 460 core\nlayout(location = 0) in vec3 pos;\nlayout(location = 1) in vec3 inCol;\nout vec3 col;\nvoid main(){\ncol=inCol;\ngl_Position = vec4(pos, 1.0f);\n}";
	glShaderSource(vShader, 1, &vShaderSource, NULL);
	glCompileShader(vShader);
	glGetShaderiv(vShader, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetShaderInfoLog(vShader, 500 * sizeof(char), NULL, log);
		//LOG("GLSL: ");
		return false;
	}
	unsigned int fShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fShaderSource = "#version 460 core\nin vec3 col; out vec4 fragColor;\nvoid main(){\nfragColor = vec4(col, 1.0f);\n}";
	glShaderSource(fShader, 1, &fShaderSource, NULL);
	glCompileShader(fShader);
	glGetShaderiv(fShader, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetShaderInfoLog(fShader, 500 * sizeof(char), NULL, log);
		return false;
	}
	programId = glCreateProgram();
	glAttachShader(programId, vShader);
	glAttachShader(programId, fShader);
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		glGetProgramInfoLog(programId, 500 * sizeof(char), NULL, log);
		return false;
	}

	float vertex[] = {
	-0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f,
	 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);

	//unsigned int indices[6] = {1,2,3,4,3,2};
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return true;
}

update_status ModuleOpenGL::PreUpdate()
{
	glEnable(GL_DEPTH_TEST); // Enable depth test
	glEnable(GL_CULL_FACE); // Enable cull backward faces
	glFrontFace(GL_CCW); // Front faces will be counter clockwise
	
	int w = 0;
	int h = 0;
	SDL_GetWindowSize(App->GetWindow()->window, &w, &h);
	glViewport(0, 0, w, h);
	glClearColor(0.f, 0.f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleOpenGL::Update()
{

	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	glBindVertexArray(VAO);
	glUseProgram(programId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	SDL_GL_SwapWindow(App->GetWindow()->window);
	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleOpenGL::CleanUp()
{
	LOG("Destroying renderer");

	SDL_GL_DeleteContext(context);

	//Destroy window

	return true;
}

void ModuleOpenGL::WindowResized(unsigned width, unsigned height)
{
}

