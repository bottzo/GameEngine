#include "Application.h"
#include "ModuleRenderMandelbrot.h"
#include "ModuleWindow.h"
#include "GL/glew.h"
#include "imgui.h"
#include "Files.h"

ModuleRenderMandelbrot::ModuleRenderMandelbrot() 
{
}

ModuleRenderMandelbrot::~ModuleRenderMandelbrot() 
{
}

bool ModuleRenderMandelbrot::Init()
{
	//programId = CreateProgram("Shaders/Vertex.glsl", "Shaders/Fragment.glsl");
	programId = CreateProgram("Shaders/MandelbrotVertex.glsl", "Shaders/MandelbrotFragment.glsl");

	if (programId == 0)
		return false;
	glUseProgram(programId);
	glUniform1ui(0, unis.maxIterations);
	glUniform1ui(1, unis.colorPeriod);
	int w, h;
	App->window->GetWindowSize(&w, &h);
	glUniform2i(2, w, h);
	glUniform1d(3, cLength);
	glUniform2d(4, centerX, centerY);
	glUniform3f(5, colors[0], colors[1], colors[2]);
	glUniform3f(6, colors[3], colors[4], colors[5]);


	float vertex[] = {
	-1.0f, -1.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,
	-1.0f,  1.0f, 0.0f,
	 1.0f, 1.0f, 0.0f
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, VBOEBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBOEBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	unsigned int indices[6] = { 0,1,2,3,2,1 };
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBOEBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	return true;
}

update_status ModuleRenderMandelbrot::Update()
{
	static bool window = true;
	ImGui::Begin("Mandelbrot ui window", &window);
	if (ImGui::InputDouble("CLength", &cLength, 0.1))
	{
		glUseProgram(programId);
		glUniform1d(3, cLength);
		if (cLength > prevCLength)
			cLength = (cLength - 0.1) * 0.99;
		else
			cLength = (cLength + 0.1) * 1.01;
		prevCLength = cLength;
	}
	if (ImGui::InputDouble("CenterX", &centerX, stepCenter))
	{
		glUseProgram(programId);
		glUniform2d(4, centerX, centerY);
		stepCenter = 0.01 * cLength;
	}
	if (ImGui::InputDouble("CenterY", &centerY, stepCenter))
	{
		glUseProgram(programId);
		glUniform2d(4, centerX, centerY);
		stepCenter = 0.01 * cLength;
	}
	if (ImGui::DragInt("MaxIterations", (int*)&unis.maxIterations))
	{
		glUseProgram(programId);
		glUniform1ui(0, unis.maxIterations);
	}
	if (ImGui::DragInt("ColorPeriod", (int*)&unis.colorPeriod))
	{
		glUseProgram(programId);
		glUniform1ui(1, unis.colorPeriod);
	}
	if (ImGui::ColorPicker3("Color 1", colors))
	{
		glUseProgram(programId);
		glUniform3f(5, colors[0], colors[1], colors[2]);
	}
	if (ImGui::ColorPicker3("Color 2", &colors[3]))
	{
		glUseProgram(programId);
		glUniform3f(6, colors[3], colors[4], colors[5]);
	}
	//App->GetWindow()->GetWindowSize(&w, &h);
	//glUniform2i(2, w, h);
	ImGui::End();
	glBindVertexArray(VAO);
	glUseProgram(programId);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glUseProgram(0);
	glBindVertexArray(0);
	return UPDATE_CONTINUE;
}

bool ModuleRenderMandelbrot::CleanUp()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, VBOEBO);
	return true;
}

void ModuleRenderMandelbrot::WindowUniform(int w, int h)
{
	glUseProgram(programId);
	glUniform2i(2, w, h);
}