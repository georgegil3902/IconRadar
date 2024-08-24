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
    ImGui_ImplOpenGL3_Init("#version 330 core"); // GLSL version
}

void renderGui(GLFWwindow *window, cv::Mat &radarImage)
{
    static GLuint radarTexture = 0;
    static int lastRadarImageUpdateFrame = -1;

    static float lowThresh = 50.0f;
    static float highThresh = 150.0f;

    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Get the framebuffer size to match the ImGui window size to the GLFW window
    // int framebufferWidth, framebufferHeight;
    // glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);

    // Set the ImGui window position to the top-left corner
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    // Set the ImGui window size to match the GLFW window size
    // ImGui::SetNextWindowSize(ImVec2((float)framebufferWidth, (float)framebufferHeight));

    // Calculate the height required for the ImGui window
    ImVec2 imageSize = ImVec2(radarImage.cols, radarImage.rows);
    float sliderHeight = ImGui::GetFrameHeightWithSpacing();     // Height of the slider widget with spacing
    float textHeight = ImGui::CalcTextSize("Radar Image:").y;    // Height of the text label
    float spacing = ImGui::GetStyle().ItemSpacing.y;             // Vertical spacing between elements
    float windowPadding = ImGui::GetStyle().WindowPadding.y * 2; // Window padding (top and bottom)

    float totalHeight = textHeight + spacing + imageSize.y + spacing + sliderHeight + spacing + sliderHeight + windowPadding;

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse;

    // Begin the ImGui window with the specified flags
    ImGui::SetNextWindowSize(ImVec2(imageSize.x, totalHeight)); // Set window size to fit the content
    ImGui::Begin("Canny Edge Detection", nullptr, window_flags);

    ImGui::SliderFloat("Low Threshold", &lowThresh, 0.0f, 255.0f);
    ImGui::SliderFloat("High Threshold", &highThresh, 0.0f, 255.0f);

    cv::Mat edges = applyCannyEdgeDetection(radarImage, lowThresh, highThresh);

    // Display the original radar image with edges overlaid
    cv::Mat displayImage;
    cv::cvtColor(edges, displayImage, cv::COLOR_GRAY2BGR);
    cv::addWeighted(radarImage, 1.0, displayImage, 0.4, 0, displayImage);

    // Update texture only when radarImage changes
    if (radarTexture == 0 || ImGui::GetFrameCount() != lastRadarImageUpdateFrame)
    {
        if (radarTexture != 0)
        {
            glDeleteTextures(1, &radarTexture); // Delete the previous texture if it exists
        }
        radarTexture = matToTexture(displayImage, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE);
        lastRadarImageUpdateFrame = ImGui::GetFrameCount();
    }

    ImGui::Text("Radar Image:");
    ImGui::Image((void *)(intptr_t)radarTexture, imageSize);

    ImGui::End();

    // Render ImGui and OpenGL draw data
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

GLuint matToTexture(const cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
    GLuint textureID;

    glGenTextures(1, &textureID);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glGenTextures: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glBindTexture: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glTexImage2D: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

    return textureID;
}

void cleanupGui()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}