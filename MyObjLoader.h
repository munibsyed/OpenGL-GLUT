#pragma once
#include <glm.hpp>
#include <ext.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

//OBJ loading code taken from https://github.com/BennyQBD/ModernOpenGLTutorial

using glm::vec3;
using glm::vec2; 

struct Face
{
	Face(int faceNum, int f1, int f2, int f3)
	{
		m_faceNum = faceNum;
		m_faces[0] = f1;
		m_faces[1] = f2;
		m_faces[2] = f3;
		m_isTri = true;
	}
	
	Face(int faceNum, int f1, int f2, int f3, int f4)
	{
		m_faceNum = faceNum;
		m_faces[0] = f1;
		m_faces[1] = f2;
		m_faces[2] = f3;
		m_faces[3] = f4;
		m_isTri = false;
	}

	int m_faceNum;
	bool m_isTri;
	int m_faces[4];
};

struct OBJIndex
{
	unsigned int vertexIndex;
	unsigned int uvIndex;
	unsigned int normalIndex;

	bool operator<(const OBJIndex& r) const { return vertexIndex < r.vertexIndex; }
};

class IndexedModel
{
public:
	std::vector<vec3> positions;
	std::vector<vec2> texCoords;
	std::vector<vec3> normals;
	std::vector<unsigned int> indices;

	void CalcNormals();
};

class MyObjLoader
{
public:
	MyObjLoader();
	~MyObjLoader();

	bool LoadObj(const char* path);

	bool LoadObj(const char* path, std::vector<vec3> & outVerts, std::vector<vec2> & outUVs, std::vector<vec3> & outNormals, std::vector<OBJIndex> & outObjIndices);

	bool LoadObj(const char* path, std::vector<vec3> & outVerts, std::vector<vec2> & outUVs, std::vector<vec3> & outNormals,
		std::vector<unsigned int> & vertexIndices, std::vector<unsigned int> & uvIndices, std::vector<unsigned int> & normalIndices);

	int LoadObj(const char* path, std::vector<vec3> & outVerts, std::vector<unsigned int> & outIndices);

	IndexedModel ToIndexedModel();

private:
	unsigned int FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex* currentIndex, const IndexedModel& result);
	void CreateOBJFace(const std::string& line);

	vec2 ParseOBJVec2(const std::string& line);
	vec3 ParseOBJVec3(const std::string& line);
	OBJIndex ParseOBJIndex(const std::string& token, bool* hasUVs, bool* hasNormals);

	std::vector<OBJIndex> OBJIndices;
	std::vector<vec3> vertices;
	std::vector<vec2> uvs;
	std::vector<vec3> normals;
	bool hasUVs;
	bool hasNormals;

	
};