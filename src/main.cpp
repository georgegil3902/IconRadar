#include "RadarData.h"
#include "gui.h"

int main(int argc, char **argv)
{
    // Load radar data
    std::string filename = "../assets/OGM.csv";
    std::vector<RadarData> radarData = loadRadarData(filename);
    if (radarData.empty())
        return -1;

    // Initialize GLFW
    if (!glfwInit())
        return -1;

    const int windowWidth = 800;
    const int windowHeight = 880;

    // Set the window to be non-resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a window
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Icon Radar", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set Context Current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return -1;
    }

    // Initialize ImGui
    initializeGui(window);

    // Create radar image
    int width = 800;
    int height = 800;
    float thresh = 0.6f;
    cv::Mat radarImage = plotRadarData(radarData, width, height, thresh);

    // cv::Mat edges = applyCannyEdgeDetection(radarImage, 50.0f, 150.0f);
    // cv::Mat overlayImage;
    // cv::cvtColor(edges, overlayImage, cv::COLOR_GRAY2BGR);
    // cv::addWeighted(radarImage, 0.8, overlayImage, 0.8, 0, overlayImage);
    // cv::imshow("Edge Detected Image", overlayImage);

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render the ImGui interface
        renderGui(window, radarImage);

        glfwSwapBuffers(window);
    }

    // Cleanup
    cleanupGui();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
