#version 460 core

layout(location = 0) in vec3 inPos;
layout(location = 1) in vec2 inUv;
layout(location = 2) in vec3 inNorm;
layout(location = 3) in vec4 inTang;

layout(location = 0)uniform mat4 model;
layout(location = 1) uniform mat4 view;
layout(location = 2) uniform mat4 proj;

//layout(std140, binding = 0) uniform CameraMatrices{
//	mat4 view;
//	mat4 proj;
//};

layout (location = 4)uniform vec3 lDir;
layout (location = 7)uniform vec3 cPos;

//out vec3 oNorm;
out vec2 oUv;
out vec3 lightDir;
out vec3 viewDir;

void main()
{
	vec3 N = normalize(vec3(model * vec4(inNorm, 0.0)));
	vec3 T = normalize(vec3(model * vec4(inTang.xyz, 0.0))); 
	vec3 B = inTang.w * cross(N, T);
	mat3 TBNInv = transpose(mat3(T,B,N));
	lightDir = TBNInv * lDir;

	oUv = inUv;
	//oNorm = transpose(inverse(mat3(model))) * inNorm;
	vec3 surfacePos = (model*vec4(inPos,1)).xyz * TBNInv;
	viewDir = TBNInv * (surfacePos - cPos);
	gl_Position = proj * view * model * vec4(inPos,1);
}