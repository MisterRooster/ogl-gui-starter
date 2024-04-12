![Header](preview.png)

# ogl-gui-starter

Description:
------------
This application is a simple OpenGL window using GLFW3 with imgui embedded as graphical user interface.
It's purpose is to be used as a starting point for OpenGL projects that
need a gui.
A few useful helper classes for rendering, file reading/writing etc. are provided.

> **Note**:
Support for OpenGL 4.3 core profile is required.

## How to Compile
Clone the repo:
```bash
git clone git://github.com/MisterRooster/ogl-gui-starter.git
cd ogl-gui-starter 
```

Then simply run CMake with your favourite IDE. Tested with Visual Studio 2022,
no working guarantee for other environments.
Example commands for VS 2022:
```bash
mkdir build
cd build
cmake -G "Visual Studio 17"..
```

-----
Copyright (c) 2022 MisterRooster ([github.com/MisterRooster](https://github.com/MisterRooster)). All rights reserved.  
Licensed under the MIT license. See [LICENSE](LICENSE) for full terms.