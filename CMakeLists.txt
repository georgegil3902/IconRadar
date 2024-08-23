cmake_minimum_required(VERSION 3.10)
project(IconRadar)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Define the path to your local OpenCV installation
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/vendor/opencv/build")

# Find OpenCV package
find_package(OpenCV REQUIRED)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Include other directories
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add the source files
add_executable(IconRadar
    src/main.cpp
    src/imgui.cpp
    src/imgui_draw.cpp
    src/imgui_widgets.cpp
    src/imgui_tables.cpp
    src/imgui_impl_glfw.cpp
    src/imgui_impl_opengl3.cpp
)

# Link libraries
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
target_link_libraries(IconRadar
    ${OpenCV_LIBS}
    glfw
    OpenGL::GL
)

# Installation rules
install(TARGETS IconRadar
    RUNTIME DESTINATION bin  # This installs the executable to the 'bin' directory
)

install(DIRECTORY ${OpenCV_DIR}/x64/vc16/bin/
    DESTINATION bin          # This copies the OpenCV DLLs to the 'bin' directory
    FILES_MATCHING PATTERN "*.dll"  # Only copy .dll files
)
