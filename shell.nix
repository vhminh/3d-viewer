{ pkgs ? import <nixpkgs> {} }:
pkgs.mkShell {
  nativeBuildInputs = with pkgs.buildPackages; [
    gcc
    cmake
    libGL
    libGL.dev
    glfw3
    glfw
    glm
    clang-tools
  ];
  buildInputs = with pkgs; [
      glfw3
      glfw
  ];
}
