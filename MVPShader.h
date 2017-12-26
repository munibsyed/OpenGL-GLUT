#pragma once
#include "Shader.h"
#include <glm.hpp>
#include <ext.hpp>

using glm::mat4;

class MVPShader :
	public Shader
{
public:
	MVPShader(mat4 *modelViewProjection);
	~MVPShader();

	void Update();
private:
	mat4 *m_modelViewProjection;
};

