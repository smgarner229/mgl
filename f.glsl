#version 330 core
in vec4 vertexColor;
out vec4 FragColor;

uniform vec3 ourColor;

void main()
{
   gl_FragData[0] = vertexColor;
}