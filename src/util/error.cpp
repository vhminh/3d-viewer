#include "utils/error.h"

#include <sstream>

FileNotFoundException::FileNotFoundException(const std::string& path) : file_path(path) {
	std::ostringstream ss;
	ss << "cannot open file \"" << path << "\"";
	msg = ss.str();
}

const std::string& FileNotFoundException::get_file_path() { return file_path; }

const char* FileNotFoundException::what() const noexcept { return msg.c_str(); }

ImageLoadingException::ImageLoadingException(const std::string& path) : file_path(path) {
	std::ostringstream ss;
	ss << "cannot load image file file \"" << path << "\"";
	msg = ss.str();
}

const std::string& ImageLoadingException::get_file_path() { return file_path; }

const char* ImageLoadingException::what() const noexcept { return msg.c_str(); }

ShaderCompilationException::ShaderCompilationException(const std::string& msg) : msg(msg) {}

const char* ShaderCompilationException::what() const noexcept { return msg.c_str(); }

ProgramLinkageException::ProgramLinkageException(const std::string& msg) : msg(msg) {}

const char* ProgramLinkageException::what() const noexcept { return msg.c_str(); }

WindowCreationException::WindowCreationException(const std::string& msg) : msg(msg) {}

const char* WindowCreationException::what() const noexcept { return msg.c_str(); }

ModelLoadingError::ModelLoadingError(const std::string& path, const std::string& assimp_msg)
	: path(path), assimp_msg(assimp_msg) {
	std::ostringstream ss;
	ss << "cannot load model file \"" << path << "\": " << assimp_msg;
	msg = ss.str();
}

const char* ModelLoadingError::what() const noexcept { return msg.c_str(); }
