#version 460 core

out vec4 FragColor;
in vec3 chosenColor;

void main() {
    FragColor = vec4(chosenColor, 1.0);
}