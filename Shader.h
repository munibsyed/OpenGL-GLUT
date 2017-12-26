#pragma once
#include <fstream>
#include <string>
#include "Mesh.h"
#include <iostream>

//may come in useful, if I make a bunch of derived shaders and need to make a factory class
enum class GenericShader
{
	GENERIC_SCREEN_SPACE_SHADER,
	GENERIC_MVP_SHADER
};

/* idea, perhaps I could create some kind of giant 'render state' class?
   It would contain pointers to everything that could be useful information to shader, e.g.

   Camera* m_mainCamera = nullptr;
   Camera** m_allCameras = nullptr;
   Light* m_allLights = nullptr;

   All these variables would be null until the relevant object is created. That means that when a generic shader is created,
   it could query this render state object for all the uniforms that it must initialize itself with

*/
class Shader
{
public:
	Shader(const char* vs, const char* fs, bool fromFile = false);
	Shader(const Shader &other);
	~Shader();

	virtual void Update();

	unsigned int GetShaderID();

	virtual void UseShader();

protected:
	//hide from user
	Shader();

	void CompileAndAttachShaders(const char* vsSource, const char* fsSource);

	unsigned int m_shaderID;
	const char* m_vsSource;
	const char* m_fsSource;
};

