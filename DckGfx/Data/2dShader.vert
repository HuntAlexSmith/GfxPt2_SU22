#version 450 core

// Input variables
in vec4 position;
in vec3 color;

// Output variables
out vec3 myColor;

// Main function
void main() {
    gl_Position = position;
    myColor = color;
}
