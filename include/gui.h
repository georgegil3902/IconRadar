#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "RadarData.h"

// Function to initialize ImGui
void initializeGui(GLFWwindow *window);

// Function to render ImGui interface
void renderGui(int width, int height, cv::Mat &radarImage);

// Function to convert BGR OpenCV Image to RGB OpenGL texture
GLuint matToTexture(const cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);

// Function to cleanup ImGui
void cleanupGui();

#endif // IMGUIINTERFACE_H
