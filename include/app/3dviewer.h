#pragma once

#include "3dmodel/scene.h"
#include "app/app.h"
#include "app/camera.h"
#include "glwrapper/shader.h"

#include <vector>

class X3DViewer : public App {
public:
	NO_COPY(X3DViewer);
	X3DViewer(const char* title, int width, int height, const std::vector<const char*>& paths);
	virtual void process_input(float dt) override;
	virtual void update(float dt) override;
	virtual void render() override;

private:
	Camera camera;
	Shader shader;
	Scene scene;

	double update_fps_countdown = 1.0;
	int frame_count = 0;

	glm::dvec2 last_cursor_pos;
	bool has_last_cursor = false;
	void process_camera_keyboard_input(float dt);
	void process_camera_mouse_input(float dt);
};
