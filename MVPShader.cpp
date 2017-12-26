#include "MVPShader.h"



MVPShader::MVPShader(mat4 *modelViewProjection)
{
	m_vsSource = "#version 410\n layout(location = 0) in vec4 position; layout(location = 1) in vec4 colour; uniform mat4 modelViewProjection; out vec4 vColour; void main() { vec4 pos = position; pos.w = 1.0; vColour = colour; gl_Position = modelViewProjection * pos; }";
	m_fsSource = "#version 410\n in vec4 vColour; out vec4 fragColor; void main() { fragColor = vec4(1,0,0,1); }";
	m_modelViewProjection = modelViewProjection;
	
	CompileAndAttachShaders(m_vsSource, m_fsSource);
}

MVPShader::~MVPShader()
{
}

void MVPShader::Update()
{
	unsigned int mvpLocation = glGetUniformLocation(m_shaderID, "modelViewProjection");
	glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(*m_modelViewProjection));

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("%s\n", err);
	}
}