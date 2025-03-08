#include <string>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image.h"
#include "stb_image_resize2.h"
#include "stb_image_write.h"

#include <filesystem>
#include <iostream>

int main(int argc, char* argv[]) {
	assert(argc == 2);
	for (const auto& f : std::filesystem::recursive_directory_iterator(std::string(argv[1]))) {
		if (f.is_regular_file() && f.path().string().ends_with(".png")) {
			std::cout << "start " << f.path() << std::endl;
			int w, h, channels;
			unsigned char* data = stbi_load(f.path().c_str(), &w, &h, &channels, 0);
			std::cout << "w " << w << " h " << h << " channels " << channels << std::endl;
			stbir_pixel_layout layout;
			if (channels == 1) {
				layout = STBIR_1CHANNEL;
			} else if (channels == 2) {
				layout = STBIR_2CHANNEL;
			} else if (channels == 3) {
				layout = STBIR_RGB;
			} else if (channels == 4) {
				layout = STBIR_4CHANNEL;
			} else {
				assert(false);
			}
			unsigned char* resized = stbir_resize_uint8_linear(data, w, h, 0, nullptr, w / 2, h / 2, 0, layout);
			stbi_write_png(f.path().c_str(), w / 2, h / 2, channels, resized, 0);

			free(resized);
			stbi_image_free(data);
			std::cout << "done " << f.path() << std::endl;
		}
	}
	return 0;
}
