#include "Application.h"
#define STB_IMAGE_IMPLEMENTATION
#include "dependencies\stb\stb_image.h"

Application* Application::m_instance = nullptr;
const int targetFPS = 60;
const float FRAME_TIME = 1000.0f / (float)targetFPS;

Application::Application()
{
	m_shader = nullptr;
	m_camera = nullptr;
	m_toyObjLoader = nullptr;
}


Application::~Application()
{
	//This function wasn't being called earlier because glutSetOption had not been set to continue execution on OpenGL application exit
	Shutdown();
}

void Application::Shutdown()
{
	if (m_shader != nullptr)
		delete m_shader;

	if (m_camera != nullptr)
		delete m_camera;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		if (m_meshes[i] != nullptr)
			delete m_meshes[i];
	}
	std::cout << "Deleting" << std::endl;
}


void Application::Init()
{
	m_currTime = 0;
	m_prevTime = 0;
	m_deltaCursorX = 0;
	m_deltaCursorY = 0;
	m_frameTimeElapsed = 0;

	m_isRunning = true;
	m_canRender = false;
	m_canRender = false;
	m_instance = this;


	GLenum err = glewInit();
	//seems like glewInit() can't be called until a window instance exists
	if (err != GLEW_OK)
	{
		std::cout << "Error: " << glewGetErrorString(err) << std::endl;
	}

	m_position = vec3(0, 0, -5.0f);

	//add lights!
	//spot light still needs a lot of work
	m_lightManager.AddDirectionalLight(vec3(0, -1, 0));
	m_lightManager.SetGlobalLightColour(vec3(0, 1, 0));
	m_lightManager.AddSpecularLight(vec3(0, 0, -5), vec3(0, 0, 1));
	m_lightManager.SetSpecularPower(4);
	m_lightManager.AddSpotLight(vec3(0, 0, -5), vec3(0, 0, 1), 0.2f);

	m_shader = new Shader("Resources\\vsSource.vs", "Resources\\fsSource.fs", true);
	m_shader->UseShader();

	//Load texture
	//int imageWidth, imageHeight, imageFormat;
	//unsigned char* data = stbi_load("Resources\\brick_texture.jpg", &imageWidth, &imageHeight, &imageFormat, STBI_default);


	//Mesh *mesh = new Mesh("Resources\\stanford\\Bunny.obj");
	//m_meshes.push_back(mesh);
		
	m_camera = new FlyCamera(25, 2.5f, 45.0f, WINDOW_WIDTH / float(WINDOW_HEIGHT), 0.1f, 1000);
	m_camera->SetLookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));
}

void Application::Start(int argc, char * argv[])
{

	glutInit(&argc, argv);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	//perhaps make the display mode customizable in the future
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//could add glutInitWindowPosition(), but I will leave it out for now

	m_windowID = glutCreateWindow(m_title);

	glClearColor(1.0, 0.0, 1.0, 1.0); //does not work until a window context exists
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//I should add the error handling code

	glutReshapeFunc(ReshapeWrapper);
	glutMouseFunc(MouseButtonPressWrapper);
	glutMotionFunc(MouseMoveWrapper); //while a button is pressed
	glutPassiveMotionFunc(MouseMoveWrapper); //while no buttons are pressed
	glutDisplayFunc(DisplayWrapper);
	glutKeyboardFunc(KeyboardDownWrapper);
	glutKeyboardUpFunc(KeyboardUpWrapper);
	glutSpecialFunc(SpecialKeyboardDownWrapper);
	glutSpecialUpFunc(SpecialKeyboardUpWrapper);

	Init();

	glutIdleFunc(RunWrapper);
	glutMainLoop();
}

void Application::Update(float deltaTime)
{
	if (m_hasMouseMoved == false)
	{
		m_deltaCursorX = 0;
		m_deltaCursorY = 0;
	}

	auto end = m_keysHeldDown.end();
	if (m_keysHeldDown.find('w') != end)
	{
		m_camera->AddDeltaPosition(m_camera->GetForward());
	}
	if (m_keysHeldDown.find('s') != end)
	{
		m_camera->AddDeltaPosition(-m_camera->GetForward());
	}
	if (m_keysHeldDown.find('d') != end)
	{
		m_camera->AddDeltaPosition(-m_camera->GetRight());
	}
	if (m_keysHeldDown.find('a') != end)
	{
		m_camera->AddDeltaPosition(m_camera->GetRight());
	}
	if (m_keysHeldDown.find('q') != end)
	{
		m_camera->AddDeltaPosition(m_camera->GetUp());
	}
	if (m_keysHeldDown.find('e') != end)
	{
		m_camera->AddDeltaPosition(-m_camera->GetUp());
	}

	m_camera->Update(deltaTime, m_deltaCursorX, m_deltaCursorY);
	m_hasMouseMoved = false;

	//Perform update logic on meshes if needed

	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->Update(deltaTime);
	}

	//CTRL modifier
	//if (m_inputModifer == 2)
	{
		if (m_keysHeldDown.find('o') != end) //open up file dialogue
		{
			m_keysThatMustGoUp.insert('o');
			m_keysHeldDown.erase('o');
			OpenFileDialog fileDialog;
			fileDialog.Flags |= OFN_SHOWHELP; //show help button
			fileDialog.DefaultExtension = _T(".obj");
			fileDialog.FilterIndex = 0; //set to first file filter
			fileDialog.Title = _T("Choose file to open");
			if (fileDialog.ShowDialog())
			{

			}

			//wide string
			std::wstring s = fileDialog.FileName;

			//convert from wchar to char
			const wchar_t* wstr = s.c_str();
			size_t wlen = wcslen(wstr) + 1;
			char filepath[255];
			size_t convertedChars = 0;
			wcstombs_s(&convertedChars, filepath, wlen, wstr, _TRUNCATE);

			std::cout << "Loaded obj successfully!" << std::endl;
			Mesh* mesh = new Mesh(filepath, m_shader->GetShaderID(), 0);
			m_meshes.push_back(mesh);
		}
	}
}

void Application::Draw(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set global things here that affect all meshes, such as lighting

	unsigned int shaderID = m_shader->GetShaderID();
	unsigned int lightDirLoc = glGetUniformLocation(shaderID, "lightDir");
	unsigned int globalLightColourLoc = glGetUniformLocation(shaderID, "globalLightColour");
	unsigned int cameraPosLoc = glGetUniformLocation(shaderID, "cameraPos");
	unsigned int specDirLoc = glGetUniformLocation(shaderID, "specDir");
	unsigned int specPosLoc = glGetUniformLocation(shaderID, "specPos");
	unsigned int specPowLoc = glGetUniformLocation(shaderID, "specPow");

	unsigned int spotLightPosLoc = glGetUniformLocation(shaderID, "spotLightPos");
	unsigned int spotLightDirLoc = glGetUniformLocation(shaderID, "spotLightDir");
	unsigned int spotLightAngleLoc = glGetUniformLocation(shaderID, "spotLightAngles");


	vec3* dirs = m_lightManager.GetDirectionalLights();
	vec3* specDirs = m_lightManager.GetSpecularDirections();
	vec3* specPositions = m_lightManager.GetSpecularPositions();
	vec3 globalColour = m_lightManager.GetGlobalLightColour();
	vec3* spotLightPos = m_lightManager.GetSpotLightPositions();
	vec3* spotLightDir = m_lightManager.GetSpecularDirections();
	float* spotLightAngle = m_lightManager.GetSpotLightAngles();

	//set directional lights
	glUniform3fv(lightDirLoc, m_lightManager.GetNumDirectionalLights(), glm::value_ptr(dirs[0]));
	//set global light colour
	glUniform3fv(globalLightColourLoc, 1, glm::value_ptr(globalColour));
	//set camera pos
	glUniform3fv(cameraPosLoc, 1, glm::value_ptr(m_camera->GetPosition()));
	//set specular lights
	glUniform3fv(specDirLoc, 1, glm::value_ptr(specDirs[0]));
	glUniform3fv(specPosLoc, 1, glm::value_ptr(specPositions[0]));
	glUniform1f(specPowLoc, m_lightManager.GetSpecularPower());

	//set spot lights
	glUniform3fv(spotLightPosLoc, 1, glm::value_ptr(spotLightPos[0]));
	glUniform3fv(spotLightDirLoc, 1, glm::value_ptr(spotLightDir[0]));
	glUniform1fv(spotLightAngleLoc, 1, &spotLightAngle[0]);

	m_shader->Update();
	//open old bootstrap project and figure it out
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->Draw(m_camera->GetViewTransform(), m_camera->GetProjectionTransform());
	}
}

void Application::Display()
{
	if (m_canRender)
	{
		ClearScreen();
		Draw(m_deltaTime);
		glutSwapBuffers();
	}
}

void Application::MouseButtonPressed(int button, int state, int x, int y)
{
}

void Application::Reshape(int width, int height)
{
	glViewport(0, 0, width, height);

	m_camera->SetPerspective(m_camera->GetFOV(), width / (float)height, m_camera->GetNearPlane(), m_camera->GetFarPlane());
}

void Application::ConstructMesh(const char * filename)
{
	Mesh* mesh = new Mesh(filename, m_shader->GetShaderID(), 0);
	m_meshes.push_back(mesh);
}

void Application::CreateBuffers(int numAttribs, int * numFloats, int numVerts, int numIndices, Vertex * vertices, unsigned int * indices)
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

}

void Application::Run()
{	
	if (m_isRunning)
	{
		m_currTime = GetTime();
		m_deltaTime = m_currTime - m_prevTime;
		m_prevTime = m_currTime;

		m_frameTimeElapsed += m_deltaTime;
		//m_canRender = false; //bring back if using fixed frame rate
		m_canRender = true;

		m_deltaTime /= 1000.0f;
		
		Update(m_deltaTime);
		glutPostRedisplay();
	
		/*
		if (m_frameTimeElapsed > FRAME_TIME)
		{
			glutPostRedisplay();
			m_frameTimeElapsed -= FRAME_TIME;
			m_canRender = true;
		}*/
	}
	else
	{
		glutLeaveMainLoop();
		//glutDestroyWindow(m_windowID);
	}
}


int Application::GetTime()
{
	return glutGet(GLUT_ELAPSED_TIME);
}

void Application::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Application::SetBackgroundColour(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
}

void Application::Quit()
{
	m_isRunning = false;
}

void Application::SetInstance()
{
	m_instance = this;
}

void Application::DisplayWrapper()
{
	m_instance->Display();
}

void Application::ReshapeWrapper(int width, int height)
{
	m_instance->Reshape(width, height);
}

void Application::RunWrapper()
{
	m_instance->Run();
}

void Application::MouseButtonPressWrapper(int button, int state, int x, int y)
{
	m_instance->MouseButtonPressed(button, state, x, y);
}

void Application::MouseMoveWrapper(int x, int y)
{
	float deltaX = m_instance->m_oldCursorPosX - x;
	float deltaY = m_instance->m_oldCursorPosY - y;
	m_instance->m_deltaCursorX = glm::clamp(deltaX, -3.0f, 3.0f);
	m_instance->m_deltaCursorY = glm::clamp(deltaY, -3.0f, 3.0f);

	m_instance->m_oldCursorPosX = x;
	m_instance->m_oldCursorPosY = y;
	m_instance->m_hasMouseMoved = true;

}

void Application::KeyboardDownWrapper(unsigned char key, int x, int y)
{
	m_instance->m_inputModifer = glutGetModifiers();

	if (key == 27) //escape key
	{
		m_instance->Quit();
	}
	if (tolower(key) == 'w') //up
	{
		m_instance->m_keysHeldDown.insert('w');
	}
	else if (tolower(key) == 's') //down
	{
		m_instance->m_keysHeldDown.insert('s');
	}

	else if (tolower(key) == 'd') //right
	{
		m_instance->m_keysHeldDown.insert('d');
	}

	else if (tolower(key) == 'a') //left
	{
		m_instance->m_keysHeldDown.insert('a');
	}

	else if (tolower(key) == 'q')
	{
		m_instance->m_keysHeldDown.insert('q');
	}

	else if (tolower(key) == 'e')
	{
		m_instance->m_keysHeldDown.insert('e');
	}
	
	else if (tolower(key) == 'o')
	{
		auto end = m_instance->m_keysThatMustGoUp.end();
		if (m_instance->m_keysThatMustGoUp.find('o') == end)
		{
			//key must not be in a waiting to go up position
			m_instance->m_keysHeldDown.insert('o');
		}

	}

}

void Application::KeyboardUpWrapper(unsigned char key, int x, int y)
{
	//m_instance->m_inputModifer = 0;

	if (tolower(key) == 'w') //up
	{
		m_instance->m_keysHeldDown.erase('w');
	}
	else if (tolower(key) == 's') //down
	{
		m_instance->m_keysHeldDown.erase('s');
	}

	else if (tolower(key) == 'd') //right
	{
		m_instance->m_keysHeldDown.erase('d');
	}

	else if (tolower(key) == 'a') //left
	{
		m_instance->m_keysHeldDown.erase('a');
	}

	else if (tolower(key) == 'q') //up
	{
		m_instance->m_keysHeldDown.erase('q');
	}

	else if (tolower(key) == 'e') //left
	{
		m_instance->m_keysHeldDown.erase('e');
	}

	else if (tolower(key) == 'o')
	{
		auto end = m_instance->m_keysThatMustGoUp.end();
		if (m_instance->m_keysThatMustGoUp.find('o') != end)
		{
			//key has gone up, it is no longer 'waiting' to go up
			m_instance->m_keysThatMustGoUp.erase('o');
		}
		m_instance->m_keysHeldDown.erase('o');
	}
}

void Application::SpecialKeyboardDownWrapper(int key, int x, int y)
{
	if (key == 101) //up arrow
	{
		vec3 forward = m_instance->m_camera->GetForward();
		m_instance->m_camera->AddDeltaPosition(forward);
	}
	else if (key == 103) //down arrow
	{
		vec3 forward = m_instance->m_camera->GetForward();
		m_instance->m_camera->AddDeltaPosition(-forward);
	}

	else if (key == 102) //right arrow
	{
		vec3 right = m_instance->m_camera->GetRight();
		m_instance->m_camera->AddDeltaPosition(-right);
	}

	else if (key == 100) //left arrow
	{
		vec3 right = m_instance->m_camera->GetRight();
		m_instance->m_camera->AddDeltaPosition(right);
	}

}

void Application::SpecialKeyboardUpWrapper(int key, int x, int y)
{
}
