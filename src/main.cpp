#include "app/config.h"
#include "app/userapp.h"
#include "stb_image.h"

#include "3dmodel/model.h"

int main() {

	UserApp app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	/* Model model("/home/minh/Downloads/New_Sponza/New_Sponza_001.gltf"); */
	/* Model model("/Users/mvu/Downloads/New_Sponza/New_Sponza_001.gltf"); */

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
