#version 450 core

// Input variables
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

// Uniform variables
uniform mat4 objToWorld;
uniform mat4 worldToCam;
uniform mat4 perspMat;

out vec3 myColor;

void main() {
    gl_Position = perspMat * worldToCam * objToWorld * position;

    myColor = color;
}