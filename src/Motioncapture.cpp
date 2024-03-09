#include <opencv2/opencv.hpp>
#include <iostream>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <cv_bridge/cv_bridge.h>

#include "rclcpp/rclcpp.hpp"

using namespace cv;
using namespace std;

class Motioncapture{
public:
    Motioncapture(){}
    
    sensor_msgs::msg::CompressedImage detection(const sensor_msgs::msg::CompressedImage& ci){
        ActualImgR = ci;
        ActualImg = makeCompressedImageMsgToImage(ci);

        // make your editing stuff;

        EditedImgR = makeImageToCompressedImageMsg(EditedImg);
        ComparisonImg= ActualImg;

        return EditedImgR;
    }
private:
    Mat makeCompressedImageMsgToImage (const sensor_msgs::msg::CompressedImage& ci){
        Mat frame;
        cv_bridge::CvImagePtr cv_ptr;
        try{
            cv_ptr =cv_bridge::toCvCopy(ci,sensor_msgs::image_encodings::RGB8);
        }catch(cv_bridge::Exception e){
            RCLCPP_ERROR(rclcpp::get_logger("rclcpp"),"cv_bridge exception %s",e.what());
            return;
        }

        frame = cv_ptr->image;
        return frame;
    }
    sensor_msgs::msg::CompressedImage makeImageToCompressedImageMsg (const Mat& frame){
        cv_bridge::CvImagePtr cv_ptr;
        sensor_msgs::msg::CompressedImage Cmsg;
        //Cmsg.header = msg-> header;
        Cmsg.format="jpeg";
        vector<int>params = {cv::IMWRITE_JPEG_QUALITY,90};
        imencode(".jpeg",frame,Cmsg.data,params);
        return Cmsg;
    }
    sensor_msgs::msg::CompressedImage ActualImgR;
    sensor_msgs::msg::CompressedImage EditedImgR;
    Mat ComparisonImg;
    Mat ActualImg;
    Mat EditedImg;
};