#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 color;
out vec4 vertexColor;
uniform mat4 transform_1;
void main()
{
   gl_Position = transform_1*vec4(aPos,1.0);
   vertexColor = vec4((gl_Position.x+1.)/2.,(gl_Position.y+1.)/2.,0.2,1.0);
}