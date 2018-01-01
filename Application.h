#pragma once
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <iomanip>
#include <thread>
#include <glm.hpp>
#include <unordered_set>
#include "LightManager.h"
#include "Shader.h"
#include "SolidColourShader.h"
#include "FlyCamera.h"
#include "MyObjLoader.h"
#include "OpenFileDialog.h"
#include <freeglut.h>

using glm::vec3;
using glm::vec4;

class Application
{
public:
	Application();
	virtual ~Application();

	void Shutdown();

	void Init();

	//initialize and run glut loop
	void Start(int argc, char *argv[]);

	virtual void Update(float deltaTime);

	virtual void Draw(float deltaTime);

	virtual void Display();

	virtual void MouseButtonPressed(int button, int state, int x, int y);	

	virtual void Reshape(int width, int height);

	void ConstructMesh(const char* filename);
	
	void CreateBuffers(int numAttribs, int* numFloats, int numVerts, int numIndices, Vertex *vertices, unsigned int* indices);

	void Run();

	int GetTime();

	void ClearScreen();
	
	void SetBackgroundColour(float r, float g, float b, float a = 1.0f);

	void Quit();

	void SetInstance();

private:
	const static int NUM_LIGHTS = 4;

	LightManager m_lightManager;

	int m_inputModifer;
	std::unordered_set<char> m_keysHeldDown;
	std::unordered_set<char> m_keysThatMustGoUp;
	//Frame counters

	bool m_isRunning;
	unsigned int m_FPS;	
	float m_deltaTime; //in milliseconds
	int m_frameTimeElapsed;
	int m_prevTime;
	int m_currTime;
	int m_windowID;
	bool m_canRender; //set from Run, used in Display
	/////

	//Vertex and Index Buffers
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	/////

	//Shaders
	Shader *m_shader;
	/////

	//Cameras
	FlyCamera *m_camera;
	vec3 m_position;

	//OBJ Loaders
	MyObjLoader* m_toyObjLoader;
	std::vector<vec3> meshVertices;
	std::vector<vec2> meshUvs;
	std::vector<vec3> meshNormals;

	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	std::vector<Mesh*> m_meshes;

	//Cursor positions
	float m_oldCursorPosX;
	float m_oldCursorPosY;
	float m_newCursorPosX;
	float m_newCursorPosY;
	float m_deltaCursorX;
	float m_deltaCursorY;
	bool m_hasMouseMoved;

protected:
	static Application* m_instance;

public:

	const static int WINDOW_WIDTH = 640;
	const static int WINDOW_HEIGHT = 480;
	const char* m_title = "Munib's Application";

	//static wrapper functions
	static void DisplayWrapper();
	static void ReshapeWrapper(int width, int height);
	static void RunWrapper();
	static void MouseButtonPressWrapper(int button, int state, int x, int y);
	static void MouseMoveWrapper(int x, int y);
	static void KeyboardDownWrapper(unsigned char key, int x, int y);
	static void KeyboardUpWrapper(unsigned char key, int x, int y);
	static void SpecialKeyboardDownWrapper(int key, int x, int y);
	static void SpecialKeyboardUpWrapper(int key, int x, int y);

};

