#pragma once

#include <exception>
#include <string>

class FileNotFoundException : public std::exception {
public:
    FileNotFoundException(const std::string &path);
    const std::string& get_file_path();
    const char* what() const noexcept;
private:
    std::string file_path;
    std::string msg;
};

class ImageLoadingException : public std::exception {
public:
    ImageLoadingException(const std::string &path);
    const std::string& get_file_path();
    const char* what() const noexcept;
private:
    std::string file_path;
    std::string msg;
};

class ShaderCompilationException : public std::exception {
public:
    ShaderCompilationException(const std::string &msg);
    const char* what() const noexcept;
private:
    std::string msg;
};

class ProgramLinkageException : public std::exception {
public:
    ProgramLinkageException(const std::string &msg);
    const char* what() const noexcept;
private:
    std::string msg;
};

class WindowCreationException : public std::exception {
public:
    WindowCreationException(const std::string &msg);
    const char* what() const noexcept;
private:
    std::string msg;
};
