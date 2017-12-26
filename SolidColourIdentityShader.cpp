#include "SolidColourIdentityShader.h"



SolidColourIdentityShader::SolidColourIdentityShader(glm::vec4 colour)
{
	
	const char* toFormat = "#version 410\n layout(location = 0) in vec4 position; out vec4 vColour; void main() { vColour = %s; gl_Position = position; }";
	std::stringstream colourSS;

	colourSS << "vec4(" << colour.x << ", " << colour.y << ", " << colour.z << ", " << colour.w << ")";

	char* vsSourceTemp = new char[strlen(toFormat) + colourSS.str().length()];
	sprintf(vsSourceTemp, toFormat, colourSS.str().c_str());
	m_vsSource = vsSourceTemp;
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vColour; }";
	CompileAndAttachShaders(m_vsSource, m_fsSource);

	delete[] vsSourceTemp;
}

SolidColourIdentityShader::SolidColourIdentityShader(float r, float g, float b, float a)
{
	const char* toFormat = "#version 410\n layout(location = 0) in vec4 position; out vec4 vColour; void main() { vColour = vec4(%.1f, %.1f, %.1f, %.1f); gl_Position = position; }";
	char* vsSourceTemp = new char[strlen(toFormat)]; //deal with lengthier float inputs

	sprintf(vsSourceTemp, toFormat, r, g, b, a);
	m_vsSource = vsSourceTemp;
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vColour; }";

	CompileAndAttachShaders(m_vsSource, m_fsSource);

	delete[] vsSourceTemp;

}


SolidColourIdentityShader::~SolidColourIdentityShader()
{
}
