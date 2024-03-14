#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <cv_bridge/cv_bridge.h>

#include "rclcpp/rclcpp.hpp"

using namespace cv;
using namespace std;

class MotionCapture
{
public:
    MotionCapture() {}

    void detectionF(sensor_msgs::msg::Image::SharedPtr &ImageOutput, const sensor_msgs::msg::Image::ConstSharedPtr &ImageInput);

    void detectionI();

private:
    sensor_msgs::msg::Image ActualImgR;
    sensor_msgs::msg::Image EditedImgR;
    Mat ComparisonImg;
    Mat ActualImg;
    std::vector<uchar> prev_data;
};
