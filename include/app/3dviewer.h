#pragma once

#include "3dmodel/model.h"
#include "app/app.h"
#include "app/camera.h"
#include "glwrapper/shader.h"

class X3DViewer : public App {
public:
	NO_COPY(X3DViewer);
	X3DViewer(const char* title, int width, int height, const char* model_path);
	virtual void process_input(float dt) override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
	Camera camera;
	Shader shader;
	Shader light_shader;
	Model model;

	double update_fps_countdown = 1.0;
	int frame_count = 0;

	glm::dvec2 last_cursor_pos;
	bool has_last_cursor = false;
	void process_camera_keyboard_input(float dt);
	void process_camera_mouse_input(float dt);
};
