# Raytracer

A basic (but modular) raytracer written in C++.

> [!NOTE]
> This project can run on web platform, or even render image for a VR headset
> but doing so will greatly reduce the rendering speed

## Usage

```shell
./raytracer scenes/default.cfg
./raytracer <SCENE_FILE>
```

## Build

Build yourself the project will you need to meet the following requirements:
* CMake >= 3.1O
* libconfig++

### Justfile

```shell
just release
```

### CMake

```shell
cmake -S . -B build
cmake --build build -j
```
