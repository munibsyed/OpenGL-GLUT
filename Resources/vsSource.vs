#version 410
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightDir[4];
uniform vec3 globalLightColour;

out vec4 vColour;
out vec3 vNormal;
out vec3 vLightDir[4];
out vec3 vGlobalLightColour;

void main()
{ 
	vColour = vec4(normal, 1);
	vNormal = normal;
	vLightDir = lightDir;
	vGlobalLightColour = globalLightColour;
	gl_Position = projection * view * model * vec4(position,1);
}