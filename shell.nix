{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    gcc
    cmake
    gdb
    libGL
    libglvnd
    glfw3
    glfw
    glm
    zlib
    clang-tools
  ];
}
