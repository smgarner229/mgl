#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>

//#include <GL/gl.h>
#include <GLFW/glfw3.h>

typedef struct rgb_bounce
{
  float val;
  int direction;
} rgb_bounce;

void init_rgb_bounce(rgb_bounce * rgb_val, float val, int direction);
void bounce_index(rgb_bounce * rgb_val);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(int argc, char ** argv)
{
  GLFWwindow * window;
  if(!glfwInit())
  {
    fprintf(stderr,"Could not open window!\n");
    return EXIT_FAILURE;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
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

  rgb_bounce r,g,b;
  init_rgb_bounce(&r,0.0,0);
  init_rgb_bounce(&g,0.25,0);
  init_rgb_bounce(&b,0.5,1);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);

    /* Render here */
    glClearColor(r.val,g.val,b.val,1.0);
    bounce_index(&r);
    bounce_index(&g);
    bounce_index(&b);
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

void init_rgb_bounce(rgb_bounce * rgb_val, float _val, int _direction)
{
  rgb_val->val = _val;
  rgb_val->direction = _direction;
  return;
}

void bounce_index(rgb_bounce * rgb_val)
{
  if(rgb_val->val<=0.0)
    rgb_val->direction = 0;
  if(rgb_val->val>=1.0)
    rgb_val->direction = 1;
  if(rgb_val->direction==0)
    rgb_val->val+=0.01;
  else
    rgb_val->val-=0.01;
  return;
}

