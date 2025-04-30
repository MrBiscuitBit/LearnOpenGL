#include <shader.h>

void window_size_callback(GLFWwindow *window, int width, int height);
void process_input();

int log_enabled = 0;

int main(int argc, char **argv){

    int error_code = 0;

#ifdef DEBUG
    printf("Hello Debug Build\n\n");
    if(argc > 1){
        for(int i = 1; i < argc; i++){
            char *arg = argv[i];
            if(strcmp(arg, "--log") == 0){
                log_enabled = 1;
                LOG("Log Enabled!");
            }
            else{
                printf("Usage: .\\%s --log\n", argv[0]);
                return ++error_code;
            }
        }
    }
#endif

    if(glfwInit() == GLFW_FALSE){
        printf("ERROR::GLFW Init Failed. Extiting Program..\n");
        return ++error_code;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int wnd_width = 800, wnd_height = 600;
    GLFWwindow *window = glfwCreateWindow(wnd_width, wnd_height, "LearnOpenGL", NULL, NULL);
    if(!window){
        printf("ERROR::GLFW Create Window Failed. Extiting Program..\n");
        glfwTerminate();
        return ++error_code;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        printf("ERROR::Failed To Initialize GLAD. Exiting Program..\n");
        glfwDestroyWindow(window);
        glfwTerminate();
        return ++error_code;
    }

    glViewport(0, 0, wnd_width, wnd_height);
    glfwSetWindowSizeCallback(window, window_size_callback);

    uint32 shader_program = load_shader_program("resrcs\\vertex_shader.txt", "resrcs\\fragment_shader.txt");

    float vertices[] = {
       -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.0f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
    };

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // Render Loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    int running = 1;
    while(!glfwWindowShouldClose(window) && running != 0){
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

    glDeleteProgram(shader_program);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

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