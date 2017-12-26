#include "MyObjLoader.h"

static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

MyObjLoader::MyObjLoader()
{
}


MyObjLoader::~MyObjLoader()
{
}

bool MyObjLoader::LoadObj(const char * path, std::vector<vec3> & outVerts, std::vector<vec2> & outUVs, std::vector<vec3> & outNormals, std::vector<OBJIndex> & outObjIndices)
{
	std::ifstream file;
	file.open(path);

	while (!file.eof())
	{
		std::string line;
		if (file.is_open())
		{
			while (file.good())
			{
				getline(file, line);	

				unsigned int lineLength = line.length();

				if (lineLength < 2)
					continue;

				const char* lineCStr = line.c_str();

				switch (lineCStr[0])
				{
				case 'v':
					if (lineCStr[1] == 't')
						this->uvs.push_back(ParseOBJVec2(line));
					else if (lineCStr[1] == 'n')
						this->normals.push_back(ParseOBJVec3(line));
					else if (lineCStr[1] == ' ' || lineCStr[1] == '\t')
						this->vertices.push_back(ParseOBJVec3(line));
					break;
				case 'f':
					CreateOBJFace(line);
					break;
				default: break;
				};
			}
		}
		else
		{
			std::cerr << "Unable to load mesh: " << path << std::endl;
			return false;
		}
		outVerts = vertices;
		outUVs = uvs;
		outNormals = normals;
		outObjIndices = OBJIndices;

		return true;
	}

}

bool MyObjLoader::LoadObj(const char * path, std::vector<vec3>& outVerts, std::vector<vec2>& outUVs, std::vector<vec3>& outNormals, 
						std::vector<unsigned int> & vertexIndices, std::vector<unsigned int> & uvIndices, std::vector<unsigned int> & normalIndices)
{
	//in case there's something already in these
	outVerts.clear();
	outUVs.clear();
	outNormals.clear();
	vertexIndices.clear();
	uvIndices.clear();
	normalIndices.clear();

	//std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
	std::vector<vec3> tempVertices;
	std::vector<vec2> tempUVs;
	std::vector<vec3> tempNormals;

	FILE* file = fopen(path, "r");
	if (file == NULL)
	{
		printf("Can't open the file!\n");
		return false;
	}
	int linesProcessed = 0;
	while (true)
	{
		char lineHeader[128];
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
			break;
		
		//parse line
		if (strcmp(lineHeader, "v") == 0)
		{
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			tempVertices.push_back(vertex);
		}
		else if (strcmp(lineHeader, "vt") == 0)
		{
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			tempUVs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0)
		{
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			tempNormals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0)
		{
			//f 8/11/7 7/12/7 6/10/7 (8th vertex, 11th texcoord, 7th normal)
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1],
				&vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			if (matches != 9)
			{
				//try looking for v//vn v//vn v//vn
					matches = fscanf(file, "%d//%d %d//%d %d//%d", &vertexIndex[0], &normalIndex[0],
					&vertexIndex[1], &normalIndex[1], &vertexIndex[2], &normalIndex[2]);
				if (matches != 6)
				{
					printf("Face information cannot be read by this parser\n");
					return false;

				}
			}

			for (int i = 0; i < 3; i++)
			{
				vertexIndices.push_back(vertexIndex[i]);
				uvIndices.push_back(uvIndex[i]);
				normalIndices.push_back(normalIndex[i]);
			}

		}

		linesProcessed++;
	
	}

	//indices must be turned into raw data
	for (int i = 0; i < vertexIndices.size(); i++)
	{
		vec3 currentVertex = tempVertices[vertexIndices[i] - 1];
		outVerts.push_back(currentVertex);
		vertexIndices[i] = i;
	}

	for (int i = 0; i < uvIndices.size(); i++)
	{
		vec2 currentUV = tempUVs[uvIndices[i] - 1];
		outUVs.push_back(currentUV);
	}

	for (int i = 0; i < normalIndices.size(); i++)
	{
		vec3 currentNormal = tempNormals[normalIndices[i] - 1];
		outNormals.push_back(currentNormal);
	}
	return true;
}

int MyObjLoader::LoadObj(const char * path, std::vector<vec3> & outVerts, std::vector<unsigned int> & outIndices)
{
	//std::vector<std::string*> lines;
	//std::vector<vec3> vertices;
	//std::vector<vec3> normals;
	//std::vector<Face> faces;

	//std::vector<unsigned int> indices;

	//std::ifstream in(path);

	//if (in.is_open() == false)
	//{
	//	std::cout << "Could not open file!" << std::endl;
	//	return -1;
	//}

	//char buf[256];
	//while (in.eof() == false)
	//{
	//	char c[256];
	//	in.getline(c, 256);
	//	lines.push_back(new std::string(c));
	//}

	//for (int i = 0; i < lines.size(); i++)
	//{
	//	if ((*lines[i])[0] == '#') //ignore # because they are comments
	//	{
	//		continue;
	//	}
	//	else if ((*lines[i])[0] == 'v' && (*lines[i])[1] == ' ')
	//	{
	//		//must be vertex
	//		float x, y, z;
	//		sscanf(lines[i]->c_str(), "v %f %f %f", &x, &y, &z);
	//		vertices.push_back(vec3(x, y, z));
	//	}
	//	else if ((*lines[i])[0] == 'v' && (*lines[i])[1] == 'n')
	//	{
	//		//mut be vertex normal
	//		float x, y, z;
	//		sscanf(lines[i]->c_str(), "vn %f %f %f", &x, &y, &z);
	//		normals.push_back(vec3(x, y, z));
	//	}
	//	else if ((*lines[i])[0] == 'f')
	//	{
	//		//indices
	//		int v1, v2, v3, v4;
	//		int vt1, vt2, vt3;
	//		int vn1, vn2, vn3, vn4;
	//		if (std::count(lines[i]->begin(), lines[i]->end(), ' ') == 3)
	//		{
	//			if (std::count(lines[i]->begin(), lines[i]->end(), "//") != 0)
	//			{
	//				sscanf(lines[i]->c_str(), "f %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3);

	//				vertices.push_back(vec3(v1, v2, v3));
	//				normals.push_back(vec3(vn1, vn2, vn3));
	//			}

	//			else if (std::count(lines[i]->begin(), lines[i]->end(), '/') != 0)
	//			{
	//				sscanf(lines[i]->c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1,
	//					&v2, &vt2, &vn2, &v3, &vt3, &vn3);
	//				
	//				vertices.push_back(vec3(v1, v2, v3));
	//				normals.push_back(vec3(vn1, vn2, vn3));
	//			}
	//		}

	//		else if (std::count(lines[i]->begin(), lines[i]->end(), ' ') == 4) //then this is a four sided face
	//		{
	//			if (std::count(lines[i]->begin(), lines[i]->end(), "//") != 0)
	//			{
	//				sscanf(lines[i]->c_str(), "f %d//%d %d//%d %d//%d %d//%d", &v1, &vn1, &v2, &vn2, &v3, &vn3, &v4, &vn4);
	//				
	//				//0,1,2 tri
	//				//0,2,3 tri
	//				indices.push_back(v1);
	//				indices.push_back(v2);
	//				indices.push_back(v3);

	//				indices.push_back(v1);
	//				indices.push_back(v3);
	//				indices.push_back(v4);


	//				normals.push_back(vec3(vn1, vn2, vn3));
	//				normals.push_back(vec3(vn1, vn3, vn4));

	//			}

	//		}
	//	}
	//	
	//	delete lines[i];
	//}	

	//for (int i = 0; i < indices.size(); i++)
	//{
	//	vec3 currentVertex = vertices[indices[i] - 1];
	//	outVerts.push_back(currentVertex);
	//}

	return 0;
}

unsigned int MyObjLoader::FindLastVertexIndex(const std::vector<OBJIndex*>& indexLookup, const OBJIndex * currentIndex, const IndexedModel & result)
{
	unsigned int start = 0;
	unsigned int end = indexLookup.size();
	unsigned int current = (end - start) / 2 + start;
	unsigned int previous = start;

	while (current != previous)
	{
		OBJIndex* testIndex = indexLookup[current];

		if (testIndex->vertexIndex == currentIndex->vertexIndex)
		{
			unsigned int countStart = current;

			for (unsigned int i = 0; i < current; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current - i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;

				countStart--;
			}

			for (unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
			{
				OBJIndex* possibleIndex = indexLookup[current + i];

				if (possibleIndex == currentIndex)
					continue;

				if (possibleIndex->vertexIndex != currentIndex->vertexIndex)
					break;
				else if ((!hasUVs || possibleIndex->uvIndex == currentIndex->uvIndex)
					&& (!hasNormals || possibleIndex->normalIndex == currentIndex->normalIndex))
				{
					glm::vec3 currentPosition = vertices[currentIndex->vertexIndex];
					glm::vec2 currentTexCoord;
					glm::vec3 currentNormal;

					if (hasUVs)
						currentTexCoord = uvs[currentIndex->uvIndex];
					else
						currentTexCoord = glm::vec2(0, 0);

					if (hasNormals)
						currentNormal = normals[currentIndex->normalIndex];
					else
						currentNormal = glm::vec3(0, 0, 0);

					for (unsigned int j = 0; j < result.positions.size(); j++)
					{
						if (currentPosition == result.positions[j]
							&& ((!hasUVs || currentTexCoord == result.texCoords[j])
								&& (!hasNormals || currentNormal == result.normals[j])))
						{
							return j;
						}
					}
				}
			}

			return -1;
		}
		else
		{
			if (testIndex->vertexIndex < currentIndex->vertexIndex)
				start = current;
			else
				end = current;
		}

		previous = current;
		current = (end - start) / 2 + start;
	}

	return -1;
}

void MyObjLoader::CreateOBJFace(const std::string & line)
{
	std::vector<std::string> tokens = SplitString(line, ' ');

	this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[2], &this->hasUVs, &this->hasNormals));
	this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));

	if ((int)tokens.size() > 4)
	{
		this->OBJIndices.push_back(ParseOBJIndex(tokens[1], &this->hasUVs, &this->hasNormals));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[3], &this->hasUVs, &this->hasNormals));
		this->OBJIndices.push_back(ParseOBJIndex(tokens[4], &this->hasUVs, &this->hasNormals));
	}
}

vec2 MyObjLoader::ParseOBJVec2(const std::string & line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 3;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec2(x, y);
}

vec3 MyObjLoader::ParseOBJVec3(const std::string & line)
{
	unsigned int tokenLength = line.length();
	const char* tokenString = line.c_str();

	unsigned int vertIndexStart = 2;

	while (vertIndexStart < tokenLength)
	{
		if (tokenString[vertIndexStart] != ' ')
			break;
		vertIndexStart++;
	}

	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');

	float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);

	return glm::vec3(x, y, z);
}

OBJIndex MyObjLoader::ParseOBJIndex(const std::string & token, bool * hasUVs, bool * hasNormals)
{
	unsigned int tokenLength = token.length();
	const char* tokenString = token.c_str();

	unsigned int vertIndexStart = 0;
	unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	OBJIndex result;
	result.vertexIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	result.uvIndex = 0;
	result.normalIndex = 0;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.uvIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasUVs = true;

	if (vertIndexEnd >= tokenLength)
		return result;

	vertIndexStart = vertIndexEnd + 1;
	vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');

	result.normalIndex = ParseOBJIndexValue(token, vertIndexStart, vertIndexEnd);
	*hasNormals = true;

	return result;
}

void IndexedModel::CalcNormals()
{
	for (unsigned int i = 0; i < indices.size(); i += 3)
	{
		int i0 = indices[i];
		int i1 = indices[i + 1];
		int i2 = indices[i + 2];

		glm::vec3 v1 = positions[i1] - positions[i0];
		glm::vec3 v2 = positions[i2] - positions[i0];
			
		glm::vec3 normal = glm::normalize(glm::cross(v1, v2));

		normals[i0] += normal;
		normals[i1] += normal;
		normals[i2] += normal;
	}

	for (unsigned int i = 0; i < positions.size(); i++)
		normals[i] = glm::normalize(normals[i]);
}


static bool CompareOBJIndexPtr(const OBJIndex* a, const OBJIndex* b)
{
	return a->vertexIndex < b->vertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
	unsigned int result = start;
	while (result < length)
	{
		result++;
		if (str[result] == token)
			break;
	}

	return result;
}

static inline unsigned int ParseOBJIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
	return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
	std::vector<std::string> elems;

	const char* cstr = s.c_str();
	unsigned int strLength = s.length();
	unsigned int start = 0;
	unsigned int end = 0;

	while (end <= strLength)
	{
		while (end <= strLength)
		{
			if (cstr[end] == delim)
				break;
			end++;
		}

		elems.push_back(s.substr(start, end - start));
		start = end + 1;
		end = start;
	}

	return elems;
}