#version 450 core

in vec3 myColor;
in vec3 camNorm;

uniform int ignoreNorm;

out vec4 FragColor;

void main() {
    float shade = 1.0f;
    if(ignoreNorm == 0)
    {
        vec3 m = normalize(camNorm);
        shade = max(0, m.z);
    }
    FragColor = vec4(shade*myColor, 1);
}