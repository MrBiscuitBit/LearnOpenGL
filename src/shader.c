#include "shader.h"

char *read_file(const char *path){
    LOG("Reading File To Buffer: (%s)...", path);

    FILE *file = fopen(path, "rb");
    if (!file) {
        ERR("Failed To Open File");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    rewind(file);

    if (length == 0) {
        WRN("File Is Empty");
    }

    char *buffer = (char *)malloc(length + 1);
    if(!buffer){
        ERR("Failed To Allocate Buffer For File\n");
        fclose(file);
        return NULL;
    }

    size_t read_size = fread(buffer, 1, length, file);
    if(read_size != length){
        WRN("File Read Mismatch: (Expected %ld, Result %zu)", length, read_size);
    }

    buffer[read_size] = '\0';
    fclose(file);
    LOG("File Read Successfully");
    return buffer;
}

u32 load_shader_program(const char *vertex_shader_source, const char *fragment_shader_source){

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
    if(!vertex_src){
        ERR("FAILED TO LOAD SHADER VERTEX SOURCE");
        return 0;
    }
    //LOG("Vertex Src:\n%s\n", vertex_src);
    u32 vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char * const*)&vertex_src, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex_shader, info_log_buffer_size, NULL, info_log);
        ERR("SHADER VERTEX COMPILATION_FAILED: %s", info_log);
        return 0;
    }
    free(vertex_src);

    char *frag_src = read_file(fragment_shader_source);
    if(!frag_src){
        ERR("FAILED TO LOAD SHADER FRAGMENT SOURCE");
        return 0;
    }
    //LOG("Fragment Src:\n%s\n", frag_src);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char * const*)&frag_src, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader, info_log_buffer_size, NULL, info_log);
        ERR("SHADER FRAGMENT COMPILATION_FAILED: %s", info_log);
        glDeleteShader(fragment_shader);
        return 0;
    }
    free(frag_src);

    u32 shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program, info_log_buffer_size, NULL, info_log);
        ERR("SHADER PROGRAM LINKING_FAILED: %s", info_log);
        glDeleteShader(vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteProgram(shader_program);
        return 0;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    return shader_program;
}