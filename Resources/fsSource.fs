#version 410
in vec4 vColour;
in vec3 vNormal;
in vec3 vLightDir[4];
in vec3 vGlobalLightColour;

out vec4 fragColor;
void main() {

    float d = 0; 
    for (int i = 0; i < vLightDir.length(); i++)
    {
        d += max(0, dot(normalize(vNormal.xyz), -vLightDir[i]));
    }
    fragColor = vec4(d * vGlobalLightColour, 1);
}