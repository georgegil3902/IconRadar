#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <opencv2/opencv.hpp>
#include <limits>

// Structure to hold radar data
struct RadarData
{
    float x;
    float y;
    float confidence;
};

// Function to load radar data from a CSV file
std::vector<RadarData> loadRadarData(const std::string &filename)
{
    std::vector<RadarData> data;
    std::ifstream file(filename);
    std::string line;

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return data;
    }

    // Read each line from the CSV file
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        RadarData radarData;
        char comma;

        // Parse the line and extract radar data
        if (iss >> radarData.x >> comma >> radarData.y >> comma >> radarData.confidence)
        {
            data.push_back(radarData);
        }
    }

    return data;
}

// Function to get a continuous color based on confidence
cv::Scalar getColor(float confidence)
{
    cv::Scalar color;
    cv::Scalar blue(255, 0, 0);
    cv::Scalar green(0, 255, 0);
    cv::Scalar yellow(0, 255, 255);
    cv::Scalar red(0, 0, 255);

    float thresh = 0.6f;

    if (confidence < thresh)
    {
        // Interpolate between Green and Yellow (compressed range)
        float ratio = (confidence) / (thresh);
        color = (1.0f - ratio) * blue + ratio * green;
    }
    else
    {
        // Interpolate between Yellow and Red (compressed range)
        float ratio = (confidence - thresh) / (1.0f - thresh);
        color = (1.0f - ratio) * yellow + ratio * red;
    }

    return color;
}

int main(int argc, char **argv)
{
    // Filepath to the CSV file in the assets folder
    std::string filename = "../assets/OGM.csv"; // Adjust this path based on your project structure

    // Load radar data from the CSV file
    std::vector<RadarData> radarData = loadRadarData(filename);

    // Check if the radar data was loaded successfully
    if (radarData.empty())
    {
        std::cerr << "Error: No data loaded from " << filename << std::endl;
        return -1;
    }

    // Debug output to verify the loaded data
    // for (const auto &data : radarData)
    // {
    //     std::cout << "x: " << data.x << ", y: " << data.y << ", confidence: " << data.confidence << std::endl;
    // }

    // Find the min and max values for x and y coordinates
    float xMin = std::numeric_limits<float>::max();
    float xMax = std::numeric_limits<float>::lowest();
    float yMin = std::numeric_limits<float>::max();
    float yMax = std::numeric_limits<float>::lowest();

    for (const auto &data : radarData)
    {
        if (data.x < xMin)
            xMin = data.x;
        if (data.x > xMax)
            xMax = data.x;
        if (data.y < yMin)
            yMin = data.y;
        if (data.y > yMax)
            yMax = data.y;
    }

    // Debug output to verify the min and max values
    std::cout << "xMin: " << xMin << ", xMax: " << xMax << std::endl;
    std::cout << "yMin: " << yMin << ", yMax: " << yMax << std::endl;

    // Create an image to visualize the radar data
    int width = 800;
    int height = 800;
    cv::Mat radarImage = cv::Mat::zeros(height, width, CV_8UC3); // 8UC3 means 8-bit 3-channel color image

    // Calculate scaling factors
    float xRange = xMax - xMin;
    float yRange = yMax - yMin;
    float xScale = height / xRange;
    float yScale = width / yRange;
    float uScale = std::min(xScale, yScale);

    // Plot the radar data with scaling
    for (const auto &data : radarData)
    {
        int v = static_cast<int>(height / 2 - (data.x * uScale));
        int h = static_cast<int>(width / 2 + (data.y * uScale)); // Flip horizontal-axis for correct orientation

        // Determine color based on confidence level
        cv::Scalar color = getColor(data.confidence);

        // Draw a circle for each radar data point
        cv::circle(radarImage, cv::Point(h, v), 0.75, color, -1);
    }

    // Display the radar image in a window
    cv::imshow("Radar Data Visualization", radarImage);

    // Wait for a key press indefinitely
    cv::waitKey(0);

    return 0;
}
