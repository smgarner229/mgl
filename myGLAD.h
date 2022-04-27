#ifndef __MYGLAD_H__
#define __MYGLAD_H__

#include <GLFW/glfw3.h>
#include <GL/gl.h>

typedef void (*GL_VIEWPORT_TYPE)(int,int,int,int);
GL_VIEWPORT_TYPE glViewport = (GL_VIEWPORT_TYPE)glfwGetProcAddress("glViewport");

#endif

