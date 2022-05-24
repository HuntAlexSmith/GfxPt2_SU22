#version 450 core

// Input variables
in vec4 position;
in vec3 color;
in vec4 normal;

// Uniform variables
uniform mat4 objToWorld;
uniform mat4 worldToCam;
uniform mat4 perspMat;
uniform mat4 normMat;

out vec3 myColor;
out vec3 camNorm;

void main() {
    gl_Position = perspMat * worldToCam * objToWorld * position;
    camNorm = mat3(worldToCam) * mat3(normMat) * normal.xyz;
    myColor = color;
}