#pragma once
#include <glm.hpp>
#include <ext.hpp>
#include <vector>
#include <GL\glew.h>
#include <GL\glut.h>
#include "MyObjLoader.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;

struct Vertex
{
	vec4 position;
	vec4 normal;
	vec4 colour;
};

struct OpenGLInfo
{
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_faceCount;

};
struct OBJVertex
{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
};

class Mesh
{
public:
	Mesh(const char* filename, unsigned int shaderID, int drawMode);

	Mesh(const std::vector<vec3> &vertices, const std::vector<vec3> &normals, const std::vector<vec2> &uvs, const std::vector<OBJIndex> &indices, unsigned int shaderID);

	//suitable for my obj loader
	Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &vertexIndices, unsigned int shaderID);
	Mesh(const std::vector<vec3> &vertices, const std::vector<unsigned int> &vertexIndices, unsigned int shaderID);

	~Mesh();
	
	void InitMesh(const IndexedModel& model);

	void CreateBuffers(int numAttribs, int* numFloats, int numVerts, int numIndices, Vertex *vertices, unsigned int* indices);

	void Update(float deltaTime);

	void Draw(mat4 view, mat4 projection);

	void SetPosition(float x, float y, float z);

	vec3 GetPosition();
	
	void Rotate(float xDeg, float yDeg, float zDeg);


private:
	enum VBO_Descriptors
	{
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB
	};

	enum DRAW_MODE
	{
		FILL,
		WIREFRAME
	};

	int m_drawMode;

	const static int NUM_BUFFERS = 4;
	std::vector<Vertex> m_vertices;
	std::vector<OBJIndex> m_objIndices;
	std::vector<unsigned int> m_vertexIndices;
	std::vector<OpenGLInfo> m_glInfo;

	unsigned int m_VBO, m_VAO, m_IBO;
	unsigned int m_VBOs[NUM_BUFFERS];
	unsigned int m_numIndices;

	unsigned int m_shaderID;

	mat4 m_modelTransform;
};

