#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "my_c_shader_library.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 color;\n"
    "out vec4 vertexColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    // "   vertexColor = vec4((aPos.x+1.)/2.,(aPos.y+1.)/2.,(aPos.z+1.)/2.,1.0);\n"
    "   vertexColor = vec4(color.x,color.y,color.z,1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "in vec4 vertexColor;\n"
    "out vec4 FragColor;\n"
    // "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vertexColor;\n"
    "}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(int argc, char ** argv)
{
  // Initialize the glfw library
  if(!glfwInit())
  {
    fprintf(stderr,"Could not open window!\n");
    return EXIT_FAILURE;
  }

  // Give glfw Hints to configuration.
  // This will help glfw figure things out?
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
//  glfwWindowHint(GLFW_RESIZABLE,GLFW_TRUE);
  
  // Window declaration
  GLFWwindow * window;
  // Lets make it full screen lol
  GLFWmonitor* my_monitor = glfwGetPrimaryMonitor();
  window = glfwCreateWindow(800,600,"Hello There",NULL,NULL);
  if(!window)
  {
    fprintf(stdout,"Failed to initialize window.  Exitting\n");
    glfwTerminate();
    return EXIT_FAILURE;
  }
  
  // Make that context current
  glfwMakeContextCurrent(window);
 
  // Tell glfw what to do whenever we change the size of our frame
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

  // Load in our glad library
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    fprintf(stdout,"Failed to initialize GLAD library.  Exitting\n");
    return EXIT_FAILURE;
  }

  shader_program * my_prog = malloc(sizeof(shader_program));
  init_shader_program(my_prog,"v.txt","f.txt");

  // Data for our triangle
  float vertices[18] = {
      -0.9f, -0.9f, 0.0f, 1.0,0.0,0.0, // left  
       0.9f, -0.9f, 0.0f, 0.0,1.0,0.0,// right 
      -0.9f,  0.9f, 0.0f, 0.0,0.0,1.0,// top
  }; 
  GLuint corners[3] = {
    0,1,2
  };
  
  GLuint * VAO = (GLuint*)malloc(sizeof(GLuint));
  GLuint * VBO = (GLuint*)malloc(sizeof(GLuint));
  GLuint * EBO = (GLuint*)malloc(sizeof(GLuint));
  glGenVertexArrays(1,VAO);
  glGenBuffers(1,VBO);
  glGenBuffers(1,EBO);
  glBindVertexArray(*VAO);
  glBindBuffer(GL_ARRAY_BUFFER,*VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,*EBO);

  GLuint aPosIndex,colorIndex;
  aPosIndex=glGetAttribLocation(my_prog->ID,"aPos");
  colorIndex=glGetAttribLocation(my_prog->ID,"color");

  glEnableVertexAttribArray(aPosIndex);
  glEnableVertexAttribArray(colorIndex);

  glVertexAttribPointer(aPosIndex,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)0);
  glVertexAttribPointer(colorIndex,3,GL_FLOAT,GL_FALSE,6*sizeof(GL_FLOAT),(GLvoid*)(3*sizeof(GL_FLOAT)));
  
  glBufferData(GL_ARRAY_BUFFER,sizeof(GL_FLOAT)*18,vertices,GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(corners),corners,GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  glBindVertexArray(0);

  //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(my_prog->ID);
    glBindVertexArray(*VAO);
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,(GLvoid*)0);   

    glBindVertexArray(0);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteBuffers(1,VBO);
  glDeleteBuffers(1,EBO);
  glDeleteVertexArrays(1,VAO);
  glDeleteProgram(my_prog->ID);

  glfwTerminate();
  free(VAO);
  free(VBO);
  free(EBO);
  return EXIT_SUCCESS;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  fprintf(stdout,"Resizing window to size %ix%i\n",width,height);
  glViewport(0,0,width,height);
  return;
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

