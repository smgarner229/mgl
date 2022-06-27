#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

int main(int argc, char ** argv)
{
  if(!glfwInit())
  {
    fprintf(stderr,"glfwInit failed.  Exitting\n");
    return EXIT_FAILURE;
  }

  // Setup the window
  GLFWwindow * window;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

  int num_monitors;
  GLFWmonitor ** my_monitors = glfwGetMonitors(&num_monitors);
  fprintf(stdout,"Number of monitors is: %i\n",num_monitors); 
  
  fprintf(stdout,"Monitor name is: %s\n",glfwGetMonitorName(*my_monitors));

  int width,height;
  glfwGetMonitorPhysicalSize(*my_monitors,&width,&height);
  fprintf(stdout,"Physical size is: %ix%i\n",width,height);

  float xscale,yscale;
  glfwGetMonitorPos(*my_monitors,&width,&height);
  fprintf(stdout,"Monitor position is: %i,%i\n",width,height);
//  glfwGetMonitorContentScale(my_monitors, &xscale, &yscale);
//  glfwGetMonitorContentScale(*my_monitors,&xscale,&yscale);
//  fprintf(stdout,"x & y scales are %f,%f\n",xscale,yscale);

  glfwTerminate();
  return EXIT_SUCCESS;
}
