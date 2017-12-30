#include "LightManager.h"



LightManager::LightManager()
{
}


LightManager::~LightManager()
{
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
