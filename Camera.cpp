#include "Camera.h"



Camera::Camera(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	SetPerspective(fov, aspectRatio, nearPlane, farPlane);
}


Camera::~Camera()
{

}

void Camera::Update(float deltaTime)
{
}

void Camera::SetLookAt(const vec3 & eye, const vec3 & target, const vec3 & up)
{
	m_viewTransform = glm::lookAt(eye, target, up);
	UpdateProjectionViewTransform();
	UpdateWorldTransform();
}

void Camera::SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	m_projectionTransform = glm::perspective(fov, aspectRatio, nearPlane, farPlane);
	m_FOV = fov;
	m_nearPlane = nearPlane;
	m_farPlane = farPlane;
	//UpdateProjectionViewTransform();
}

void Camera::UpdateViewTransform()
{
	m_viewTransform = glm::inverse(m_worldTransform);
}

void Camera::UpdateWorldTransform()
{
	m_worldTransform = glm::inverse(m_viewTransform);
}


void Camera::UpdateProjectionViewTransform()
{
	//std::cout << m_worldTransform[3][0] << ", " << m_worldTransform[3][1] << ", " << m_worldTransform[3][2] << std::endl;
	m_projectionViewTransform = m_projectionTransform * m_viewTransform;
	//m_projectionViewTransform = m_projectionTransform;
}

float Camera::GetFOV()
{
	return m_FOV;
}

float Camera::GetNearPlane()
{
	return m_nearPlane;
}

float Camera::GetFarPlane()
{
	return m_farPlane;
}

vec3 Camera::GetPosition()
{
	return vec3(m_worldTransform[3]);
}


mat4 Camera::GetWorldTransform()
{
	return m_worldTransform;
}

mat4 Camera::GetViewTransform()
{
	return m_viewTransform;
}

mat4 Camera::GetProjectionTransform()
{
	return m_projectionTransform;
}

mat4 Camera::GetProjectionViewTransform()
{
	return m_projectionViewTransform;
}

Camera::Camera()
{
}
