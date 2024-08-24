#include "RadarData.h"
#include "gui.h"

// Main entry point for the application
int main(int argc, char **argv)
{
    // Load radar data from a CSV file
    // @param filename: Path to the CSV file containing radar data
    // @return: A vector of RadarData objects parsed from the file
    std::string filename = "../assets/OGM.csv";
    std::vector<RadarData> radarData = loadRadarData(filename);

    // Check if radar data was loaded successfully
    if (radarData.empty())
    {
        std::cerr << "Failed to load radar data from " << filename << std::endl;
        return -1; // Exit if data could not be loaded
    }

    // Initialize GLFW (Graphics Library Framework)
    // @return: true if GLFW was initialized successfully, false otherwise
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Define window dimensions
    const int windowWidth = 800;
    const int windowHeight = 900;

    // Set GLFW window hints (optional configurations)
    // Here we specify that the window should not be resizable
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create a GLFW window with specified dimensions and title
    // @param windowWidth: Width of the window in pixels
    // @param windowHeight: Height of the window in pixels
    // @param "Icon Radar": Title of the window
    // @return: A pointer to the created GLFW window, or NULL if creation fails
    GLFWwindow *window = glfwCreateWindow(windowWidth, windowHeight, "Icon Radar", NULL, NULL);
    if (!window)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // Clean up and exit if window creation fails
        return -1;
    }

    // Make the created window's OpenGL context current
    glfwMakeContextCurrent(window);

    // Enable vsync for smoother rendering (limits the frame rate to the display's refresh rate)
    glfwSwapInterval(1);

    // Load OpenGL functions using GLAD
    // @return: true if OpenGL functions were loaded successfully, false otherwise
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Initialize ImGui for rendering the graphical user interface (GUI)
    // @param window: The GLFW window where ImGui will render
    initializeGui(window);

    // Create an image from the radar data for visualization
    // @param width: Width of the generated radar image
    // @param height: Height of the generated radar image
    // @param thresh: Threshold for filtering radar data based on confidence
    // @return: An OpenCV Mat representing the radar image
    int width = 800;
    int height = 800;
    float thresh = 0.6f;
    cv::Mat radarImage = plotRadarData(radarData, width, height, thresh);

    // Main loop: Render the GUI and process events until the window is closed
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents(); // Poll for and process input events (keyboard, mouse, etc.)

        // Render the ImGui interface with the radar image
        // @param window: The GLFW window where ImGui will render
        // @param radarImage: The radar image to be displayed in the GUI
        renderGui(window, radarImage);

        // Swap front and back buffers to display the rendered frame
        glfwSwapBuffers(window);
    }

    // Cleanup and terminate the ImGui context and GLFW window
    cleanupGui();
    glfwDestroyWindow(window); // Destroy the window and its associated OpenGL context
    glfwTerminate();           // Terminate GLFW and clean up any allocated resources

    return 0; // Exit the application
}
