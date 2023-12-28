#version 460 core

in vec2 uv;
in vec3 normal;
in vec4 tangent;
out vec4 fragColor;

layout(location = 3) uniform sampler2D theTexture;

void main(){
	fragColor = texture2D(theTexture, uv) * mix(vec4(normal,1),tangent,0.5);
}