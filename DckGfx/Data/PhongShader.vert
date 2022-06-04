#version 450 core

// Input variables, remember layout
layout(location = 0) in vec4 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec4 normal;

// Uniform variables, mainly matrices
uniform mat4 objToWorld;
uniform mat4 worldToCam;
uniform mat4 perspMat;
uniform mat4 normMat;

// Output variables
out vec3 myColor;
out vec4 worldPos;
out vec4 worldNorm;

void main() {
    // Give the color to the output variable
    myColor = color;

    // Give the world position and normal to the output
    worldPos = objToWorld * position;
    worldNorm = normMat * normal;

    // Calculate gl_Position
    gl_Position = perspMat * worldToCam * objToWorld * position;
}