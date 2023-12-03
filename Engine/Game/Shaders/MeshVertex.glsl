#version 460 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tCoord;
layout(location = 2) in vec3 norm;

layout(location = 0) uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 projection;

out vec2 uv;
out vec3 normals;

void main(){
	uv = tCoord;
	normals = norm;
	gl_Position = projection*view*model*vec4(pos, 1.0f);
}