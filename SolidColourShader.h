#pragma once
#include "Shader.h"
#include <sstream>

class SolidColourShader :
	public Shader
{
public:
	SolidColourShader(vec4 colour);
	SolidColourShader(float r, float g, float b, float a);
	~SolidColourShader();
};

