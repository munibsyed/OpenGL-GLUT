#pragma once
#include "Camera.h"
class FlyCamera :
	public Camera
{
public:
	FlyCamera(float movementSpeed, float rotationSpeed, float fov, float aspectRatio, float nearPlane, float farPlane);
	~FlyCamera();

	virtual void Update(float deltaTime, float deltaCursorX, float deltaCursorY);
	
	void SetPosition(const vec3 &position);
	
	mat4 GetOrientation();

	vec3 GetForward();

	vec3 GetRight();

	vec3 GetUp();

	void AddPosition(const vec3 &deltaPosition);

	void SetDeltaPosition(const vec3 &deltaPosition);

	void AddDeltaPosition(const vec3 &deltaPosition);

private:

	float m_rotationSpeed;
	float m_movementSpeed;

	vec3 m_deltaPosition;
	float m_angleX;
	float m_angleY;
};

