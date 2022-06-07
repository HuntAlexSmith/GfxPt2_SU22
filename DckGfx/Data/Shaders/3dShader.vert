#version 450 core

// Input variables
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec4 normal;

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