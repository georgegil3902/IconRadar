#include "RadarData.h"
#include <iostream>
#include <fstream>
#include <limits>

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
cv::Scalar getColor(float confidence, float threshhold)
{
    cv::Scalar color;
    cv::Scalar blue(255, 0, 0);
    cv::Scalar green(0, 255, 0);
    cv::Scalar yellow(0, 255, 255);
    cv::Scalar red(0, 0, 255);

    // float thresh = 0.6f;
    float thresh = threshhold;

    if (confidence < threshhold)
    {
        // Interpolate between Green and Yellow (compressed range)
        float ratio = (confidence) / (threshhold);
        color = (1.0f - ratio) * blue + ratio * green;
    }
    else
    {
        // Interpolate between Yellow and Red (compressed range)
        float ratio = (confidence - threshhold) / (1.0f - threshhold);
        color = (1.0f - ratio) * yellow + ratio * red;
    }

    return color;
}

cv::Mat plotRadarData(const std::vector<RadarData> &radarData, int width, int height, float threshhold)
{
    // Calculate the ranges and scaling factors
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

    float xRange = xMax - xMin;
    float yRange = yMax - yMin;
    float xScale = height / xRange;
    float yScale = width / yRange;
    float uScale = std::min(xScale, yScale);

    // Create an image to visualize the radar data
    cv::Mat radarImage = cv::Mat::zeros(height, width, CV_8UC3);

    for (const auto &data : radarData)
    {
        int v = static_cast<int>(height / 2 - (data.x * uScale));
        int h = static_cast<int>(width / 2 + (data.y * uScale));

        cv::Scalar color = getColor(data.confidence, threshhold);
        cv::circle(radarImage, cv::Point(h, v), 0.75, color, -1);
    }

    return radarImage;
}

// Function to apply Canny edge detection
cv::Mat applyCannyEdgeDetection(const cv::Mat &inputImage, float lowThreshold, float highThreshold)
{
    cv::Mat grayImage, edges;
    // Convert the image to grayscale
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    // Apply Canny edge detection
    cv::Canny(grayImage, edges, lowThreshold, highThreshold);

    // Convert the edges to a 3-channel image so it can be displayed with ImGui (which expects 3 channels)
    // cv::Mat edgesColor;
    // cv::cvtColor(edges, edgesColor, cv::COLOR_GRAY2BGR);
    // Display the original radar image with edges overlaid

    return edges;
}