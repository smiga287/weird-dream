# Weird dream
- Computer Graphics project at Faculty of Mathematics, University of Belgrade
- [Video Overview]()
- Project was bootstrapped using the [project tempalte](https://github.com/matf-racunarska-grafika/project_base)

# Implemented

- [x] Cubemaps
- [x] Instancing
- [x] HDR, Bloom

# Controls

- `f1` toggles the ImGui menu
- `wasd` - usual movement keys

# Requirements

- `C++20` compatible compiler
- `CMake >= 3.11`
- `Ninja` or `Make`
- All resources are included in the `resources/` directory, which is why cloning will probably take a bit more time

# Installation and usage

```bash
$ git clone https://github.com/smiga287/weird-dream.git
$ cd weird-dream
$ mkdir build && cd build 
$ cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja .. # Ninja is not mandatory, but is preferred
$ ninja # alternatively use make
$ cd ..
$ ./weird_dream
```
