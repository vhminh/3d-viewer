#include "app/app.h"

#include "util/error.h"

void on_framebuffer_resize(GLFWwindow* window, int new_width, int new_height) {
	glViewport(0, 0, new_width, new_height);
}

App::App(const char* title, int width, int height) {
	// Init GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window
	window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	if (window == nullptr) {
		throw WindowCreationException("Failed to create GLFW window");
	}
	glfwMakeContextCurrent(window);

	// View port
	int fb_width, fb_height;
	glfwGetFramebufferSize(window, &fb_width, &fb_height);
	glViewport(0, 0, fb_width, fb_height);

	glfwSetFramebufferSizeCallback(window, on_framebuffer_resize);

	glEnable(GL_DEPTH_TEST);
}

App::~App() { glfwTerminate(); }

int App::get_width() const {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return w;
}

int App::get_height() const {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return h;
}

std::tuple<int, int> App::get_size() const {
	int w, h;
	glfwGetWindowSize(window, &w, &h);
	return std::make_tuple(w, h);
}

bool App::should_close() const { return glfwWindowShouldClose(window); }

void App::clear() const {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void App::poll_event() const { glfwPollEvents(); }
