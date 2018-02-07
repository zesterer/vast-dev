# Vast
---

A rather large space sim.

## Design

Vast uses a voxel system to construct physical objects such as asteroids, spacecraft, space stations and vehicles. The game is 1st person and allows the player to move independently of their spacecraft.

## Building

Vast makes use of the C++17 standard and requires a C++17-compliant compiler (recent GCC, MSVC and Clang versions are compliant).

Vast requires the following libraries to build:

- glm
- glbinding
- glfw
- SOIL
- OpenGL

If you are on a platform other than Linux, you will need to edit the "target" variable in the root makefile.

Building Vast is simple. From the root directory, simply run `make build` to build the project, and `make run` to run it.