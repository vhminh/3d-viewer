#include "3dmodel/model.h"
#include "app/3dviewer.h"
#include "app/config.h"
#include "app/spinning_duck_app.h"
#include "stb_image.h"

int main() {

	/* SpinningDuckApp app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/Users/mvu/Downloads/uploads_files_2465920_burger_merged.glb"); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/home/minh/projects/glTF-Sample-Assets/Models/Sponza/glTF/Sponza.gltf"); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/Users/mvu/Downloads/intel_sponza/NewSponza_Main_glTF_003.gltf"); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/home/minh/Downloads/intel_sponza/main1_sponza/NewSponza_Main_glTF_003.gltf"); */
	/* X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT, */
	/*               "/home/minh/Downloads/intel_sponza_downscaled/main1_sponza/NewSponza_Main_glTF_003.gltf"); */
	X3DViewer app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT,
	              "/Users/mvu/Downloads/intel_sponza_downscaled/NewSponza_Main_glTF_003.gltf");

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
