#include "FlyCamera.h"



FlyCamera::FlyCamera(float movementSpeed, float rotationSpeed, float fov, float aspectRatio, float nearPlane, float farPlane)
{
	SetPerspective(fov, aspectRatio, nearPlane, farPlane);

	m_rotationSpeed = rotationSpeed;
	m_movementSpeed = movementSpeed;

	m_angleX = 0;
	m_angleY = 0;
}


FlyCamera::~FlyCamera()
{
}

void FlyCamera::Update(float deltaTime, float deltaCursorX, float deltaCursorY)
{

	vec3 deltaPosition = m_deltaPosition * m_movementSpeed * deltaTime;
	AddPosition(deltaPosition);
	m_deltaPosition = vec3(0);

	m_angleX += deltaCursorX * m_rotationSpeed * deltaTime;
	m_angleY += deltaCursorY * m_rotationSpeed * deltaTime;
	vec3 position = GetPosition();
	SetLookAt(position, position + GetForward(), GetUp());

	/*
	float angleX = deltaCursorX * m_rotationSpeed * deltaTime;
	float angleY = deltaCursorY * m_rotationSpeed * deltaTime;

	m_angleX = (400 - cursorX) * 0.005f;
	m_angleY = (300 - cursorY) * 0.005f;
	m_angleY = glm::clamp(angleY, glm::radians(-60.0f), glm::radians(60.0f));

	vec3 direction(cos(m_angleY) * sin(m_angleX),
	sin(m_angleY),
	cos(m_angleY) * cos(m_angleX));

	//calculate right vector, makes an 'arc' on the x,z plane
	vec3 right(cos(m_angleX) - 3.14f/2.0f, 0, sin(m_angleX) - 3.14f / 2.0f);
	vec3 up = glm::cross(direction, right);
	//mat4 rotation = glm::rotate(angleX, vec3(0, 1, 0)); //should it always rotate on a global up axis?
	//rotation *= glm::rotate(-angleY, vec3(m_worldTransform[0]));
	//m_worldTransform *= rotation;

	vec3 forward = GetForward();
	vec3 newForward = glm::rotate(forward, angleX, vec3(0, 1, 0));
	vec3 newForward2 = glm::rotate(forward, -angleY, vec3(1, 0, 0));

	vec3 position = GetPosition();
	m_viewTransform = glm::lookAt(position, position + newForward + newForward2, vec3(0,1,0));
	m_target = position + newForward + newForward2;
	UpdateWorldTransform();

	vec3 deltaPosition = m_deltaPosition * m_movementSpeed * deltaTime;
	AddPosition(deltaPosition);
	m_deltaPosition = vec3(0);


	UpdateViewTransform();
	UpdateProjectionViewTransform();

	*/
}

mat4 FlyCamera::GetOrientation()
{
	mat4 orientation;
	orientation = glm::rotate(orientation, m_angleY, vec3(1, 0, 0));
	orientation = glm::rotate(orientation, -m_angleX, vec3(0, 1, 0));
	return orientation;
}

void FlyCamera::SetPosition(const vec3& position)
{
	//std::cout << "Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	m_worldTransform[3][0] = position.x;
	m_worldTransform[3][1] = position.y;
	m_worldTransform[3][2] = position.z;
	m_viewTransform = glm::inverse(m_worldTransform);
	UpdateProjectionViewTransform();

}

vec3 FlyCamera::GetForward()
{
	vec4 forward = glm::inverse(GetOrientation()) * vec4(0, 0, 1, 1);
	return vec3(forward);
}

vec3 FlyCamera::GetRight()
{
	vec4 right = glm::inverse(GetOrientation()) * vec4(1, 0, 0, 1);
	return vec3(right);
}

vec3 FlyCamera::GetUp()
{
	vec4 up = glm::inverse(GetOrientation()) * vec4(0, 1, 0, 1);
	return vec3(up);
}

void FlyCamera::AddPosition(const vec3 &deltaPosition)
{
	m_worldTransform[3][0] += deltaPosition.x;
	m_worldTransform[3][1] += deltaPosition.y;
	m_worldTransform[3][2] += deltaPosition.z;

	UpdateViewTransform();
	UpdateProjectionViewTransform();
}

void FlyCamera::SetDeltaPosition(const vec3 & deltaPosition)
{
	m_deltaPosition = deltaPosition;
}

void FlyCamera::AddDeltaPosition(const vec3 & deltaPosition)
{
	m_deltaPosition += deltaPosition;
}