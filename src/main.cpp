#include "app/config.h"
#include "app/userapp.h"
#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <iostream>

int main() {

	UserApp app("3D viewer", DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT);

	/* Assimp::Importer importer; */
	/* const aiScene* scene = importer.ReadFile("/home/minh/Downloads/New_Sponza/New_Sponza_001.gltf", */
	/*                                          aiProcess_CalcTangentSpace | aiProcess_Triangulate | */
	/*                                              aiProcess_JoinIdenticalVertices | aiProcess_SortByPType); */
	/* std::cout << "lights: " << scene->HasLights() << " " << scene->mNumLights << std::endl; */
	/* std::cout << "cameras: " << scene->HasCameras() << " " << scene->mNumCameras << std::endl; */
	/* std::cout << "materials: " << scene->HasMaterials() << " " << scene->mNumMaterials << std::endl; */
	/* std::cout << "meshes: " << scene->HasMeshes() << " " << scene->mNumMeshes << std::endl; */
	/* std::cout << "animations: " << scene->HasAnimations() << " " << scene->mNumAnimations << std::endl; */

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
