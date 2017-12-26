#version 410
layout(location = 0) in vec4 position;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec4 vColour;
void main()
{ 
	vColour = normalize(position);
	gl_Position = projection * view * model * position;
}