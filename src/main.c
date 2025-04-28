#include <stdio.h>
#include <string.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void window_size_callback(GLFWwindow *window, int width, int height);
void process_input();

int main(int argc, char **argv){

    printf("Hello OpenGL\n");

    if(glfwInit() == GLFW_FALSE){
        printf("ERROR::GLFW Init Failed. Extiting Program..\n");
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int wnd_width = 800, wnd_height = 600;
    GLFWwindow *window = glfwCreateWindow(wnd_width, wnd_height, "LearnOpenGL", NULL, NULL);
    if(!window){
        printf("ERROR::GLFW Create Window Failed. Extiting Program..\n");
        glfwTerminate();
        return 2;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("ERROR::Failed To Initialize GLAD. Exiting Program..\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return 3;
    }

    glViewport(0, 0, wnd_width, wnd_height);
    glfwSetWindowSizeCallback(window, window_size_callback);

    int success;
    int info_log_buffer_size = 512;
    char info_log[info_log_buffer_size];

    const char *vertex_shader_source = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "out vec4 vertex_color;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos, 1.0f);\n"
        "   vertex_color = vec4(0.5f, 0.0f, 0.0f, 1.0f);\n"
        "}\0";
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex_shader, info_log_buffer_size, NULL, info_log);
        printf("ERROR::SHADER VERTEX COMPILATION_FAILED: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    const char *fragment_shader_source =
        "#version 330 core\n"
        "in vec4 vertex_color;\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "   FragColor = vertex_color;\n"
        "}\0";
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
    glCompileShader(fragment_shader);
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader, info_log_buffer_size, NULL, info_log);
        printf("ERROR::SHADER FRAGMENT COMPILATION_FAILED: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    unsigned int shader_program = glCreateProgram();
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

    float vertices[] = {
       -0.5f, -0.5f, 0.0f,
        0.0f,  0.5f, 0.0f,
        0.5f, -0.5f, 0.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // render loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    while(!glfwWindowShouldClose(window)){
        process_input(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

void window_size_callback(GLFWwindow *window, int width, int height){
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }

}