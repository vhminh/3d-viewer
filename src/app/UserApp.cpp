#include "UserApp.h"
#include <iostream>
#include <GLES3/gl3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../glwrapper/texture.h"
#include "config.h"

UserApp::UserApp(const std::string& title, int width, int height)
    : App(title, width, height),
    camera(window, glm::vec3(0.0f, 1.0f, 1.0f), 0.0, 0.0) {

    // init shader
    ShaderProgram tmp("shader/simple.vert", "shader/simple.frag");
    shader_program.move_from(tmp);
    shader_program.use();

    cam_pos = glm::vec3(0.0f, 0.0f, 1.0f);
    cam_center = glm::vec3(0.0, 0.0, 0.0);

    // cube
    float vertices[] = {
        // vert           // texture
        -0.5, -0.5,  0.5, 0.0, 0.0, // front
        -0.5,  0.5,  0.5, 0.0, 1.0,
         0.5,  0.5,  0.5, 1.0, 1.0,

         0.5,  0.5,  0.5, 1.0, 1.0,
         0.5, -0.5,  0.5, 1.0, 0.0,
        -0.5, -0.5,  0.5, 0.0, 0.0,

         0.5, -0.5, -0.5, 0.0, 0.0, // back
         0.5,  0.5, -0.5, 0.0, 1.0,
        -0.5,  0.5, -0.5, 1.0, 1.0,

        -0.5,  0.5, -0.5, 1.0, 1.0,
        -0.5, -0.5, -0.5, 1.0, 0.0,
         0.5, -0.5, -0.5, 0.0, 0.0,

        -0.5,  0.5,  0.5, 0.0, 0.0, // up
        -0.5,  0.5, -0.5, 0.0, 1.0,
         0.5,  0.5, -0.5, 1.0, 1.0,

         0.5,  0.5, -0.5, 1.0, 1.0,
         0.5,  0.5,  0.5, 1.0, 0.0,
        -0.5,  0.5,  0.5, 0.0, 0.0,

        -0.5, -0.5, -0.5, 0.0, 0.0, // bottom
        -0.5, -0.5,  0.5, 0.0, 1.0,
         0.5, -0.5,  0.5, 1.0, 1.0,

         0.5, -0.5,  0.5, 1.0, 1.0,
         0.5, -0.5, -0.5, 1.0, 0.0,
        -0.5, -0.5, -0.5, 0.0, 0.0,

        -0.5, -0.5, -0.5, 0.0, 0.0, // left
        -0.5,  0.5, -0.5, 0.0, 1.0,
        -0.5,  0.5,  0.5, 1.0, 1.0,

        -0.5,  0.5,  0.5, 1.0, 1.0,
        -0.5, -0.5,  0.5, 1.0, 0.0,
        -0.5, -0.5, -0.5, 0.0, 0.0, 

         0.5, -0.5,  0.5, 0.0, 0.0, // right
         0.5,  0.5,  0.5, 0.0, 1.0,
         0.5,  0.5, -0.5, 1.0, 1.0,
 
         0.5,  0.5, -0.5, 1.0, 1.0,
         0.5, -0.5, -0.5, 1.0, 0.0,
         0.5, -0.5,  0.5, 0.0, 0.0,
    };
    glBindVertexArray(cube.get_id());
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    Texture duck_tex("res/duck.jpg");
    duck.move_from(duck_tex);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // ground
    float ground_vertices[] = {
        // vert             // texture
        -10.0,  0.0,  10.0,  0.0,  0.0,
        -10.0,  0.0, -10.0,  0.0, 10.0,
         10.0,  0.0, -10.0, 10.0, 10.0,

         10.0,  0.0, -10.0, 10.0, 10.0,
         10.0,  0.0,  10.0, 10.0,  0.0,
        -10.0,  0.0,  10.0,  0.0,  0.0,
    };
    glBindVertexArray(ground.get_id());

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    
    Texture grass_tex("res/grass.png");
    grass.move_from(grass_tex);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void UserApp::process_input(float dt) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    camera.process_input(dt);
}

void UserApp::update(float dt) {

}

void UserApp::render() const {

    // ------ draw cube
    glm::mat4 model_mat = glm::mat4(1.0);
    model_mat = glm::translate(model_mat, glm::vec3(0.0, 1.0, 0.0));
    model_mat = glm::rotate(model_mat, glm::radians(float(sin(glfwGetTime() + 1) * 90.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
    GLint loc = glGetUniformLocation(shader_program.get_id(), "model_mat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

    loc = glGetUniformLocation(shader_program.get_id(), "view_mat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(camera.get_view_matrix()));

    glm::mat4 projection_mat = glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
    loc = glGetUniformLocation(shader_program.get_id(), "projection_mat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection_mat));

    glBindVertexArray(cube.get_id());
    glBindTexture(GL_TEXTURE_2D, duck.get_id());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

    // ------ draw ground
    model_mat = glm::mat4(1.0);
    loc = glGetUniformLocation(shader_program.get_id(), "model_mat");
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

    glBindVertexArray(ground.get_id());
    glBindTexture(GL_TEXTURE_2D, grass.get_id());
    glActiveTexture(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex"), 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    // ------ swap buffer
    glfwSwapBuffers(window);
    glBindVertexArray(0);
}
