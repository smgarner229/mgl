#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

#include "my_c_shader_library.h"

/*
    Initializes the glfw library
    If this does not work, close the program
*/
void initialize_glfw(void)
{
    if(!glfwInit())
    {
        fprintf(stderr,"Could not initialize glfw!\nExitting.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

#define GLFW_MAJOR_VERSION 3
#define GLFW_MINOR_VERSION 3
#define RESIZABLE GLFW_TRUE
void glfw_window_hints()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,GLFW_MAJOR_VERSION);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,GLFW_MINOR_VERSION);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE,RESIZABLE);
}

#define DEFAULT_WINDOW_WIDTH 800
#define DEFAULT_WINDOW_HEIGHT 600
void initialize_window(GLFWmonitor ** monitor, GLFWwindow ** window)
{
    *monitor = glfwGetPrimaryMonitor();
    if(!*monitor)
    {
        fprintf(stderr,"Couldn't access a monitor!\nExitting.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    *window = glfwCreateWindow(DEFAULT_WINDOW_WIDTH,DEFAULT_WINDOW_HEIGHT,"Pi Estimation",*monitor,NULL);
    if(!*window)
    {
        fprintf(stderr,"Couldn't open window!\nExitting.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

void process_input(GLFWwindow * win)
{
    if(glfwGetKey(win,GLFW_KEY_ENTER)==GLFW_PRESS)
    {
        fprintf(stdout,"Enter released, exitting gracefully\n");
        glfwSetWindowShouldClose(win,1);
    }
    return;
}

void load_glad(void)
{
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        fprintf(stderr,"Couldn't initialize GLAD library!\nExitting.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char ** argv)
{
    initialize_glfw();
    glfw_window_hints();

    GLFWmonitor* monitor;
    GLFWwindow * window;

    initialize_window(&monitor,&window);

    glfwMakeContextCurrent(window);

    load_glad();

    shader_program * my_program = malloc(sizeof(shader_program));
    // init_shader_program(my_program,"v.glsl","f.glsl");

    int counter = 0;

    float half = 0.5;
    float mhalf = -0.5;

    float quad[20]={
        half,mhalf,0,1,-1,
        half,half,0,1,1,
        mhalf,half,0,-1,1,
        mhalf,mhalf,0,-1,-1    
    };

    GLuint glBuffer;
    glGenBuffers(1,&glBuffer);
    glBindBuffer(GL_ARRAY_BUFFER,glBuffer);
    glBufferData(GL_ARRAY_BUFFER,sizeof(float)*20,quad,GL_STATIC_DRAW);

    shader_program * my_prog = malloc(sizeof(shader_program));
    init_shader_program(my_prog,"circle.vs","circle.fs");

    GLuint vtex = glGetAttribLocation(my_prog->ID,"vertex");
    GLuint val = glGetAttribLocation(my_prog->ID,"value");

    glEnableVertexAttribArray(vtex);
    glEnableVertexAttribArray(val);

    glVertexAttribPointer(vtex,3,GL_FLOAT,GL_FALSE,20,(GLvoid*)0);
    glVertexAttribPointer(val,2,GL_FLOAT,GL_FALSE,20,(GLvoid*)12);

    glUseProgram(my_prog->ID);

    glDrawArrays(GL_TRIANGLE_FAN,0,4);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

    while(counter++ < 100 && !glfwWindowShouldClose(window))
    {
        process_input(window);
        glClearColor(0.2f,0.3f,0.3f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();

    // delete_shader_program(my_program);
    free(my_program);

    return EXIT_SUCCESS;
}