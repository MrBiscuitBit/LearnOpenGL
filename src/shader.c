#include "shader.h"

char* read_file(const char* path) {
    FILE* file = fopen(path, "r");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    rewind(file);

    char* buf = malloc(size + 1);
    if (!buf) { fclose(file); return NULL; }

    size_t read = fread(buf, 1, size, file);
    buf[read] = '\0';

    fclose(file);
    return buf;
}

uint32 load_shader_program(const char *vertex_shader_source, const char *fragment_shader_source){

    int error = 0;
    if(!vertex_shader_source){
        printf("ERROR::Missing Shader Source: Vertex\n");
        error++;
    }
    if(!fragment_shader_source){
        printf("ERROR::Missing Shader Source: Fragment\n");
        error++;
    }
    if(error > 0) return 0;

    int success, info_log_buffer_size = 512;
    char info_log[info_log_buffer_size];
    
    char *vertex_src = read_file(vertex_shader_source);
    uint32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char * const*)&vertex_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex_shader, info_log_buffer_size, NULL, info_log);
        printf("ERROR::SHADER VERTEX COMPILATION_FAILED: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }
    free(vertex_src);

    char *frag_src = read_file(fragment_shader_source);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char * const*)&vertex_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader, info_log_buffer_size, NULL, info_log);
        printf("ERROR::SHADER FRAGMENT COMPILATION_FAILED: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }
    free(frag_src);

    uint32 shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, info_log_buffer_size, NULL, info_log);
        printf("ERROR::SHADER PROGRAM LINKING_FAILED: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}