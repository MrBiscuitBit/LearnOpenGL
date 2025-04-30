#include <shader.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void window_size_callback(GLFWwindow *window, int width, int height);
void process_input(GLFWwindow *window, float *mix_factor);

int log_enabled = 0;

int main(int argc, char **argv){

    int error_code = 0;

#ifdef DEBUG
    printf("  Hello Debug Build\n\n");
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
        ERR("ERROR::GLFW Init Failed. Extiting Program..");
        return ++error_code;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int wnd_width = 800, wnd_height = 600;
    GLFWwindow *window = glfwCreateWindow(wnd_width, wnd_height, "LearnOpenGL", NULL, NULL);
    if(!window){
        ERR("ERROR::GLFW Create Window Failed. Extiting Program..");
        glfwTerminate();
        return ++error_code;
    }
    glfwMakeContextCurrent(window);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        ERR("ERROR::Failed To Initialize GLAD. Exiting Program..");
        glfwDestroyWindow(window);
        glfwTerminate();
        return ++error_code;
    }

    glViewport(0, 0, wnd_width, wnd_height);
    glfwSetWindowSizeCallback(window, window_size_callback);

    u32 shader_program = load_shader_program("resrcs\\shaders\\default.vert", "resrcs\\shaders\\default.frag");

    u32 tex_wood;
    glGenTextures(1, &tex_wood);
    glBindTexture(GL_TEXTURE_2D, tex_wood);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    int tex_width, tex_height, nr_channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char *data = stbi_load("resrcs\\textures\\container.jpg", &tex_width, &tex_height, &nr_channels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        ERR("Failed To Load Texture Wood");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    u32 tex_smile;
    glGenTextures(1, &tex_smile);
    glBindTexture(GL_TEXTURE_2D, tex_smile);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("resrcs\\textures\\awesomeface.png", &tex_width, &tex_height, &nr_channels, 0);
    if(data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex_width, tex_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        ERR("Failed To Load Texture Smile");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    u32 indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    u32 VBO, VAO, EBO;
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glUseProgram(shader_program);
    glUniform1i(glGetUniformLocation(shader_program, "texture_one"), 0);
    glUniform1i(glGetUniformLocation(shader_program, "texture_two"), 1);
    int mix_factor_location = glGetUniformLocation(shader_program, "mix_factor");
    glUseProgram(0);



    // Render Loop
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    float mix_factor = 0.2f;

    int running = 1;
    while(!glfwWindowShouldClose(window) && running != 0){
        process_input(window, &mix_factor);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shader_program);

        glUniform1f(mix_factor_location, mix_factor);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex_wood);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex_smile);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glUseProgram(0);

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

void process_input(GLFWwindow *window, float *mix_factor){

    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS){
        *mix_factor += 0.0005f;
        if(*mix_factor >= 1.0f) *mix_factor = 1.0f;
    }
    else if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS){
        *mix_factor -= 0.0005f;
        if(*mix_factor <= 0.0f) *mix_factor = 0.0f;
    }

}