#include "3dmodel/model.h"
#include "app/3dviewer.h"
#include "app/config.h"
#include "app/spinning_duck_app.h"
#include "stb_image.h"

int main() {

	/* SpinningDuckApp app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/home/minh/Downloads/New_Sponza/New_Sponza_001.gltf"); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/Users/mvu/Downloads/New_Sponza/New_Sponza_001.gltf"); */
	X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
	              "/Users/mvu/Downloads/uploads_files_2465920_burger_merged.glb");

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
