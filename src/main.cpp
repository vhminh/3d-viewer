#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "app/UserApp.h"
#include "app/config.h"

int main() {

    UserApp app("Learn OpenGL", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

    double prev_time = glfwGetTime();

    while (!app.should_close()) {
        double current_time = glfwGetTime();
        double time_elapsed = current_time - prev_time;
        prev_time = current_time;

        app.process_input(time_elapsed);
        app.update(time_elapsed);
        app.clear();
        app.render();
        app.poll_event();
    }
    return EXIT_SUCCESS;
}

