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
    int log_buffer_size = 512;
    char info_log[log_buffer_size];
    // Create Vertex Shader
    const char *vertex_shader_source = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);"
        "}\0";
    unsigned int vertex_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    // Compile Vertex Shader
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glCompileShader(vertex_shader);
    // Check For Compilation Errors
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(vertex_shader, log_buffer_size, NULL, info_log);
        printf("ERROR::COMPILATION_FAILED, SHADER VERTEX: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    // Create Fragment Shaders
    const char *fragment_shader_source_orange =
        "#version 330 core\n"
        "out vec4 FragColor;"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";
    unsigned int fragment_shader_orange;
    fragment_shader_orange = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile Fragment Shader
    glShaderSource(fragment_shader_orange, 1, &fragment_shader_source_orange, NULL);
    glCompileShader(fragment_shader_orange);
    // Check For Compilation Errors
    glGetShaderiv(fragment_shader_orange, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader_orange, log_buffer_size, NULL, info_log);
        printf("ERROR::COMPILATION_FAILED, SHADER FRAGMENT: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    const char *fragment_shader_source_yellow =
        "#version 330 core\n"
        "out vec4 FragColor;"
        "void main()\n"
        "{\n"
        "   FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);\n"
        "}\0";
    unsigned int fragment_shader_yellow;
    fragment_shader_yellow = glCreateShader(GL_FRAGMENT_SHADER);
    // Compile Fragment Shader
    glShaderSource(fragment_shader_yellow, 1, &fragment_shader_source_yellow, NULL);
    glCompileShader(fragment_shader_yellow);
    // Check For Compilation Errors
    glGetShaderiv(fragment_shader_yellow, GL_COMPILE_STATUS, &success);
    if(!success){
        glGetShaderInfoLog(fragment_shader_yellow, log_buffer_size, NULL, info_log);
        printf("ERROR::COMPILATION_FAILED, SHADER FRAGMENT: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    //Create Shader Programs
    unsigned int shader_program_orange;
    shader_program_orange = glCreateProgram();
    // Link Shaders To Program
    glAttachShader(shader_program_orange, vertex_shader);
    glAttachShader(shader_program_orange, fragment_shader_orange);
    glLinkProgram(shader_program_orange);
    //Check For Linking Errors
    glGetProgramiv(shader_program_orange, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program_orange, log_buffer_size, NULL, info_log);
        printf("ERROR::LINKING_FAILED PROGRAM SHADER ORANGE: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    unsigned int shader_program_yellow;
    shader_program_yellow = glCreateProgram();
    // Link Shaders To Program
    glAttachShader(shader_program_yellow, vertex_shader);
    glAttachShader(shader_program_yellow, fragment_shader_yellow);
    glLinkProgram(shader_program_yellow);
    //Check For Linking Errors
    glGetProgramiv(shader_program_yellow, GL_LINK_STATUS, &success);
    if(!success){
        glGetProgramInfoLog(shader_program_yellow, log_buffer_size, NULL, info_log);
        printf("ERROR::LINKING_FAILED PROGRAM SHADER YELLOW: %s\n", info_log);
        memset(info_log, '\0', sizeof(info_log));
    }

    // Cleanup Shader Objects
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader_orange);
    glDeleteShader(fragment_shader_yellow);

    float triangle_left[] = {
        -0.5f, -0.25f, 0.0f,
        -0.25f, 0.25f, 0.0f,
         0.0f, -0.25f, 0.0f,
    };
    float triangle_right[] = {
         0.0f, -0.25f, 0.0f,
         0.25f, 0.25f, 0.0f,
         0.5f, -0.25f, 0.0f 
    };

    unsigned int VBOs[2], VAOs[2];
    glGenBuffers(2, VBOs);
    glGenVertexArrays(2, VAOs);

    // Bind Triangle Left
    glBindVertexArray(VAOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_left), triangle_left, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind Triangle Right
    glBindVertexArray(VAOs[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_right), triangle_right, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind Stuff
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // render loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    while(!glfwWindowShouldClose(window)){
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render Triangle Left
        glUseProgram(shader_program_orange);
        glBindVertexArray(VAOs[0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // Render Triangle Right
        glUseProgram(shader_program_yellow);
        glBindVertexArray(VAOs[1]);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glUseProgram(0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteVertexArrays(2, VAOs);
    glDeleteBuffers(2, VBOs);
    glDeleteProgram(shader_program_orange);
    glDeleteProgram(shader_program_yellow);

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