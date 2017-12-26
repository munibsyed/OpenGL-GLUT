#pragma once
#include <glm.hpp>
#include <ext.hpp>
#include <iostream>
#include <Windows.h>

using glm::mat4;
using glm::vec4;
using glm::vec3;


class Camera
{
public:
	Camera(float fov, float aspectRatio, float nearPlane, float farPlane);
	~Camera();

	virtual void Update(float deltaTime);

	void SetLookAt(const vec3 &eye, const vec3 &target, const vec3& up);

	void SetPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);

	void UpdateViewTransform();

	void UpdateWorldTransform();

	void UpdateProjectionViewTransform();

	float GetFOV();

	float GetNearPlane();

	float GetFarPlane();

	vec3 GetPosition();

	mat4 GetWorldTransform();

	mat4 GetViewTransform();

	mat4 GetProjectionTransform();

	mat4 GetProjectionViewTransform();


protected:
	mat4 m_worldTransform;
	mat4 m_viewTransform;
	mat4 m_projectionTransform;
	mat4 m_projectionViewTransform;

	float m_FOV; //degrees
	float m_nearPlane;
	float m_farPlane;

	Camera();
};

