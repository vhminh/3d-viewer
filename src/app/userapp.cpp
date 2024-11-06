#include "app/userapp.h"

#include "app/config.h"
#include "glwrapper/texture.h"

#include "gl3.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

UserApp::UserApp(const std::string& title, int width, int height)
	: App(title, width, height),
	  shader_program(Shader(GL_VERTEX_SHADER, "res/shader/simple.vert"),
                     Shader(GL_FRAGMENT_SHADER, "res/shader/simple.frag")),
	  light_shader_program(
		  Shader(GL_VERTEX_SHADER, "res/shader/simple.vert"),
		  Shader(GL_FRAGMENT_SHADER, "res/shader/light-source.frag")),
	  duck("res/texture/duck.jpg"), grass("res/texture/grass.png"),
	  camera(window, glm::vec3(0.0f, 1.0f, 1.0f), 0.0, 0.0) {

	cam_pos = glm::vec3(0.0f, 0.0f, 1.0f);
	cam_center = glm::vec3(0.0, 0.0, 0.0);

	// cube
	float vertices[] = {
		// clang-format off
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
		// clang-format on
	};
	glBindVertexArray(cube.get_id());
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
	                      (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// ground
	float ground_vertices[] = {
		// clang-format off
		// vert             // texture
		-10.0,  0.0,  10.0,  0.0,  0.0,
		-10.0,  0.0, -10.0,  0.0, 10.0,
		 10.0,  0.0, -10.0, 10.0, 10.0,
		
		 10.0,  0.0, -10.0, 10.0, 10.0,
		 10.0,  0.0,  10.0, 10.0,  0.0,
		-10.0,  0.0,  10.0,  0.0,  0.0,
		// clang-format on
	};
	glBindVertexArray(ground.get_id());

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices,
	             GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
	                      (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// light
	float light_vertices[] = {
		// clang-format off
		-0.5, -0.5,  0.5, // front
		-0.5,  0.5,  0.5,
		 0.5,  0.5,  0.5,

		 0.5,  0.5,  0.5,
		 0.5, -0.5,  0.5,
		-0.5, -0.5,  0.5,
		
		 0.5, -0.5, -0.5, // back
		 0.5,  0.5, -0.5,
		-0.5,  0.5, -0.5,
		
		-0.5,  0.5, -0.5,
		-0.5, -0.5, -0.5,
		 0.5, -0.5, -0.5,
		
		-0.5,  0.5,  0.5, // up
		-0.5,  0.5, -0.5,
		 0.5,  0.5, -0.5,
		
		 0.5,  0.5, -0.5,
		 0.5,  0.5,  0.5,
		-0.5,  0.5,  0.5,
		
		-0.5, -0.5, -0.5, // bottom
		-0.5, -0.5,  0.5,
		 0.5, -0.5,  0.5,
		
		 0.5, -0.5,  0.5,
		 0.5, -0.5, -0.5,
		-0.5, -0.5, -0.5,
		
		-0.5, -0.5, -0.5, // left
		-0.5,  0.5, -0.5,
		-0.5,  0.5,  0.5,
		
		-0.5,  0.5,  0.5,
		-0.5, -0.5,  0.5,
		-0.5, -0.5, -0.5,
		
		 0.5, -0.5,  0.5, // right
		 0.5,  0.5,  0.5,
		 0.5,  0.5, -0.5,
		
		 0.5,  0.5, -0.5,
		 0.5, -0.5, -0.5,
		 0.5, -0.5,  0.5,
		// clang-format on
	};
	glBindVertexArray(light.get_id());

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices,
	             GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void UserApp::process_input(float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	camera.process_input(dt);
}

void UserApp::update(float dt) {
	update_fps_countdown -= dt;
	if (update_fps_countdown < 0.0) {
		float fps = 1.0 / dt;
		std::string str_fps = std::string("FPS: ") + std::to_string(fps);
		glfwSetWindowTitle(window, str_fps.c_str());
		update_fps_countdown = 1.0;
	}
}

void UserApp::render() const {

	shader_program.use();

	// ------ draw cube
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, glm::vec3(0.0, 1.0, 0.0));
	model_mat = glm::rotate(model_mat,
	                        glm::radians(float(sin(glfwGetTime() + 1) * 90.0f)),
	                        glm::vec3(1.0f, 0.0f, 0.0f));
	GLint loc = glGetUniformLocation(shader_program.get_id(), "model_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

	loc = glGetUniformLocation(shader_program.get_id(), "view_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
	                   glm::value_ptr(camera.get_view_matrix()));

	glm::mat4 projection_mat = glm::perspective(
		glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT,
		0.1, 100.0);
	loc = glGetUniformLocation(shader_program.get_id(), "projection_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection_mat));

	loc = glGetUniformLocation(shader_program.get_id(), "light_color");
	glUniform3fv(loc, 1, glm::value_ptr(glm::vec3(1.0f, 0.2f, 0.7f)));

	glBindVertexArray(cube.get_id());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, duck.get_id());
	glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex"), 0);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

	// ------ draw ground
	model_mat = glm::mat4(1.0);
	loc = glGetUniformLocation(shader_program.get_id(), "model_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));

	glBindVertexArray(ground.get_id());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, grass.get_id());
	glUniform1i(glGetUniformLocation(shader_program.get_id(), "tex"), 1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ------ draw light source
	light_shader_program.use();

	model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, glm::vec3(2.0, 1.5, 2.0));
	model_mat = glm::scale(model_mat, glm::vec3(0.5f, 0.5f, 0.5f));
	loc = glGetUniformLocation(light_shader_program.get_id(), "model_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(model_mat));
	loc = glGetUniformLocation(light_shader_program.get_id(), "view_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE,
	                   glm::value_ptr(camera.get_view_matrix()));
	loc = glGetUniformLocation(light_shader_program.get_id(), "projection_mat");
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(projection_mat));

	glBindVertexArray(light.get_id());
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);
	// ------ swap buffer
	glfwSwapBuffers(window);
	glBindVertexArray(0);
}
