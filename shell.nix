{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    gcc
    cmake
    libGL
    libglvnd
    glfw3
    glfw
    glm
    clang-tools
  ];
}
