#version 410
#define MAX_LIGHTS 2

in vec3 vPosition;
in vec4 vColour;
in vec3 vNormal;
in vec3 vLightDir[MAX_LIGHTS];
in vec3 vSpecDir[MAX_LIGHTS];
in vec3 vSpecPos[MAX_LIGHTS];
in float vSpecPow;

in vec3 vSpotLightDir[MAX_LIGHTS];
in vec3 vSpotLightPos[MAX_LIGHTS];
in float vSpotLightAngles[MAX_LIGHTS];

in vec3 vGlobalLightColour;
in vec3 vCameraPos;

out vec4 fragColor;
void main() {

    float d = 0; 
    for (int i = 0; i < vLightDir.length(); i++)
    {
        d += max(0, dot(normalize(vNormal), -vLightDir[i]));
    }
    float s = 0;
    vec3 toCam = normalize(vCameraPos - vPosition);
    for (int i = 0; i < vSpecPos.length(); i++)
    {
	vec3 reflected = reflect(vSpecDir[i], vNormal);
	float spec = max(0, dot(toCam, reflected));
	s += pow(spec, vSpecPow);
    }

    float spot = 0;
    for (int i = 0; i < vSpotLightPos.length(); i++)
    {
	vec3 toLight = normalize(vSpotLightPos[i] - vPosition);
	vec3 dirNormalized = normalize(vSpotLightDir[i]);
	//is inside cone?
	float spotDot = dot(toLight, -dirNormalized);
	if (spotDot > vSpotLightAngles[i])
	{
	    //spot light angle is expressed as a cosine. E.g. 90 deg = dot > 0, 45 deg = dot > 0.5
	    spot += max(dot(normalize(vNormal), toLight), 0);
	}
    }	
    
    //fragColor = vec4(spot * vec3(1,1,1), 1);
    fragColor = vec4(d * vGlobalLightColour + (s * vGlobalLightColour * 0) + spot * vec3(0,0,0), 1);
    //fragColor = vec4(vNormal,1);
}