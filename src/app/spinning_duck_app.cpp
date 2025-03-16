#include "app/spinning_duck_app.h"

#include "app/config.h"
#include "gl3.h"
#include "glwrapper/texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

SpinningDuckApp::SpinningDuckApp(const char* title, int width, int height) :
	App(title, width, height), camera(glm::vec3(0.0f, 1.0f, 1.0f), 0.0, 0.0),
	shader(Shader::create("resource/shader/spinning_duck.v.glsl", "resource/shader/spinning_duck.f.glsl")),
	light_shader(Shader::create("resource/shader/spinning_duck.v.glsl", "resource/shader/white.f.glsl")),
	duck(Texture::create("resource/texture/duck.jpg", TextureType::BASE_COLOR)),
	grass(Texture::create("resource/texture/grass.png", TextureType::BASE_COLOR)) {

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

void SpinningDuckApp::process_camera_keyboard_input(float dt) {
	glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 control_dir = glm::vec3(0.0f, 0.0f, 0.0f);
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		control_dir.z -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		control_dir.z += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		control_dir.x -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		control_dir.x += 1.0f;
	}
	if (glm::length(control_dir) < EPSILON) {
		// no control input
		return;
	}
	control_dir = glm::normalize(control_dir);
	glm::vec3 direction = camera.get_direction();
	glm::vec3 move_vec = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 facing_dir_2D = glm::vec3(direction.x, 0.0, direction.z); // facing direction without the y-axis
	if (glm::length(facing_dir_2D) < EPSILON) {
		// looking at the sky or the ground
		return;
	}
	facing_dir_2D = glm::normalize(facing_dir_2D);
	move_vec += glm::normalize(glm::cross(facing_dir_2D, up)) * control_dir.x * CAMERA_SPEED * dt;
	move_vec += -facing_dir_2D * control_dir.z * CAMERA_SPEED * dt;
	camera.origin += move_vec;
}

void SpinningDuckApp::process_camera_mouse_input() {
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	glm::dvec2 cur_cursor_pos = glm::dvec2(x, y);
	if (!has_last_cursor) {
		has_last_cursor = true;
		last_cursor_pos = cur_cursor_pos;
		return;
	}

	glm::dvec2 diff = cur_cursor_pos - last_cursor_pos;
	camera.yaw += CAMERA_ROTATION_SPEED * diff.x;
	camera.pitch -= CAMERA_ROTATION_SPEED * diff.y;
	camera.pitch = glm::clamp(camera.pitch, -45.0, 45.0);

	last_cursor_pos = cur_cursor_pos;
}

void SpinningDuckApp::process_input(float dt) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	process_camera_keyboard_input(dt);
	process_camera_mouse_input();
}

void SpinningDuckApp::update(float dt) {
	update_fps_countdown -= dt;
	frame_count += 1;
	if (update_fps_countdown < 0.0) {
		std::string str_fps = std::string("3D viewer - FPS: ") + std::to_string(frame_count);
		glfwSetWindowTitle(window, str_fps.c_str());
		update_fps_countdown = 1.0;
		frame_count = 0;
	}
}

void SpinningDuckApp::render() {

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
	shader.setUniformVec3("camera_origin", camera.origin);

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
}
