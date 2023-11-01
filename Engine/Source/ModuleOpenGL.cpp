//TODO: secure no warnings no es portable
#define _CRT_SECURE_NO_WARNINGS

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

//TODO: On posem callback funtion? En global space, static o en la classe?
static void __stdcall OpenGLErrorFunction(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
{
	const char* tmp_source = "", * tmp_type = "", * tmp_severity = "";
	switch (source) {
	case GL_DEBUG_SOURCE_API: tmp_source = "API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM: tmp_source = "Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: tmp_source = "Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY: tmp_source = "Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION: tmp_source = "Application"; break;
	case GL_DEBUG_SOURCE_OTHER: tmp_source = "Other"; break;
	};
	switch (type) {
	case GL_DEBUG_TYPE_ERROR: tmp_type = "Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: tmp_type = "Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR: tmp_type = "Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY: tmp_type = "Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE: tmp_type = "Performance"; break;
	case GL_DEBUG_TYPE_MARKER: tmp_type = "Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP: tmp_type = "Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP: tmp_type = "Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER: tmp_type = "Other"; break;
	};
	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH: tmp_severity = "high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM: tmp_severity = "medium"; break;
	case GL_DEBUG_SEVERITY_LOW: tmp_severity = "low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: tmp_severity = "notification"; break;
	};
	LOG("<Source:%s> <Type:%s> <Severity:%s> <ID:%d> <Message:%s>\n", tmp_source, tmp_type, tmp_severity, id, message);
}

//TODO: on poso aquesta funcio!!!
bool FileToBuffer(const char* filePath, char** buffer)
{
	//Note: necesito posar tambe el mode de obrir el file??
	FILE* file = fopen(filePath, "rb");
	if (file == NULL)
		return false;
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	*buffer = (char*)malloc(size + 1);
	fseek(file, 0, SEEK_SET);
	fread(*buffer, 1, size, file);
	(*buffer)[size] = '\0';
	fclose(file);
	return true;
}

//TODO: on poso aquesta funcio
static unsigned int CompileShader(const char* sourcePath, GLenum type)
{
	char* sourceBuffer = nullptr;
	unsigned int shaderId = glCreateShader(type);
	if (!FileToBuffer(sourcePath, &sourceBuffer))
	{
		glDeleteShader(shaderId);
		return 0;
	}
	glShaderSource(shaderId, 1, &sourceBuffer, NULL);
	free(sourceBuffer);
	sourceBuffer = nullptr;
	glCompileShader(shaderId);
	int shaderErr = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		if (length == 0)
			return 0;
		char* errMessage = (char*)malloc(length);
		glGetShaderInfoLog(shaderId, length, NULL, errMessage);
		LOG("%s\n", errMessage);
		free(errMessage);
		return 0;
	}
	return shaderId;
}

//TODO: on poso aquesta funcio
//Varios paths input
//funcio ha de poder deduir de qiun tipus de shader es tracta (vertex/geometry/fragment)
//construir el programa apartir de tots els shaders de input
static unsigned int CreateProgram(const char* vShaderPath, const char* fShaderPath)
{
	unsigned int vShaderId = CompileShader(vShaderPath, GL_VERTEX_SHADER);
	if (!vShaderId)
		return 0;
	unsigned int fShaderId = CompileShader(fShaderPath, GL_FRAGMENT_SHADER);
	if (!fShaderId)
	{
		glDeleteShader(vShaderId);
		return 0;
	}
	unsigned int programId = glCreateProgram();
	glAttachShader(programId, vShaderId);
	glAttachShader(programId, fShaderId);
	glLinkProgram(programId);
	int shaderErr = 0;
	glGetProgramiv(programId, GL_LINK_STATUS, &shaderErr);
	if (shaderErr == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
		if (length == 0)
			return 0;
		char* errMessage = (char*)malloc(length);
		glGetProgramInfoLog(programId, length, NULL, errMessage);
		LOG("%s", errMessage);
		free(errMessage);
		return 0;
	}
	glDetachShader(programId, vShaderId);
	glDetachShader(programId, fShaderId);
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);
	return programId;
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

#ifdef _DEBUG
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLErrorFunction, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, true);
#endif // _DEBUG

	programId = CreateProgram("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	if (programId == 0)
		return false;

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

	unsigned int indices[6] = {0,1,2,3,2,1};
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
	glBindVertexArray(VAO);
	glUseProgram(programId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	return UPDATE_CONTINUE;
}

update_status ModuleOpenGL::PostUpdate()
{
	
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

