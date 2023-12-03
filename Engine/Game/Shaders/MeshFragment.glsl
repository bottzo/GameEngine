#version 460 core

in vec2 uv;
in vec3 normals;
out vec4 fragColor;

layout(location = 3) uniform sampler2D theTexture;

void main(){
	fragColor = texture2D(theTexture, uv) * vec4(normals,1.0);
}