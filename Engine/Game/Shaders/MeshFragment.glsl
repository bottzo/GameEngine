#version 460 core

in vec3 oNorm;
in vec2 oUv;
in vec3 surfacePos;

layout (location = 3) uniform sampler2D theSampler;

layout (location = 4)uniform vec3 lightDir;
layout (location = 5)uniform vec3 lightCol;
layout (location = 6)uniform vec3 ambientCol;
layout (location = 7)uniform vec3 cameraPos;
layout (location = 8)uniform float kD;

out vec4 fragCol;

void main()
{
	vec3 N = normalize(oNorm);
	vec3 L = -normalize(lightDir);
	vec3 D = texture(theSampler, oUv).xyz;
	
	float diffuse = max(dot(N,L),0.0);
	vec3 V = normalize(surfacePos - cameraPos);
	vec3 R = normalize(reflect(L, N));
	float specular = pow(max(dot(R,V), 0.0),20);
	fragCol = vec4(ambientCol * D + kD * diffuse * D * lightCol + specular * lightCol * (1 - kD), 1);
}