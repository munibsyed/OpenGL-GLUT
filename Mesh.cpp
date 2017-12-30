#include "Mesh.h"


Mesh::Mesh(const char * filename)
{
	MyObjLoader objLoader;
	bool result = objLoader.LoadObj(filename);
	if (result)
		InitMesh(objLoader.ToIndexedModel());
}

Mesh::Mesh(const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<vec2>& uvs,
	const std::vector<OBJIndex>& indices, unsigned int shaderID)
{
	m_shaderID = shaderID;
	m_VBO = 0;
	m_VAO = 0;
	m_IBO = 0;
	int numFloats[2] = { 4, 4 };
	m_objIndices = indices; //copying could be slow with larger meshes

	Vertex vert;
	for (int i = 0; i < vertices.size(); i++)
	{
		vert.position = vec4(vertices[i], 1);
		vert.normal = vec4(normals[i], 1);
		m_vertices.push_back(vert);
	}
	//.push_back(vert);

	//for (int i = 0; i < vertices.size(); i++)
	//{
	//	//get all verts with this index
	//	for (int ii = 0; ii < indices.size(); ii++)
	//	{
	//		if (indices[ii].vertexIndex == i)
	//		{
	//			Vertex vert = { vec4(vertices[i], 1), vec4(normals[indices[ii].normalIndex], 1) };
	//			m_vertices.push_back(vert);
	//			if (m_vertices.size() % 3 == 0)
	//				break;
	//		}
	//	}
	//}
	//
	unsigned int* indexArr = new unsigned int[indices.size()];
	for (int i = 0; i < indices.size(); i++)
	{
		indexArr[i] = indices[i].vertexIndex;
	}

	//psuedo-code
	/*
		There are 36 indices because 6 faces * 2 triangles per face * 3 verts per triangle
		There are 24 vertices because 8 vertices * 3 faces
		

		1. Iterate through faces
		2. 

		for (int i = 0; i < indices.size(); i++)
		{
				
		}
	*/

	Vertex* vertexArr = new Vertex[m_vertices.size()];
	vertexArr = &m_vertices[0];
	CreateBuffers(sizeof(numFloats)/sizeof(numFloats[0]), numFloats, m_vertices.size(), indices.size(), vertexArr, indexArr);

	delete[] vertexArr;
	delete[] indexArr;
}

Mesh::Mesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &vertexIndices, unsigned int shaderID)
{
	m_shaderID = shaderID;
	m_vertices = vertices;
	m_vertexIndices = vertexIndices;
	int numFloats[1] = { 4 };

	Vertex* vertexArr = new Vertex[vertices.size()];
	unsigned int* indexArr = new unsigned int[vertexIndices.size()];
	for (int i = 0; i < vertices.size(); i++) //probably not the most efficient process
	{
		vertexArr[i] = vertices[i];
	}

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		indexArr[i] = vertexIndices[i];
	}

	CreateBuffers(1, numFloats, vertices.size(), vertexIndices.size(), vertexArr, indexArr);

	delete[] vertexArr;
	delete[] indexArr;
}

Mesh::Mesh(const std::vector<vec3> &vertices, const std::vector<unsigned int> &vertexIndices, unsigned int shaderID)
{
	m_shaderID = shaderID;
	m_VBO = 0;
	m_VAO = 0;
	m_IBO = 0;

	m_vertices.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex vert;
		vert.position = vec4(vertices[i], 1);
		m_vertices[i] = vert;
	}
	m_vertexIndices = vertexIndices;
	int numFloats[1] = { 4 };

	Vertex* vertexArr = new Vertex[vertices.size()];
	unsigned int* indexArr = new unsigned int[vertexIndices.size()];
	//for (int i = 0; i < m_vertices.size(); i++) //probably not the most efficient process
	//{
	//	vertexArr[i] = m_vertices[i];
	//}

	for (int i = 0; i < vertexIndices.size(); i++)
	{
		indexArr[i] = vertexIndices[i];
	}

	vertexArr = &m_vertices[0];

	CreateBuffers(1, numFloats, vertices.size(), vertexIndices.size(), vertexArr, indexArr);

	
	delete[] vertexArr;
	delete[] indexArr;
}

Mesh::~Mesh()
{
	glDeleteBuffers(NUM_BUFFERS, m_VBOs);
	glDeleteVertexArrays(1, &m_VAO);
}

void Mesh::InitMesh(const IndexedModel & model)
{
	m_numIndices = model.indices.size();
	
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(NUM_BUFFERS, m_VBOs);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[POSITION_VB]);

	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOs[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOs[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Mesh::CreateBuffers(int numAttribs, int * numFloats, int numVerts, int numIndices, Vertex * vertices, unsigned int * indices)
{
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_IBO);

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//bind
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	//provide data
	unsigned int sizeOfVertices = numVerts * sizeof(Vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeOfVertices, vertices, GL_STATIC_DRAW);

	size_t startPoint = 0;
	for (int i = 0; i < numAttribs; i++)
	{
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, numFloats[i], GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)startPoint);
		startPoint += numFloats[i] * sizeof(float); //this assumes all data is floats
	}

	//bind
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//provide data
	unsigned int sizeOfIndices = numIndices * sizeof(unsigned int);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfIndices, indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0); //passing in zero unbinds buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		std::cout << "Error here: " << error << std::endl;
		std::cout << gluErrorString(error) << std::endl;
	}
}

void Mesh::Draw()
{
	unsigned int size = m_glInfo.size();
	if (size == 0)
	{
		//glBindVertexArray(m_VAO);
		//size = m_objIndices.size();
		////glDrawArrays(GL_TRIANGLES, 0, 24);
		//glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		glBindVertexArray(m_VAO);
		glDrawElementsBaseVertex(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, 0, 0);
		glBindVertexArray(0);
	}
	else
	{
		for (int i = 0; i < size; i++)
		{
			glBindVertexArray(m_glInfo[i].m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, m_glInfo[i].m_faceCount * 3);
		}
	}
}

void Mesh::SetPosition(float x, float y, float z)
{
	m_modelTransform[3][0] = x;
	m_modelTransform[3][1] = y;
	m_modelTransform[3][2] = z;
}

vec3 Mesh::GetPosition()
{
	return vec3(m_modelTransform[3]);
}

void Mesh::Rotate(float x, float y, float z)
{
	m_modelTransform = glm::rotate(m_modelTransform, z, vec3(0, 0, 1));
	m_modelTransform = glm::rotate(m_modelTransform, y, vec3(0, 1, 0));
	m_modelTransform = glm::rotate(m_modelTransform, x, vec3(1, 0, 0));

}
