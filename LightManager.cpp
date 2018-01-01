#include "LightManager.h"



LightManager::LightManager()
{
}


LightManager::~LightManager()
{
}

void LightManager::AddPointLight(vec3 position)
{
	m_pointLightPositions.push_back(position);
}

void LightManager::AddSpotLight(vec3 position, vec3 direction, float angle)
{
	m_spotLightPositions.push_back(position);
	m_spotLightDirections.push_back(direction);
	m_spotLightAngles.push_back(angle);
}

void LightManager::AddDirectionalLight(vec3 dir)
{
	m_dirs.push_back(dir);
}

void LightManager::AddDirectionalLights(const std::vector<vec3> &dirs)
{
	m_dirs.insert(m_dirs.end(), dirs.begin(), dirs.end());
}

void LightManager::AddDirectionalLights(vec3 dirs[], int numLights)
{
	for (int i = 0; i < numLights; i++)
	{
		m_dirs.push_back(dirs[i]);
	}
}

vec3 * LightManager::GetDirectionalLights()
{
	return &m_dirs[0];
}

vec3 * LightManager::GetSpecularPositions()
{
	return &m_specLightPositions[0];
}

vec3 * LightManager::GetSpecularDirections()
{
	return &m_specLightDirs[0];
}

vec3 * LightManager::GetPointLights()
{
	return &m_pointLightPositions[0];
}

vec3 * LightManager::GetSpotLightPositions()
{
	return &m_spotLightPositions[0];
}

vec3 * LightManager::GetSpotLightDirections()
{
	return &m_spotLightDirections[0];
}

float * LightManager::GetSpotLightAngles()
{
	return &m_spotLightAngles[0];
}

void LightManager::AddSpecularLight(vec3 position, vec3 dir)
{
	m_specLightDirs.push_back(dir);
	m_specLightPositions.push_back(position);
}

void LightManager::SetGlobalLightColour(vec3 colour)
{
	m_globalLightColour = colour;
}

vec3 LightManager::GetGlobalLightColour()
{
	return m_globalLightColour;
}

int LightManager::GetNumDirectionalLights()
{
	return m_dirs.size();
}

int LightManager::GetNumSpecularLights()
{
	return m_specLightPositions.size();
}

int LightManager::GetNumSpotLights()
{
	return m_spotLightPositions.size();
}

void LightManager::SetSpecularPower(float specPower)
{
	m_specularPower = specPower;
}

float LightManager::GetSpecularPower()
{
	return m_specularPower;
}
