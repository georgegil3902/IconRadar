#ifndef RADARDATA_H
#define RADARDATA_H

#include <vector>
#include <string>
#include <opencv2/opencv.hpp>

// Structure to hold radar data
struct RadarData
{
    float x;
    float y;
    float confidence;
};

// Function to load radar data from a CSV file
std::vector<RadarData> loadRadarData(const std::string &filename);

// Function to get a continuous color based on confidence
cv::Scalar getColor(float confidence, float threshhold);

// Function to plot the radar data on an image
cv::Mat plotRadarData(const std::vector<RadarData> &radarData, int width, int height, float threshhold);

#endif // RADARDATA_H
