#include "Mesh.h"


Mesh::Mesh(const std::vector<vec3>& vertices, const std::vector<vec3>& normals, const std::vector<vec2>& uvs, const std::vector<OBJIndex>& indices, unsigned int shaderID)
{
	m_shaderID = shaderID;
	m_VBO = 0;
	m_VAO = 0;
	m_IBO = 0;
	int numFloats[2] = { 4, 4 };
	m_objIndices = indices; //copying could be slow with larger meshes
	m_vertices.resize(vertices.size());
	for (int i = 0; i < vertices.size(); i++)
	{
		Vertex vert;
		vert.position = vec4(vertices[i], 1);
		m_vertices[i] = vert;
	}

	Vertex* vertexArr = new Vertex[vertices.size()];
	vertexArr = &m_vertices[0];
	unsigned int* indexArr = new unsigned int[indices.size()];
	for (int i = 0; i < indices.size(); i++)
	{
		indexArr[i] = indices[i].vertexIndex;
	}

	CreateBuffers(1, numFloats, vertices.size(), indices.size(), vertexArr, indexArr);

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
		glBindVertexArray(m_VAO);
		size = m_objIndices.size();
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);
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
