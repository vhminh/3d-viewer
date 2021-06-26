#include <iostream>
#include <fstream>
#include <sstream>
#include <GLES3/gl3.h>
#include <GLFW/glfw3.h>
#include "util/error.h"

GLuint createProgram(const char* vertex_shader_file, const char* fragment_shader_file);
GLuint createShader(GLuint type, const char* source_file);
void on_framebuffer_resize(GLFWwindow* window, int new_width, int new_height);

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

int main() {
    // Init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Learn OpenGL", nullptr, nullptr);
    if (window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(window);

    // View port
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);

    // Make shader program
    GLuint program_id;
    program_id = createProgram("shader/simple.vert", "shader/simple.frag");
    glUseProgram(program_id);

    // Vertex array
    float vertices[] = {
        -0.5, -0.5, 0.0,
        0.5, -0.5, 0.0,
        0, 0.5, 0.0,
    };
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // Loop
    while (!glfwWindowShouldClose(window)) {
        // clear
        glClearColor(1, 1, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw 
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glBindVertexArray(0);

        glfwPollEvents();

    }
    glDeleteProgram(program_id);
    glfwTerminate();
    return EXIT_SUCCESS;
}

void on_framebuffer_resize(GLFWwindow* window, int new_width, int new_height) {
    glViewport(0, 0, new_width, new_height);
}

GLuint createShader(GLuint type, const char* source_file) {
    std::ifstream fin(source_file);
    if (!fin.is_open()) {
        throw FileNotFoundException(source_file);
    }
    std::string line;
    std::string file_content;
    while (getline(fin, line)) {
        file_content.append(line).append("\n");
    }
    const char* src = file_content.c_str();
    GLuint shader_id = glCreateShader(type);
    glShaderSource(shader_id, 1, &src, nullptr);
    glCompileShader(shader_id);
    int success;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char err_msg[512];
        glGetShaderInfoLog(shader_id, 512, NULL, err_msg);
        throw ShaderCompilationException(std::string("Error compile shader from source file \"") + source_file + "\": " + err_msg);
    }
    return shader_id;
}

GLuint createProgram(const char* vertex_shader_file, const char* fragment_shader_file) {
    GLuint vertex_shader = createShader(GL_VERTEX_SHADER, vertex_shader_file);
    GLuint fragment_shader = createShader(GL_FRAGMENT_SHADER, fragment_shader_file);
    GLuint program_id = glCreateProgram();
    glAttachShader(program_id, vertex_shader);
    glAttachShader(program_id, fragment_shader);
    glLinkProgram(program_id);
    int success;
    glGetProgramiv(program_id, GL_COMPILE_STATUS, &success);
    if (!success) {
        char err_msg[512];
        glGetProgramInfoLog(program_id, 512, NULL, err_msg);
        throw ProgramLinkageException(std::string("Error link program: ") + err_msg);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    return program_id;
}
