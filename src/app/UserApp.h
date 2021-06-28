#pragma once
#include "App.h"
#include "../glwrapper/shader.h"
#include "../glwrapper/VertexArray.h"
#include "../glwrapper/texture.h"
#include "camera.h"
#include <glm/glm.hpp>

class UserApp : public App {
public:
    UserApp(const std::string& title, int width, int height);
    virtual void process_input(float dt);
    virtual void update(float dt);
    virtual void render() const;
private:
    Camera camera;
    ShaderProgram shader_program;
    VertexArray ground;
    VertexArray cube;
    Texture duck, grass;
    unsigned vert_arr_id;
    glm::vec3 cam_pos;
    glm::vec3 cam_center;

    double update_fps_countdown = 1.0;
};