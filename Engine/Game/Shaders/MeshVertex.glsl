#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 2) in vec2 inUv;
layout(location = 1) in vec3 inNorm;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 oNorm;
out vec2 oUv;
out vec3 surfacePos;

void main()
{
	oUv = inUv;
	oNorm = transpose(inverse(mat3(model)))* inNorm;
	surfacePos = (model*vec4(inPos,1)).xyz;
	gl_Position = projection * view * model * vec4(inPos,1);
}