#include "RadarData.h"
#include <iostream>
#include <fstream>
#include <limits>

// Function to load radar data from a CSV file
// @param filename: The path to the CSV file containing radar data
// @return: A vector of RadarData objects loaded from the file
std::vector<RadarData> loadRadarData(const std::string &filename)
{
    std::vector<RadarData> data; // Vector to store the loaded radar data
    std::ifstream file(filename);
    std::string line;

    // Check if the file was opened successfully
    if (!file.is_open())
    {
        std::cerr << "Error: Could not open the file " << filename << std::endl;
        return data; // Return an empty vector if the file could not be opened
    }

    // Read each line from the CSV file
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        RadarData radarData;
        char comma;

        // Parse the line and extract radar data values for x, y, and confidence
        if (iss >> radarData.x >> comma >> radarData.y >> comma >> radarData.confidence)
        {
            data.push_back(radarData); // Store the parsed data in the vector
        }
    }

    return data; // Return the vector of radar data
}

// Function to map confidence values to colors
// @param confidence: The confidence value of a radar detection point
// @param threshhold: The threshold for distinguishing between low and high confidence
// @return: A cv::Scalar color corresponding to the confidence level
cv::Scalar getColor(float confidence, float threshhold)
{
    cv::Scalar color;
    cv::Scalar blue(255, 0, 0);     // Color for low confidence (blue)
    cv::Scalar green(0, 255, 0);    // Color for moderate confidence (green)
    cv::Scalar yellow(0, 255, 255); // Color for high confidence (yellow)
    cv::Scalar red(0, 0, 255);      // Color for very high confidence (red)

    // Determine color based on confidence level
    if (confidence < threshhold)
    {
        // Interpolate between blue and green for lower confidence values
        float ratio = confidence / threshhold;
        color = (1.0f - ratio) * blue + ratio * green;
    }
    else
    {
        // Interpolate between yellow and red for higher confidence values
        float ratio = (confidence - threshhold) / (1.0f - threshhold);
        color = (1.0f - ratio) * yellow + ratio * red;
    }

    return color; // Return the interpolated color
}

// Function to plot radar data on an image
// @param radarData: A vector of RadarData objects to be plotted
// @param width: The width of the output image
// @param height: The height of the output image
// @param threshhold: The threshold for distinguishing between low and high confidence
// @return: An OpenCV Mat representing the radar data as an image
cv::Mat plotRadarData(const std::vector<RadarData> &radarData, int width, int height, float threshhold)
{
    // Determine the bounds of the radar data
    float xMin = std::numeric_limits<float>::max();
    float xMax = std::numeric_limits<float>::lowest();
    float yMin = std::numeric_limits<float>::max();
    float yMax = std::numeric_limits<float>::lowest();

    // Find the minimum and maximum x and y values in the radar data
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

    // Calculate scaling factors for plotting the radar data onto the image
    float xRange = xMax - xMin;
    float yRange = yMax - yMin;
    float xScale = height / xRange;
    float yScale = width / yRange;
    float uScale = std::min(xScale, yScale); // Use uniform scaling to maintain aspect ratio

    // Create an empty image with the specified dimensions
    cv::Mat radarImage = cv::Mat::zeros(height, width, CV_8UC3);
    float radius = 0.5f; // Radius for plotting points

    // Plot each radar data point on the image
    for (const auto &data : radarData)
    {
        // Convert radar coordinates to image coordinates
        int v = static_cast<int>(height / 2 - (data.x * uScale));
        int h = static_cast<int>(width / 2 + (data.y * uScale));

        // Determine the color based on confidence and plot the point
        cv::Scalar color = getColor(data.confidence, threshhold);
        cv::circle(radarImage, cv::Point(h, v), radius, color, -1); // Draw the radar point
    }

    return radarImage; // Return the generated image
}

// Function to apply Canny edge detection to an image
// @param inputImage: The input image on which to apply edge detection
// @param lowThreshold: The lower threshold for the hysteresis procedure
// @param highThreshold: The upper threshold for the hysteresis procedure
// @return: An OpenCV Mat containing the detected edges as a binary image
cv::Mat applyCannyEdgeDetection(const cv::Mat &inputImage, float lowThreshold, float highThreshold)
{
    cv::Mat grayImage, edges;

    // Convert the image to grayscale (required for Canny edge detection)
    cv::cvtColor(inputImage, grayImage, cv::COLOR_BGR2GRAY);

    // Apply Canny edge detection
    cv::Canny(grayImage, edges, lowThreshold, highThreshold, 3, true);

    // Return the detected edges as a binary image
    return edges;
}
