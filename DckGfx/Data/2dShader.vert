#version 450 core

// Input variables
in vec3 position;
in vec3 color;

// Output variables
out vec3 myColor;

// Main function
void main() {
    gl_Position = vec4(position.x, position.y, position.z, 1);
    myColor = color;
}
