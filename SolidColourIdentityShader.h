#pragma once
#include "Shader.h"
#include <glm.hpp>
#include <sstream>

class SolidColourIdentityShader : public Shader
{
public:
	SolidColourIdentityShader(glm::vec4 colour);
	SolidColourIdentityShader(float r, float g, float b, float a);
	~SolidColourIdentityShader();
};

