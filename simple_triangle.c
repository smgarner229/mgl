#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
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

  // Get the shaders together
  int success;
  char complog[512];

  unsigned int vertexShader;
  vertexShader=glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
  if(!success)
  {
    glGetShaderInfoLog(vertexShader,512,NULL,complog);
    printf("Error in compiling shader.\nError message printed below\n%s",complog);
  }
  

  unsigned int fragmentShader;
  fragmentShader=glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader,1,&fragmentShaderSource,NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader,GL_COMPILE_STATUS,&success);
  if(!success)
  {
    glGetShaderInfoLog(fragmentShader,512,NULL,complog);
    printf("Error in compiling fragment shader.\nError message printed below\n%s",complog);
  }

  // Link the shaders together
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram,vertexShader);
  glAttachShader(shaderProgram,fragmentShader);
  glLinkProgram(shaderProgram);
  glGetProgramiv(shaderProgram,GL_LINK_STATUS,&success);
  if(!success)
  {
    glGetProgramInfoLog(shaderProgram,512,NULL,complog);
    printf("Error in program.\nError message printed below\n%s",complog);
  }
//  glUseProgram(shaderProgram);
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  // Data for our triangle
//  int n_pts = 9;
//  float * verticies = (float*)malloc(sizeof(float)*n_pts);
//  verticies[0]=-0.5; verticies[1]=-0.5; verticies[2]=0.0;
//  verticies[3]=0.5; verticies[4]=-0.5; verticies[5]=0.0;
//  verticies[6]=0.0; verticies[7]=0.5; verticies[8]=0.0;

  // Data for our triangle
  float vertices[21] = {
      -0.5f, -0.5f, 0.0f, // left  
       0.5f, -0.5f, 0.0f, // right 
      -0.5f,  0.5f, 0.0f, // top
       0.5f,  0.5f, 0.0f,
       -1.0, -1.0, 0.0,
       -1.0, -0.75, 0.0,
       -0.75,-1.0,0.0
  }; 
  GLuint corners[9] = {
    3,1,2,1,0,2,4,5,6
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

  GLuint aPosIndex;
  aPosIndex=glGetAttribLocation(shaderProgram,"aPos");
  glEnableVertexAttribArray(aPosIndex);
  glVertexAttribPointer(aPosIndex,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
  glBufferData(GL_ARRAY_BUFFER,sizeof(GL_FLOAT)*21,vertices,GL_DYNAMIC_DRAW);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(corners),corners,GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER,0);
  //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
  glBindVertexArray(0);

  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    process_input(window);

    /* Render here */
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(*VAO);
    // vertices[0]+=0.005;
    // vertices[0] = (vertices[0] > 1.0 ? -1.0 : vertices[0]);
    //glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
    glDrawElements(GL_TRIANGLES,9,GL_UNSIGNED_INT,(GLvoid*)0);    
    glBindVertexArray(0);
    /* Swap front and back buffers */
    glfwSwapBuffers(window);
    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteBuffers(1,VBO);
  glDeleteBuffers(1,EBO);
  glDeleteVertexArrays(1,VAO);
  glDeleteProgram(shaderProgram);

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

