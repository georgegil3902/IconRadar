#include "gui.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// Initializes the ImGui context and OpenGL/GLFW bindings for the GUI
// @param window: Pointer to the GLFW window that ImGui will render into
void initializeGui(GLFWwindow *window)
{
    IMGUI_CHECKVERSION();   // Ensure that the correct version of ImGui is being used
    ImGui::CreateContext(); // Create a new ImGui context
    // ImGui::StyleColorsDark(); // Uncomment to apply the dark theme to ImGui
    ImGui_ImplGlfw_InitForOpenGL(window, true);  // Initialize ImGui for GLFW
    ImGui_ImplOpenGL3_Init("#version 330 core"); // Initialize ImGui for OpenGL with specified GLSL version
}

// Renders the GUI, including radar image display and edge detection controls
// @param window: Pointer to the GLFW window (unused in this function but necessary for completeness)
// @param radarImage: The radar image (OpenCV Mat) to be displayed and processed
void renderGui(GLFWwindow *window, cv::Mat &radarImage)
{
    static GLuint radarTexture = 0;            // Texture ID for the radar image
    static int lastRadarImageUpdateFrame = -1; // Tracks the last frame when the radar image was updated

    // Parameters for Canny edge detection
    static float lowThresh = 50.0f;
    static float highThresh = 150.0f;
    static bool edgeDetectionEnabled = false; // Boolean to toggle Canny edge detection

    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // Set the ImGui window position to the top-left corner of the screen
    ImGui::SetNextWindowPos(ImVec2(0, 0));

    // Calculate the required height for the ImGui window based on content
    ImVec2 imageSize = ImVec2(radarImage.cols, radarImage.rows);
    float sliderHeight = ImGui::GetFrameHeightWithSpacing();     // Height of the slider widget with spacing
    float textHeight = ImGui::CalcTextSize("Radar Image:").y;    // Height of the text label
    float checkboxHeight = ImGui::GetFrameHeightWithSpacing();   // Height of the checkbox widget with spacing
    float spacing = ImGui::GetStyle().ItemSpacing.y;             // Vertical spacing between elements
    float windowPadding = ImGui::GetStyle().WindowPadding.y * 2; // Window padding (top and bottom)

    float totalHeight = checkboxHeight + spacing + textHeight + spacing + imageSize.y + spacing + sliderHeight + spacing + sliderHeight + windowPadding;

    // Window flags to remove title bar, resizing, moving, and collapsing capabilities
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
                                    ImGuiWindowFlags_NoResize |
                                    ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoCollapse;

    // Begin the ImGui window with the specified flags and size
    ImGui::SetNextWindowSize(ImVec2(imageSize.x, totalHeight)); // Set window size to fit the content
    ImGui::Begin("Canny Edge Detection", nullptr, window_flags);

    // Add sliders for adjusting Canny edge detection thresholds
    ImGui::SliderFloat("Low Threshold", &lowThresh, 0.0f, 255.0f);
    ImGui::SliderFloat("High Threshold", &highThresh, 0.0f, 255.0f);

    // Add a checkbox to enable or disable Canny edge detection
    ImGui::Checkbox("Enable Edge Detection", &edgeDetectionEnabled);

    cv::Mat displayImage;

    // Apply Canny edge detection if enabled
    if (edgeDetectionEnabled)
    {
        cv::Mat edges = applyCannyEdgeDetection(radarImage, lowThresh, highThresh);

        // Display the original radar image with edges overlaid
        cv::cvtColor(edges, displayImage, cv::COLOR_GRAY2BGR);                // Convert edges to BGR format
        cv::addWeighted(radarImage, 1.0, displayImage, 1.0, 0, displayImage); // Overlay edges onto the original image
    }
    else
    {
        // If Canny is disabled, just show the original radar image
        displayImage = radarImage.clone();
    }

    // Update texture only when radarImage changes
    if (radarTexture == 0 || ImGui::GetFrameCount() != lastRadarImageUpdateFrame)
    {
        if (radarTexture != 0)
        {
            glDeleteTextures(1, &radarTexture); // Delete the previous texture if it exists
        }
        radarTexture = matToTexture(displayImage, GL_LINEAR, GL_LINEAR, GL_CLAMP_TO_EDGE); // Convert Mat to OpenGL texture
        lastRadarImageUpdateFrame = ImGui::GetFrameCount();                                // Update the frame count for texture update tracking
    }

    // Display the radar image in the ImGui window
    ImGui::Text("Radar Image:");
    ImGui::Image((void *)(intptr_t)radarTexture, imageSize);

    ImGui::End(); // End the ImGui window

    // Render ImGui and OpenGL draw data
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// Converts an OpenCV Mat to an OpenGL texture
// @param mat: The OpenCV Mat to convert
// @param minFilter: The minifying function used by OpenGL when the texture is scaled down
// @param magFilter: The magnification function used by OpenGL when the texture is scaled up
// @param wrapFilter: The wrap function used by OpenGL when the texture coordinates are outside the standard range
// @return: The generated OpenGL texture ID
GLuint matToTexture(const cv::Mat &mat, GLenum minFilter, GLenum magFilter, GLenum wrapFilter)
{
    GLuint textureID;

    glGenTextures(1, &textureID); // Generate a new OpenGL texture ID
    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glGenTextures: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    glBindTexture(GL_TEXTURE_2D, textureID); // Bind the texture to the GL_TEXTURE_2D target
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glBindTexture: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    // Set the texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

    // Specify the texture image, converting the OpenCV Mat to an OpenGL texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mat.cols, mat.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, mat.ptr());
    error = glGetError();
    if (error != GL_NO_ERROR)
    {
        std::cerr << "OpenGL Error after glTexImage2D: " << error << std::endl;
        return 0; // Return 0 to indicate failure
    }

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture

    return textureID; // Return the texture ID
}

// Cleans up the ImGui and OpenGL/GLFW resources
void cleanupGui()
{
    ImGui_ImplOpenGL3_Shutdown(); // Shutdown ImGui OpenGL3 bindings
    ImGui_ImplGlfw_Shutdown();    // Shutdown ImGui GLFW bindings
    ImGui::DestroyContext();      // Destroy the ImGui context
}
