#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <glad/glad.h>

typedef struct shader_program{
    char * vertex_shader;
    char * fragment_shader;
    unsigned int ID;
} shader_program;

void init_shader_program(shader_program * prog, const char * vs, const char * fs);
char * loadfile(const char * file_name);
void delete_shader_program(shader_program * prog);
void check_compile_errors(unsigned int shader, const char * type);
void use_shader_program(shader_program * prog);
