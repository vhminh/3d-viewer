#pragma once

#include <GLFW/glfw3.h>
#include <tuple>
#include <utils/macro.h>

class App {
public:
	NO_COPY(App);
	App(const char* title, int width, int height);
	virtual bool should_close() const;
	virtual void process_input(float dt) = 0;
	virtual void update(float dt) = 0;
	virtual void clear() const;
	virtual void render() = 0;
	virtual void poll_event() const;
	int get_width() const;
	int get_height() const;
	std::tuple<int, int> get_size() const;
	virtual ~App();

protected:
	GLFWwindow* window = nullptr;
};
