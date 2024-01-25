# Fluid Simulation

## Introduction
The aim of this project is to create a fluid simulation from scratch using C++ and OpenGL. This project uses the [Lagrangian fluid mechanics](https://en.wikipedia.org/wiki/Lagrangian_and_Eulerian_specification_of_the_flow_field), using a system of particles. 
## Acknowledgements
- [Setting up OpenGL](https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&si=h-WkK5zo02fcNIEF)
- [Fluid simulation Implementation in C#](https://youtu.be/rSKMYc1CQHE?si=uEoApdGKHVgMOW8-)  
## API References
This project uses [OpenGL](https://www.opengl.org/) and all its relevant modules. Documentation can be found on [docs.GL](https://docs.gl). Usage of other API such as Vulkan was considered but OpenGL was chosen due to prior experience.
## Required Libraries
These libraries are necessary to run the program. If these libraries already exist within your file hierarchy, skip the download step. 
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
We need to link the libraries and vendors to the source code. This is IDE dependent, so please refer to your IDE on linking libraries and include directories. This project was developed using Visual Studio 2022, and so, the example images are shown from VS2022.

***Note*** : By default, the folders containing the vendors are within the folder named 'vendor'. Change property directories as needed.
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
The performance of this application varies on different hardware. Do make sure to adjust settings when necessary. Ideal framerate should be minimum of 30FPS.

Running the program on debug vs release does have a significant performance difference. Be sure to run the program in 'Release' mode. 

If running on a laptop, use performance mode and/or plug laptop to charger.
Doing so can increase framerate up by 6x.

There are further optimizations to be made, and updates will be noted here
## Authors
@Astralye


