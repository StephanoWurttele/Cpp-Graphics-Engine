# C++ Graphics Engine

## Build Dependencies ##
- [CMAKE](http://www.cmake.org/)
- [GLEW](http://glew.sourceforge.net/)
- [GLFW](https://www.glfw.org/)
- [ASSIMP](http://assimp.sourceforge.net/)
- [OpenGL](https://www.opengl.org/)
- [SOIL](http://www.lonesock.net/soil.html)

## Build System ##
- Linux/MacOS -> cmake, make & g++ 
- Window -> Coming Soon - Not yet tested.

### Basic Instructions ###
```shell
# Ubuntu
sudo apt install cmake libglew-dev libglfw3-dev libglfw3 libassimp-dev

# Arch Linux
sudo pacman -S cmake glew glfw-x11 assimp

# MacOS
brew install cmake glew glfw3 assimp
```

### Running ###
```shell
# Compile
make

# Run
./run
```

### In-Progress ###
- Cascading Shadows
- Deferred Rendering
- ECS
