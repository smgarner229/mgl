#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

//#include <GL/gl.h>
#include <GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int up_or_down = 0; // 0 is up, 1 is down
void bounce_index(float * index);

int main(int argc, char ** argv)
{
  GLFWwindow * window;
  int i;
  i = atoi(argv[1]);  
  if(!glfwInit())
  {
    fprintf(stderr,"Could not open window!\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,i);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);

  window = glfwCreateWindow(800,600,"Hello There",NULL,NULL);
  if(!window)
  {
    fprintf(stdout,"Failed to initialize window.  Exitting\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }

  glfwMakeContextCurrent(window);

  typedef void (*GL_VIEWPORT_TYPE)(int,int,int,int);
  GL_VIEWPORT_TYPE glViewport = (GL_VIEWPORT_TYPE)glfwGetProcAddress("glViewport");
  glViewport(0,0,800,600);
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

  // define the function’s prototype
  typedef void (*GL_CLEAR_TYPE) (GLbitfield);
  // find the function and assign it to a function pointer
  GL_CLEAR_TYPE glClear = (GL_CLEAR_TYPE)glfwGetProcAddress("glClear");

  typedef void (*GL_CLEAR_COLOR_TYPE)(float,float,float,float);
  GL_CLEAR_COLOR_TYPE glClearColor=(GL_CLEAR_COLOR_TYPE)glfwGetProcAddress("glClearColor");

  float j = 0.0;
  float k = 0.25;
  float l = 0.5;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);

    /* Render here */
    glClearColor(j,k,l,1.0);
    bounce_index(&j);
    bounce_index(&k);
    bounce_index(&l);
    glClear(GL_COLOR_BUFFER_BIT);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  fprintf(stdout,"Resizing window to size %ix%i\n",width,height);
  glViewport(0,0,width,height);
}


void process_input(GLFWwindow* window)
{
  if(glfwGetKey(window,GLFW_KEY_ESCAPE)==GLFW_PRESS)
  {
    fprintf(stdout,"Escape key pressed, exitting gracefully\n");
    glfwSetWindowShouldClose(window,1);
  }
  return;
}


void bounce_index(float * index)
{
  if(*index<=0.0)
    up_or_down = 0;
  if(*index>=1.0)
    up_or_down = 1;
  if(up_or_down == 0)
    *index+=0.01;
  else
    *index-=0.01;
  return;
}
