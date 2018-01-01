#pragma once
#include <glm.hpp>
#include <vector>
using glm::vec3;

//should be some kind of light manager class
//should be able to return an array of directions

//should probably be a singleton

class LightManager
{
public:
	LightManager();
	~LightManager();

	void AddPointLight(vec3 position);

	void AddSpotLight(vec3 position, vec3 direction, float angle);

	void AddDirectionalLight(vec3 dir);

	void AddDirectionalLights(const std::vector<vec3> &dirs);

	void AddDirectionalLights(vec3 dirs[], int numLights);

	int GetNumDirectionalLights();

	int GetNumSpecularLights();

	int GetNumSpotLights();

	vec3* GetDirectionalLights();

	vec3* GetSpecularPositions();

	vec3* GetSpecularDirections();
	
	vec3* GetPointLights();

	vec3* GetSpotLightPositions();

	vec3* GetSpotLightDirections();

	float* GetSpotLightAngles();

	void AddSpecularLight(vec3 position, vec3 dir);

	//this sets the colour for all lights
	void SetGlobalLightColour(vec3 colour);

	vec3 GetGlobalLightColour();
	
	void SetSpecularPower(float specPower);

	float GetSpecularPower();

private:
	std::vector<vec3> m_dirs; //represents all the directional lights
	std::vector<vec3> m_specLightDirs;
	std::vector<vec3> m_specLightPositions;
	std::vector<vec3> m_pointLightPositions;
	std::vector<vec3> m_spotLightPositions;
	std::vector<vec3> m_spotLightDirections;
	std::vector<float> m_spotLightAngles;

	float m_specularPower;

	vec3 m_globalLightColour;

};


