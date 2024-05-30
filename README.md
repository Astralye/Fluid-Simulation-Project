# Fluid Simulation

## Introduction
The aim of this project is to create a fluid simulation from scratch using C++ and OpenGL. This project uses the [Lagrangian fluid mechanics](https://en.wikipedia.org/wiki/Lagrangian_and_Eulerian_specification_of_the_flow_field), using a system of particles. 
## Acknowledgements
- [Setting up OpenGL](https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&si=h-WkK5zo02fcNIEF)
- [Fluid simulation Implementation in C#](https://youtu.be/rSKMYc1CQHE?si=uEoApdGKHVgMOW8-)  
- [Implementation of SPH](https://sph-tutorial.physics-simulation.org/pdf/SPH_Tutorial.pdf)
- [IMGUI web documentation](https://pthom.github.io/imgui_manual_online/manual/imgui_manual.html)

## Bugs
There are known bugs within the program which did not have enough time to fix which might cause unexpected behaviour.
- Unlikely random crashes. Not exactly sure, but could be memory issue
- Cannot run on Debug mode, due to IM_GUI only being able to run one menu window.
- SPH performance does not show up. Implementation and std::chrono::duration requirements.
- Particle modification tab. Was not fully implemented.
- Hover Particle info does not align with particle. Likely due to high velocities.   

## API References
This project uses [OpenGL](https://www.opengl.org/) and all its relevant modules. Documentation can be found on [docs.GL](https://docs.gl). Usage of other API such as Vulkan was considered but OpenGL was chosen due to prior experience.
## Required Libraries
These libraries are necessary to run the program. These are included with the program code, but feel free to delete them if you have them pre-installed locally.
### GLFW
GLFW is a library which allows us to create windows for OpenGL.
[Website](https://www.glfw.org/), [Github Repo](https://github.com/glfw/glfw)
### GLEW
GLEW is a library that allows the GPU the ability to utilize newer version of OpenGL code. 
[Website](https://glew.sourceforge.net/), [Github Repo](https://github.com/nigels-com/glew)

## Vendors
### GLM
GLM is a mathematics library. This allows us to not worry about implementation of abstract mathematical systems
[Website](https://glm.g-truc.net/0.9.9/index.html), [Github Repo](https://github.com/g-truc/glm)

### IMGUI
IMGUI is a library for creating GUI within our program.
[Github Repo](https://github.com/ocornut/imgui)

### STB_IMAGE
STB_IMAGE is a library for converting images into textures.
[Github Repo](https://github.com/nothings/stb)
## Linking
The libraries should be already linked being stored in the project settings.

### Include
To include the use the header files, we need to include the directory. 

On VS2022, we can find this on the 'properties' menu of the project:
![Image1](IncludeDir.png)

The directories to include are:
```
$(ProjectDir)
$(SolutionDir)vendor
$(SolutionDir)Dependencies\GLEW\include
$(SolutionDir)Dependencies\GLFW\include
```
The code uses macros to make the directories relative. You can create the file structure as shown or modify the directory to own system. The 'vendor' folder includes all folders of the aforementioned vendors.

***Note*** : GLM and STB_IMAGE are **not** included within the project. Dear IMGUI requires files to be within the project so make sure to include it. By default, the location is within 'vendor'. To change the location, you have to remove and readd the files to the project. 
### Library
In VS2022, you can find the library directory as follows
![LibraryLinker](LibraryLinker.png)
```
$(SolutionDir)Dependencies\GLEW\lib\Release\x64
$(SolutionDir)Dependencies\GLFW\lib-vc2022
```
This code is relative to solution directory. Make sure to change this if Library is located elsewhere.

![LibFiles](LibFiles.png)
```
glfw3.lib
opengl32.lib
glew32s.lib
```
Dependencies can be copied as they are the same file.
## Performance
The application is currently CPU heavy. Ideally it should be GPU heavy due to parallel processing but for this version, uses CPU threads for its parallelizing.

## Authors
@Astralye


