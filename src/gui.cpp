#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

void initializeGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    // ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core"); // Adjust GLSL version as needed
}

// void renderGui(const std::vector<RadarData> &radarData, int width, int height, cv::Mat &radarImage)
void renderGui(int width, int height, cv::Mat &radarImage)
{

    // Convert radarImage to an OpenGL texture
    static GLuint radarTexture = 0;

    if (radarTexture != 0)
    {
        glDeleteTextures(1, &radarTexture); // Delete the previous texture if it exists
    }

    radarTexture = matToTexture(radarImage, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set the clear color (optional, if not already set)
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear to black, adjust as needed

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Radar Controls");

    ImGui::Text("Radar Image:");
    ImGui::Image((void *)(intptr_t)radarTexture, ImVec2(radarImage.cols, radarImage.rows));

    static float edge_thresh = 0.5f;
    ImGui::SliderFloat("Edge Detection Threshold", &edge_thresh, 0.0f, 1.0f);

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLuint matToTexture(const cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
    GLuint textureID;

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error before glGenTextures: " << error << std::endl;
    }

    glGenTextures(1, &textureID);

    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glGenTextures: " << error << std::endl;
    }
    glBindTexture(GL_TEXTURE_2D, textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

    // Convert from BGR (OpenCV default) to RGB
    // cv::Mat rgb;
    // cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

    return textureID;
}

void cleanupGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
