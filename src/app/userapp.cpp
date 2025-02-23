#include "app/userapp.h"

#include "app/config.h"
#include "gl3.h"
#include "glwrapper/texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

UserApp::UserApp(const std::string& title, int width, int height)
	: App(title, width, height),
	  shader(Shader::create("resource/shader/simple.v.glsl", "resource/shader/simple.f.glsl")),
	  light_shader(Shader::create("resource/shader/simple.v.glsl", "resource/shader/light-source.f.glsl")),
	  duck(Texture::create("resource/texture/duck.jpg")), grass(Texture::create("resource/texture/grass.png")),
	  camera(window, glm::vec3(0.0f, 1.0f, 1.0f), 0.0, 0.0) {

	cam_pos = glm::vec3(0.0f, 0.0f, 1.0f);
	cam_center = glm::vec3(0.0, 0.0, 0.0);

	glGenVertexArrays(1, &cube_vao);
	glGenVertexArrays(1, &ground_vao);
	glGenVertexArrays(1, &light_vao);
	// cube
	float vertices[] = {
		// clang-format off
		// vert           // texture // normal
		-0.5, -0.5,  0.5, 0.0, 0.0,  0.0, 0.0, 1.0, // front
		-0.5,  0.5,  0.5, 0.0, 1.0,  0.0, 0.0, 1.0,
		 0.5,  0.5,  0.5, 1.0, 1.0,  0.0, 0.0, 1.0,
		
		 0.5,  0.5,  0.5, 1.0, 1.0,  0.0, 0.0, 1.0,
		 0.5, -0.5,  0.5, 1.0, 0.0,  0.0, 0.0, 1.0,
		-0.5, -0.5,  0.5, 0.0, 0.0,  0.0, 0.0, 1.0,
		
		 0.5, -0.5, -0.5, 0.0, 0.0,  0.0, 0.0, -1.0, // back
		 0.5,  0.5, -0.5, 0.0, 1.0,  0.0, 0.0, -1.0,
		-0.5,  0.5, -0.5, 1.0, 1.0,  0.0, 0.0, -1.0,
		
		-0.5,  0.5, -0.5, 1.0, 1.0,  0.0, 0.0, -1.0,
		-0.5, -0.5, -0.5, 1.0, 0.0,  0.0, 0.0, -1.0,
		 0.5, -0.5, -0.5, 0.0, 0.0,  0.0, 0.0, -1.0,
		
		-0.5,  0.5,  0.5, 0.0, 0.0,  0.0, 1.0, 0.0, // up
		-0.5,  0.5, -0.5, 0.0, 1.0,  0.0, 1.0, 0.0,
		 0.5,  0.5, -0.5, 1.0, 1.0,  0.0, 1.0, 0.0,
		
		 0.5,  0.5, -0.5, 1.0, 1.0,  0.0, 1.0, 0.0,
		 0.5,  0.5,  0.5, 1.0, 0.0,  0.0, 1.0, 0.0,
		-0.5,  0.5,  0.5, 0.0, 0.0,  0.0, 1.0, 0.0,
		
		-0.5, -0.5, -0.5, 0.0, 0.0,  0.0, -1.0, 0.0, // bottom
		-0.5, -0.5,  0.5, 0.0, 1.0,  0.0, -1.0, 0.0,
		 0.5, -0.5,  0.5, 1.0, 1.0,  0.0, -1.0, 0.0,
		
		 0.5, -0.5,  0.5, 1.0, 1.0,  0.0, -1.0, 0.0,
		 0.5, -0.5, -0.5, 1.0, 0.0,  0.0, -1.0, 0.0,
		-0.5, -0.5, -0.5, 0.0, 0.0,  0.0, -1.0, 0.0,
		
		-0.5, -0.5, -0.5, 0.0, 0.0,  -1.0, 0.0, 0.0, // left
		-0.5,  0.5, -0.5, 0.0, 1.0,  -1.0, 0.0, 0.0,
		-0.5,  0.5,  0.5, 1.0, 1.0,  -1.0, 0.0, 0.0,
		
		-0.5,  0.5,  0.5, 1.0, 1.0,  -1.0, 0.0, 0.0,
		-0.5, -0.5,  0.5, 1.0, 0.0,  -1.0, 0.0, 0.0,
		-0.5, -0.5, -0.5, 0.0, 0.0,  -1.0, 0.0, 0.0,

		 0.5, -0.5,  0.5, 0.0, 0.0,  1.0, 0.0, 0.0,// right
		 0.5,  0.5,  0.5, 0.0, 1.0,  1.0, 0.0, 0.0,
		 0.5,  0.5, -0.5, 1.0, 1.0,  1.0, 0.0, 0.0,

		 0.5,  0.5, -0.5, 1.0, 1.0,  1.0, 0.0, 0.0,
		 0.5, -0.5, -0.5, 1.0, 0.0,  1.0, 0.0, 0.0,
		 0.5, -0.5,  0.5, 0.0, 0.0,  1.0, 0.0, 0.0,
		// clang-format on
	};
	glBindVertexArray(cube_vao);
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// ground
	float ground_vertices[] = {
		// clang-format off
		// vert             // texture  // normal
		-10.0,  0.0,  10.0,  0.0,  0.0, 0.0, 1.0, 0.0,
		-10.0,  0.0, -10.0,  0.0, 10.0, 0.0, 1.0, 0.0,
		 10.0,  0.0, -10.0, 10.0, 10.0, 0.0, 1.0, 0.0,
		
		 10.0,  0.0, -10.0, 10.0, 10.0, 0.0, 1.0, 0.0,
		 10.0,  0.0,  10.0, 10.0,  0.0, 0.0, 1.0, 0.0,
		-10.0,  0.0,  10.0,  0.0,  0.0, 0.0, 1.0, 0.0,
		// clang-format on
	};
	glBindVertexArray(ground_vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ground_vertices), ground_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

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
	glBindVertexArray(light_vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(light_vertices), light_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
	glEnableVertexAttribArray(0);
}

void UserApp::process_input(float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
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

void UserApp::render() {

	shader.use();

	double time = glfwGetTime();

	glm::vec3 light_pos(cos(time) * 1.25, 1.25, sin(time) * 1.5);

	// ------ draw cube
	glm::mat4 model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, glm::vec3(0.0, 1.0, 0.0));
	model_mat = glm::rotate(model_mat, glm::radians(float(sin(time + 1) * 90.0f)), glm::vec3(1.0f, 0.0f, 0.0f));
	shader.setUniformMat4("model_mat", model_mat);
	shader.setUniformMat4("view_mat", camera.get_view_matrix());

	const glm::mat4 projection_mat =
		glm::perspective(glm::radians(45.0), DEFAULT_WINDOW_WIDTH * 1.0 / DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
	shader.setUniformMat4("projection_mat", projection_mat);

	shader.setUniformVec3("light_color", glm::vec3(0.75f, 1.0f, 0.7f));
	shader.setUniformVec3("light_pos", light_pos);
	shader.setUniformVec3("camera_origin", camera.get_origin());

	glBindVertexArray(cube_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, duck.get_id());
	shader.setUniformTexture("tex", 0);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);

	// ------ draw ground
	model_mat = glm::mat4(1.0);
	shader.setUniformMat4("model_mat", model_mat);

	glBindVertexArray(ground_vao);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, grass.get_id());
	shader.setUniformTexture("tex", 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ------ draw light source
	light_shader.use();

	model_mat = glm::mat4(1.0);
	model_mat = glm::translate(model_mat, light_pos);
	model_mat = glm::scale(model_mat, glm::vec3(0.25f, 0.25f, 0.25f));
	light_shader.setUniformMat4("model_mat", model_mat);
	light_shader.setUniformMat4("view_mat", camera.get_view_matrix());
	light_shader.setUniformMat4("projection_mat", projection_mat);

	glBindVertexArray(light_vao);
	glDrawArrays(GL_TRIANGLES, 0, 3 * 2 * 6);
	// ------ swap buffer
	glfwSwapBuffers(window);
	glBindVertexArray(0);
}
