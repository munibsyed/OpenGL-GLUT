#version 410
#define MAX_LIGHTS 2

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 lightDir[MAX_LIGHTS];
uniform vec3 specDir[MAX_LIGHTS];
uniform vec3 specPos[MAX_LIGHTS];
uniform float specPow;

uniform vec3 spotLightDir[MAX_LIGHTS];
uniform vec3 spotLightPos[MAX_LIGHTS];
uniform float spotLightAngles[MAX_LIGHTS];

uniform vec3 cameraPos;

uniform vec3 globalLightColour;

out vec4 vColour;
out vec3 vNormal;
out vec3 vLightDir[MAX_LIGHTS];
out vec3 vSpecDir[MAX_LIGHTS];
out vec3 vSpecPos[MAX_LIGHTS];
out float vSpecPow;

out vec3 vSpotLightDir[MAX_LIGHTS];
out vec3 vSpotLightPos[MAX_LIGHTS];
out float vSpotLightAngles[MAX_LIGHTS];

out vec3 vCameraPos;
out vec3 vPosition;
out vec3 vGlobalLightColour;

void main()
{ 
	vPosition = (model * vec4(position,1)).xyz;
	vColour = vec4(normal, 1);	
	vNormal = (model * vec4(normal,1)).xyz;

	vSpecPow = specPow;
	vSpecPos = specPos;
 	vSpecDir = specDir;
	vLightDir = lightDir;

	vSpotLightPos = spotLightPos;
	vSpotLightDir = spotLightDir;
	vSpotLightAngles = spotLightAngles;
	
	vGlobalLightColour = globalLightColour;
	vCameraPos = cameraPos;
	
	gl_Position = projection * view * model * vec4(position,1);
}