#version 330 core
out vec4 FragColor;

in vec3 ourColor; // the input variable from the vertex shader (same name and same type)  

void main()
{
    //--original output bla bla bla
    FragColor = vec4(ourColor, 1.0f);
}