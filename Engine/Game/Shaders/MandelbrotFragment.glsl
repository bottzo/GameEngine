#version 460 core

//https://www.khronos.org/opengl/wiki/Early_Fragment_Test
//layout(early_fragment_tests) in;

layout(location = 0) uniform uint maxIterations;
layout(location = 1) uniform uint colorPeriod;
layout(location = 2) uniform ivec2 resolution;
layout(location = 3) uniform double cLength;
layout(location = 4) uniform dvec2 center;

layout(location = 5) uniform vec3 col1;
layout(location = 6) uniform vec3 col2;

out vec4 fragColor;
in vec4 gl_FragCoord;

void main()
{
	const dvec2 inc = cLength / dvec2(resolution);
	dvec2 C = (center - cLength * 0.5lf) + (gl_FragCoord.xy * inc); 
	dvec2 z = dvec2(0,0);
	
	double rSquare = 0;
	uint it = 0;
	//Z^2 + C
	while(rSquare < 4 && it < maxIterations) 
	{
		const double zReal = (z.x*z.x - z.y*z.y) + C.x;
		const double zIm = (2*z.x*z.y) + C.y;
		rSquare = zReal * zReal + zIm * zIm;
		z = dvec2(zReal, zIm);
		++it;
	}
	fragColor = vec4(mix(col1, col2, float(it % colorPeriod)/float(colorPeriod)) * vec3(it < maxIterations), 1.0);
}