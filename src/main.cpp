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

    GLFWwindow *window = glfwCreateWindow(800, 800, "Icon Radar", NULL, NULL);

    if (!window)
    {
        glfwTerminate();

        return -1;
    }
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

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        // Render the ImGui interface
        renderGui(width, height, radarImage);

        glfwSwapBuffers(window);
    }

    // Cleanup
    cleanupGui();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
