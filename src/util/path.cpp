#include "utils/path.h"

std::string directory_of(const char* path) {
	std::string directory = std::string(path);
	directory.erase(directory.find_last_of('/'));
	return directory;
}
