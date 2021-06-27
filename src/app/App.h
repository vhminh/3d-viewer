#pragma once

#include <string>
#include <tuple>
#include <GLFW/glfw3.h>

class App {
public:
    App(const std::string& title, int width, int height);
    virtual bool should_close() const;
    virtual void post_contruct();
    virtual void process_input(float dt) = 0;
    virtual void update(float dt) = 0;
    virtual void clear() const;
    virtual void render() const = 0;
    virtual void poll_event() const;
    int get_width() const;
    int get_height() const;
    std::tuple<int, int> get_size() const;
    virtual ~App();
protected:
    GLFWwindow* window = nullptr;
};