#version 450 core

// Input variables
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;

// Uniform variables
uniform mat4 objToWorld;
uniform mat4 worldToCam;
uniform mat4 camToNDC;

// Output variables
out vec3 myColor;

// Main function
void main() {
    gl_Position = camToNDC * worldToCam * objToWorld * position;
    myColor = color;
}
