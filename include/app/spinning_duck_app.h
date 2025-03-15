#pragma once
#include "app.h"
#include "camera.h"
#include "glwrapper/shader.h"
#include "glwrapper/texture.h"
#include "utils/macro.h"

#include <glm/glm.hpp>

class SpinningDuckApp : public App {
public:
	NO_COPY(SpinningDuckApp);
	SpinningDuckApp(const char* title, int width, int height);
	virtual void process_input(float dt) override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
	Camera camera;
	GLuint cube_vao;
	GLuint light_vao;
	GLuint ground_vao;
	Shader shader;
	Shader light_shader;
	Texture duck, grass;
	unsigned vert_arr_id;

	double update_fps_countdown = 1.0;
	int frame_count = 0;

	glm::dvec2 last_cursor_pos;
	bool has_last_cursor = false;
	void process_camera_keyboard_input(float dt);
	void process_camera_mouse_input(float dt);
};
