cmake_minimum_required(VERSION 3.10)
project(IconRadar)

# Set the C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)

# Define the path to your local OpenCV installation
set(OpenCV_DIR "${CMAKE_SOURCE_DIR}/vendor/opencv/build")

# Find OpenCV package
find_package(OpenCV REQUIRED)


# Find OpenGL package
find_package(OpenGL REQUIRED COMPONENTS OpenGL)

# Include OpenCV headers
include_directories(${OpenCV_INCLUDE_DIRS})

# Include other directories (include headers)
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add GLAD
add_library(glad vendor/glad/src/glad.c)
target_include_directories(glad PUBLIC vendor/glad/include)

# Add GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
add_subdirectory(vendor/glfw)

# Add ImGui and ensure it knows where to find GLFW
add_library(imgui
    vendor/imgui/imgui.cpp
    vendor/imgui/imgui_draw.cpp
    vendor/imgui/imgui_tables.cpp
    vendor/imgui/imgui_widgets.cpp
    vendor/imgui/backends/imgui_impl_glfw.cpp
    vendor/imgui/backends/imgui_impl_opengl3.cpp
)
target_include_directories(imgui PUBLIC vendor/imgui vendor/imgui/backends vendor/glfw/include)

# Add the source files
add_executable(IconRadar
    src/main.cpp
    src/RadarData.cpp
    src/gui.cpp
)

# Link libraries
target_link_libraries(IconRadar
    ${OpenCV_LIBS}
    glfw
    glad
    imgui
    OpenGL::GL
)

# Installation rules
install(TARGETS IconRadar
    RUNTIME DESTINATION bin  # This installs the executable to the 'bin' directory
)

# Copy the OpenCV DLLs to the output directory
set(OPENCV_DLL_DIR "${OpenCV_DIR}/x64/vc16/bin")
install(DIRECTORY "${OpenCV_DIR}/x64/vc16/bin/"
    DESTINATION bin
    FILES_MATCHING PATTERN "*.dll")

install(DIRECTORY "${CMAKE_SOURCE_DIR}/assets/"
    DESTINATION assets
    FILES_MATCHING PATTERN "*.csv")
# install(FILES
#     "${OPENCV_DLL_DIR}/opencv_world4100d.dll"
#     DESTINATION bin
# )