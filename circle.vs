#version 330 core
attribute vec4 vertex;
attribute vec2 value;
varying vec2 val;

void main()
{
    val = value;
    gl_Position=vertex;
}