# Raytracer

A basic (but modular) raytracer written in C++.

> [!NOTE]
> This project can run on web platform, or even render image for a VR headset
> but doing so will greatly reduce the rendering speed

## Usage

```shell
$ ./raytracer scenes/default.cfg 
$ /raytracer <SCENE_FILE> [-w|--watch] [-r|--renderer <SDL|PPM>] [-t|--threads <auto|off|1-32>] [-h|--help]
	<SCENE_FILE>: scene configuration
	[-w|--watch]: scene configuration auto-reload
	[-r|--renderer <SDL|PPM|SFML>]: renderer selection (default: 'PPM')
	[-t|--threads <auto|off|1-32>]: multi-threading selection -> auto/off/specific thread count (default: 'auto')
	[-h|--help]: Show this help page./raytracer <SCENE_FILE>
```

## Build

Build yourself the project will you need to meet the following requirements:
* CMake >= 3.1O
* libconfig++
* SFML 2.6.x

### Justfile

```shell
just release
```

### CMake

```shell
cmake -S . -B build
cmake --build build -j
```
