#pragma once
#include "app.h"
#include "camera.h"
#include "glwrapper/shader.h"
#include "glwrapper/texture.h"
#include "util/macro.h"

#include <glm/glm.hpp>

class UserApp : public App {
public:
	NO_COPY(UserApp);
	UserApp(const std::string& title, int width, int height);
	virtual void process_input(float dt);
	virtual void update(float dt);
	virtual void render();

private:
	Camera camera;
	GLuint cube_vao;
	GLuint light_vao;
	GLuint ground_vao;
	Shader shader;
	Shader light_shader;
	Texture duck, grass;
	unsigned vert_arr_id;
	glm::vec3 cam_pos;
	glm::vec3 cam_center;

	double update_fps_countdown = 1.0;
	int frame_count = 0;
};
