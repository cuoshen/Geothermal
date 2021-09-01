# Geothermal

Geothermal is an open-source game engine written in C++ / DirectX 11. With a focus on 3D rendering techniques, the engine serves rather as a graphics programming framework than a general purpose game engine.

## How to build

### Windows

- Currently, Windows 10 Desktop is the only officially supported platform

Since the build system is managed entirely by Visual Studio, you may want to build with Visual Studio.

Install Visual Studio 2019 and cppwinrt plugin ([C++/WinRT - Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=CppWinRTTeam.cppwinrt101804264)), then open the solution in Visual Studio. You may need nuget package directxtk_desktop_2017, all other third-party components are already included in the repo.

After all is done, hit the big green button to fire it up :)

![image](docs/big%20green%20button.png)

## Features

### Deferred shading pipeline

Geothermal engine supports a simple forward shading path and a simple deferred shading path.

The deferred shading path uses a GBuffer set with 2 render targets and a depth target.

![image](docs/GBuffer%200.png)

![image](docs/GBuffer%201.png)



### Albedo and normal mapping

![image](docs/normal%20mapped%20earth.png)



### Real-time shadow mapping

![image](docs/dragon%20with%20shadow.png)

![image](docs/city%20with%20shadow%201.png)



### AABB based frustum culling

![image](docs/AABB.png)
