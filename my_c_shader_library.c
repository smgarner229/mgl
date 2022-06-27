#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <glad/glad.h>
#include <cglm/cglm.h>

#include "my_c_shader_library.h"

static char * loadfile(const char * file_name)
{
    FILE * fp = NULL;
    long lSize;
    char * buffer = NULL;

    fp=fopen(file_name,"rb");
    if(!fp){perror(file_name);}

    fseek(fp,0L,SEEK_END);
    lSize=ftell(fp);
    rewind(fp);

    buffer = malloc(lSize+1);
    if(!buffer){fclose(fp);fprintf(stderr,"Couldn't malloc memory in load file\n");}
    if(1!=fread(buffer,lSize,1,fp))
    {
        fclose(fp);
        fprintf(stderr,"Error in loadfile fread\n");
    }

    fclose(fp);
    return buffer;
};

void init_shader_program(shader_program * prog, const char * vs, const char * fs)
{
    prog->vertex_shader = strdup(vs);
    prog->fragment_shader = strdup(fs);

    unsigned int vertex;
    unsigned int fragment;

    vertex=glCreateShader(GL_VERTEX_SHADER);
    char * file_data = loadfile(vs);
    glShaderSource(vertex,1,(const char *const *)&file_data,NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex,"vertex");
    free(file_data);

    fragment=glCreateShader(GL_FRAGMENT_SHADER);
    file_data=loadfile(fs);
    glShaderSource(fragment,1,(const char * const *)&file_data,NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment,"fragment");
    free(file_data);

    prog->ID=glCreateProgram();
    glAttachShader(prog->ID,vertex);
    glAttachShader(prog->ID,fragment);
    glLinkProgram(prog->ID);
    check_compile_errors(prog->ID,"PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return;
};

void delete_shader_program(shader_program * prog)
{
    free(prog->vertex_shader);
    free(prog->fragment_shader);
    return;
};

void check_compile_errors(unsigned int shader, const char * type)
{
    int success;
    char  log[1024];
    if(0==strcmp(type,"PROGRAM"))
    {
        glGetProgramiv(shader,GL_LINK_STATUS,&success);
        if(!success)
        {
            glGetProgramInfoLog(shader,1024,NULL,log);
            fprintf(stderr,"Error in linking program.  Log below.\n%s",log);
        }
    }
    else
    {
        glGetShaderiv(shader,GL_COMPILE_STATUS,&success);
        if(!success)
        {
            glGetShaderInfoLog(shader,1024,NULL,log);
            fprintf(stderr,"Error in compiling %s program.  Log below.\n%s",type,log);
        }
    }
    return;
}

void use_shader_program(shader_program * prog)
{
    glUseProgram(prog->ID);
    return;
}
