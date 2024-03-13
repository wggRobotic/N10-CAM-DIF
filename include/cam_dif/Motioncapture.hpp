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

    sensor_msgs::msg::CompressedImage detection(const sensor_msgs::msg::CompressedImage &ci);

private:
    Mat makeCompressedImageMsgToImage(const sensor_msgs::msg::CompressedImage &ci);
    sensor_msgs::msg::CompressedImage makeImageToCompressedImageMsg(const Mat &frame);

    sensor_msgs::msg::CompressedImage ActualImgR;
    sensor_msgs::msg::CompressedImage EditedImgR;
    Mat ComparisonImg;
    Mat ActualImg;
};
