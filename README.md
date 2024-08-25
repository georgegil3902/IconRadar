# IconRadar

## Overview

IconRadar is an interactive image processing application that uses radar detection data to visualize and analyze the environment. The application leverages OpenCV for image manipulation and ImGui for the graphical user interface, with OpenGL handling the rendering. Users can load radar data from a CSV file, visualize it as an image, and apply edge detection filters in real-time.

## Features

- Load and visualize radar data from a CSV file.
- Real-time edge detection with adjustable thresholds.
- Responsive GUI using ImGui.
- Cross-platform support (Windows, macOS, Linux).

> Note : The software as of now is only capable of opening one csv files kept in its assets folder with a particular name. The feature to open any csv files from a relative location that comprises of radar data is under progress

## Requirements

Before setting up the project, ensure you have the following installed on your system:

- **CMake** (version 3.10 or higher)
- **GLFW** (Graphics Library Framework)
- **GLAD** (OpenGL Loader)
- **OpenCV** (Open Source Computer Vision Library)
- **ImGui** (for GUI)
- **OpenGL** (for rendering)
- **C++17** compatible compiler (e.g., MSVC on Windows, GCC or Clang on Linux/macOS)

## Setup Instructions

### 1. Clone the Repository

To begin, clone the project from the Git repository and navigate to the project directory:

```bash
git clone https://github.com/georgegil3902/IconRadar.git
cd IconRadar
```

Create a new folder named vendor and navigate to that folder

```bash
mkdir vendor
cd vendor
```

---

### 2. Setting Up GLAD

- Visit the [GLAD Configuration Page](https://glad.dav1d.de/)
- Configure the GLAD Loader

| Configurations |              |
| -------------- | ------------ |
| Language       | C/C++        |
| Specification  | OpenGL       |
| API(gl)        | Version 3.3  |
| Compatibility  | Core         |
| Extensions     | Not required |

- Generate and Download
  - Click on the Generate button.
  - Download the glad.zip file once the generation is complete.
- Extract and Organize Files
  - Extract the contents of glad.zip.
  - Place the extracted files into the IconRadar/vendor/ directory within your project.

---

### 3. GLFW Setup

Visit the [GLFW Github Repository](https://github.com/glfw/glfw) and clone the repository into IconRadar/vendor/

```bash
git clone https://github.com/glfw/glfw.git
```

---

### 4: OpenCV Setup

Visit the [OpenCV website](https://opencv.org/) and navigate to the [releases page](https://opencv.org/releases/)

- Select Windows to download an executable file
- Extract the executable file and place it in IconRadar/vendor/

---

### 5: ImGui Setup

Visit the [ImGui Github Repository](https://github.com/ocornut/imgui) and clone the repository into IconRadar/vendor/ with renaming it as imgui

```bash
git clone https://github.com/ocornut/imgui.git imgui
```

If you downloaded it as a zip file, extract it into IconRadar/vendor and rename `imgui-master` folder to `imgui`

## Building the project

> Note : You build and install your project in one step using the build.bat file or if you wish to, you can set it up yourself by following the steps below

### Quick Method : Go back to the main directory and run the build.bat file

```bash
cd ..
.\build.bat
```

### Manual Method : Follow the steps below to setup the project manually

Go back to the project's root folder
Create a folder named `build` in the main directory and navigate to it

```bash
cd ..
mkdir build
cd build
```

Then run the following commands to build the project.

```bash
cmake .. -DCMAKE_INSTALL_PREFIX=../build/install
cmake --build .
```

To install the project within your build directory run the following code

```bash
cmake --install . --config Debug
```

> Note : A system wide release installation is currently not supported. Make sure to use `cmake --install .` with the `--config Debug flag`

## Lets run the project

The executable file is in build/install/bin folder

- You can run it directly with this command

```bash
.\install\bin\IconRadar.exe
```

- Or you can navigate to the install directory and run the executable by double clicking it

> Note : Your Working software is comprised within the install directory. Hence you can copy or move the entire install directory to relocate your working software
