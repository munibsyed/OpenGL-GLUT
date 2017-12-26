#include "IdentityShader.h"



IdentityShader::IdentityShader()
{
	m_vsSource = "#version 410\n layout(location = 0) in vec4 position; layout(location = 1) in vec4 colour; out vec4 vColour; void main() { vColour = colour; gl_Position = position; }";
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vColour; }";
	CompileAndAttachShaders(m_vsSource, m_fsSource);
}


IdentityShader::~IdentityShader()
{
}