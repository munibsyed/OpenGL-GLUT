#include "Application.h"

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
	//what if the destructor called without Start() ever being called?

	//I need to take care here

	if (m_shader != nullptr)
		delete m_shader;

	if (m_camera != nullptr)
		delete m_camera;

	if (m_toyObjLoader != nullptr)
		delete m_toyObjLoader;

	for (int i = 0; i < m_meshes.size(); i++)
	{
		if (m_meshes[i] != nullptr)
			delete[] m_meshes[i];
	}
}


void Application::Init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	/*	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	*/
	glEnable(GL_DEPTH_TEST);

	GLenum err = glewInit();
	if (err != GLEW_OK)
		std::cout << "Error!" << std::endl;


	Vertex *vertices = nullptr;
	unsigned int* indices = nullptr;
	//vertices = new Vertex[6];
	//indices = new unsigned int[6]{ 0, 1, 2, 3, 4, 5 };
	//vertices[0].position = vec4(-2.5f, 0.0f, 0.0f, 1.0f);
	//vertices[1].position = vec4(0.0f, 2.5f, 0.0f, 1.0f);
	//vertices[2].position = vec4(2.5f, 0.0f, 0.0f, 1.0f);

	//vertices[3].position = vec4(-5.5f, 10.0f, -30.0f, 1.0f);
	//vertices[4].position = vec4(0.0f, 15.5f, -30.0f, 1.0f);
	//vertices[5].position = vec4(5.5f, 10.0f, -30.0f, 1.0f);


	//vertices[0].colour = vec4(1.0f, 0.0f, 0.0f, 0.5f);
	//vertices[1].colour = vec4(0.0f, 1.0f, 0.0f, 0.5f);
	//vertices[2].colour = vec4(1.0f, 0.0f, 1.0f, 0.5f);

	//vertices[3].colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	//vertices[4].colour = vec4(0.0f, 0.0, 1.0f, 1.0f);
	//vertices[5].colour = vec4(0.0f, 0.0f, 1.0f, 1.0f);
	

	m_shader = new Shader("Resources\\vsSource.vs", "Resources\\fsSource.fs", true);
	m_shader->UseShader();

	
	m_toyObjLoader = new MyObjLoader();

	std::vector<vec3> verts, normals;
	std::vector<vec2> uvs;
	std::vector<OBJIndex> meshIndices;
	
	if (m_toyObjLoader->LoadObj("Resources\\cube.obj", verts, uvs, normals, meshIndices)) //get vertices, and index object

	//if (m_toyObjLoader->LoadObj("Resources\\cube.obj",
		//meshVertices, meshUvs, meshNormals, vertexIndices, uvIndices, normalIndices))
	{
		std::cout << "Loaded obj successfully!" << std::endl;


		//Mesh *mesh = new Mesh(meshVertices, vertexIndices, m_shader->GetShaderID());
		Mesh* mesh = new Mesh(verts, normals, uvs, meshIndices, m_shader->GetShaderID());
		m_meshes.push_back(mesh);
	}
	else
	{
		std::cout << "Could not load obj. Try using a better obj loader" << std::endl;
	}
		
	m_camera = new FlyCamera(25, 2.5f, 45.0f, WINDOW_WIDTH / float(WINDOW_HEIGHT), 0.1f, 100);
	m_camera->SetLookAt(vec3(0, 0, -5), vec3(0, 0, 0), vec3(0, 1, 0));

	if (vertices != nullptr)
		delete[] vertices;
	if (indices != nullptr)
		delete[] indices;



}

void Application::Start(int argc, char * argv[])
{
	m_currTime = 0;
	m_prevTime = 0;
	m_deltaCursorX = 0;
	m_deltaCursorY = 0;
	m_frameTimeElapsed = 0;
	m_position = vec3(0, 0, -5.0f);

	m_isRunning = true;
	m_canRender = false;
	SetInstance();
	glutInit(&argc, argv);
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
	//perhaps make the display mode customizable in the future
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH | GLUT_STENCIL);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	//could add glutInitWindowPosition(), but I will leave it out for now

	m_windowID = glutCreateWindow(m_title);

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
			if (m_toyObjLoader->LoadObj(filepath,
				meshVertices, meshUvs, meshNormals, vertexIndices, uvIndices, normalIndices))
			{
				std::cout << "Loaded obj successfully!" << std::endl;
				Mesh* mesh = new Mesh(meshVertices, vertexIndices, m_shader->GetShaderID());
				m_meshes.push_back(mesh);
			}
			else
			{
				std::cout << "Could not load obj. Try using a better obj loader" << std::endl;
			}
			
		}
	}


	m_camera->Update(deltaTime, m_deltaCursorX, m_deltaCursorY);
	m_hasMouseMoved = false;

}

void Application::Draw(float deltaTime)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_shader->UseShader(); //probably doesn't need to call glUseProgram() every frame

	//unsigned int mvpLocation = glGetUniformLocation(m_shader->GetShaderID(), "modelViewProjection");
	//glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjectionViewTransform()));
	unsigned int shaderID = m_shader->GetShaderID();
	unsigned int model = glGetUniformLocation(shaderID, "model");
	unsigned int view = glGetUniformLocation(shaderID, "view");
	unsigned int projection = glGetUniformLocation(shaderID, "projection");
		
	glUniformMatrix4fv(model, 1, GL_FALSE, glm::value_ptr(glm::mat4()));
	m_camera->UpdateViewTransform();
	glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(m_camera->GetViewTransform()));
	glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(m_camera->GetProjectionTransform()));

	m_shader->Update();
	//open old bootstrap project and figure it out
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i]->Draw();
	}
	/*glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, meshVertices.size(), GL_UNSIGNED_INT, 0);*/
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
		glutDestroyWindow(m_windowID);
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
	if (key == 'w') //up
	{
		m_instance->m_keysHeldDown.insert('w');
	}
	else if (key == 's') //down
	{
		m_instance->m_keysHeldDown.insert('s');
	}

	else if (key == 'd') //right
	{
		m_instance->m_keysHeldDown.insert('d');
	}

	else if (key == 'a') //left
	{
		m_instance->m_keysHeldDown.insert('a');
	}

	else if (key == 'q')
	{
		m_instance->m_keysHeldDown.insert('q');
	}

	else if (key == 'e')
	{
		m_instance->m_keysHeldDown.insert('e');
	}
	
	else if (key == 'o')
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

	if (key == 'w') //up
	{
		m_instance->m_keysHeldDown.erase('w');
	}
	else if (key == 's') //down
	{
		m_instance->m_keysHeldDown.erase('s');
	}

	else if (key == 'd') //right
	{
		m_instance->m_keysHeldDown.erase('d');
	}

	else if (key == 'a') //left
	{
		m_instance->m_keysHeldDown.erase('a');
	}

	else if (key == 'q') //up
	{
		m_instance->m_keysHeldDown.erase('q');
	}

	else if (key == 'e') //left
	{
		m_instance->m_keysHeldDown.erase('e');
	}

	else if (key == 'o')
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
