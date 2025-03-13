#include "app/3dviewer.h"

#include "app/config.h"

#include <GLFW/glfw3.h>
#include <iostream>

X3DViewer::X3DViewer(const char* title, int width, int height, const char* model_path)
	: App(title, width, height),
	  shader(Shader::create("resource/shader/3d_viewer.v.glsl", "resource/shader/3d_viewer.f.glsl")),
	  camera(glm::vec3(0.0f, 1.0f, 1.0f), 0.0, 0.0), model(model_path) {
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void X3DViewer::process_camera_keyboard_input(float dt) {
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

void X3DViewer::process_camera_mouse_input(float dt) {
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

void X3DViewer::process_input(float dt) {
	if (GLFW_TRUE != glfwGetWindowAttrib(window, GLFW_FOCUSED)) {
		return;
	}
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
		try {
			shader = Shader::create("resource/shader/3d_viewer.v.glsl", "resource/shader/3d_viewer.f.glsl");
			std::cerr << "reload shader sucess" << std::endl;
		} catch (const std::exception& ex) {
			std::cerr << "error reloading shader " << ex.what() << std::endl;
		}
	}
	process_camera_keyboard_input(dt);
	process_camera_mouse_input(dt);
}

void X3DViewer::update(float dt) {
	update_fps_countdown -= dt;
	frame_count += 1;
	if (update_fps_countdown < 0.0) {
		std::string str_fps = std::string("3D viewer - FPS: ") + std::to_string(frame_count);
		glfwSetWindowTitle(window, str_fps.c_str());
		update_fps_countdown = 1.0;
		frame_count = 0;
	}
}

void X3DViewer::render() {
	model.render(shader, camera);
	glfwSwapBuffers(window);
}
