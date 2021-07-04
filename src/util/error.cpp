#include "error.h"

FileNotFoundException::FileNotFoundException(const std::string &path) : file_path(path) {
    if (path.length() > 0) {
        msg = std::string("Cannot open file \"" + path + "\"");
    }
}

const std::string& FileNotFoundException::get_file_path() {
    return file_path;
}

const char* FileNotFoundException::what() const noexcept {
    return msg.c_str();
}

ImageLoadingException::ImageLoadingException(const std::string &path) : file_path(path) {
    if (path.length() > 0) {
        msg = std::string("Cannot load image file \"" + path + "\"");
    }
}

const std::string& ImageLoadingException::get_file_path() {
    return file_path;
}

const char* ImageLoadingException::what() const noexcept {
    return msg.c_str();
}

ShaderCompilationException::ShaderCompilationException(const std::string &msg) : msg(msg) {}

const char* ShaderCompilationException::what() const noexcept {
    return msg.c_str();
}

ProgramLinkageException::ProgramLinkageException(const std::string &msg) : msg(msg) {}

const char* ProgramLinkageException::what() const noexcept {
    return msg.c_str();
}

WindowCreationException::WindowCreationException(const std::string &msg) : msg(msg) {}

const char* WindowCreationException::what() const noexcept {
    return msg.c_str();
}

