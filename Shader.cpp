#include "Shader.h"



Shader::Shader(const char* vs, const char* fs, bool fromFile)
{
	std::string vsContents = ""; //so it doesn't go out of scope
	std::string fsContents = "";
	if (fromFile)
	{
		std::ifstream vsFile(vs);
		
		while (!vsFile.eof())
		{
			std::string line;
			std::getline(vsFile, line);
			vsContents += line + '\n';
		}
		m_vsSource = vsContents.c_str();

		std::ifstream fsFile(fs);
		while (!fsFile.eof())
		{
			std::string line;
			std::getline(fsFile, line);
			fsContents += line + '\n';
		}
		m_fsSource = fsContents.c_str();

	}
	else
	{
		//interpret input literally
		m_vsSource = vs;
		m_fsSource = fs;
	}

	CompileAndAttachShaders(m_vsSource, m_fsSource);

}

Shader::Shader(const Shader & other)
{
}


Shader::~Shader()
{
}

unsigned int Shader::GetShaderID()
{
	return m_shaderID;
}

void Shader::UseShader()
{
	//make sure that this can't be called before the initialization logic is done
	glUseProgram(m_shaderID);
}

void Shader::Update()
{
	//performs any necessary update logic, such as setting uniforms.
	//empty for the base class
}

Shader::Shader()
{
}

void Shader::CompileAndAttachShaders(const char * vsSource, const char * fsSource)
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);


	const char** vsSourcePtr = &m_vsSource;
	const char** fsSourcePtr = &m_fsSource;
	
	glShaderSource(vertexShader, 1, vsSourcePtr, NULL);
	glShaderSource(fragmentShader, 1, fsSourcePtr, NULL);

	glCompileShader(vertexShader);
	glCompileShader(fragmentShader);

	m_shaderID = glCreateProgram();

	glAttachShader(m_shaderID, vertexShader);
	glAttachShader(m_shaderID, fragmentShader);

	glLinkProgram(m_shaderID);

	GLint success = GL_FALSE;

	glGetProgramiv(m_shaderID, GL_LINK_STATUS, &success);

	GLenum err = glGetError();
	if (err != GL_NO_ERROR)
	{
		printf("Error here\n");
		std::cout << glewGetErrorString(err) << std::endl;
	}

	if (success == GL_FALSE)
	{
		int infoLogLength = 0;
		glGetProgramiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_shaderID, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s\n", infoLog);


		delete[] infoLog;
	}

	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
}
