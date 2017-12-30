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

	void AddDirectionalLight(vec3 dir);

	void AddDirectionalLights(const std::vector<vec3> &dirs);

	void AddDirectionalLights(vec3 dirs[], int numLights);

	vec3* GetDirectionalLights();

	//this sets the colour for all lights
	void SetGlobalLightColour(vec3 colour);

	vec3 GetGlobalLightColour();

	int GetNumDirectionalLights();
	
private:
	std::vector<vec3> m_dirs; //represents all the directional lights
	vec3 m_globalLightColour;

};


