#include "SolidColourShader.h"


SolidColourShader::SolidColourShader(vec4 colour)
{
	const char* toFormat = "#version 410\n layout(location = 0) in vec3 position; uniform mat4 model; uniform mat4 view; uniform mat4 projection; out vec4 vColour; void main() { vColour = %s; gl_Position = projection * view * model * vec4(position,1); }";
	std::stringstream colourSS;

	colourSS << "vec4(" << colour.x << ", " << colour.y << ", " << colour.z << ", " << colour.w << ")";

	char* vsSourceTemp = new char[strlen(toFormat) + colourSS.str().length()];
	sprintf(vsSourceTemp, toFormat, colourSS.str().c_str());
	m_vsSource = vsSourceTemp;
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vColour; }";
	CompileAndAttachShaders(m_vsSource, m_fsSource);

	delete[] vsSourceTemp;
}

SolidColourShader::SolidColourShader(float r, float g, float b, float a)
{
	const char* toFormat = "#version 410\n layout(location = 0) in vec3 position; uniform mat4 model; uniform mat4 view; uniform mat4 projection; out vec4 vColour; void main() { vColour = %s; gl_Position = projection * view * model * vec4(position,1); }";
	std::stringstream colourSS;

	colourSS << "vec4(" << r << ", " << g << ", " << b << ", " << a << ")";

	char* vsSourceTemp = new char[strlen(toFormat) + colourSS.str().length()];
	sprintf(vsSourceTemp, toFormat, colourSS.str().c_str());
	m_vsSource = vsSourceTemp;
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vColour; }";
	CompileAndAttachShaders(m_vsSource, m_fsSource);

	delete[] vsSourceTemp;
}

SolidColourShader::~SolidColourShader()
{
}
