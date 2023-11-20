#version 460 core
layout(location = 0) in vec3 pos;

out vec3 col;

uniform mat4 model;
uniform mat4 view;
unisform mat4 projection;

void main(){
	gl_Position = vec4(pos, 1.0f);
}