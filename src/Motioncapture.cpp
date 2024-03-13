#include <cam_dif/Motioncapture.hpp>

sensor_msgs::msg::CompressedImage MotionCapture::detection(const sensor_msgs::msg::CompressedImage &ci)
{
    ActualImgR = ci;
    ActualImg = makeCompressedImageMsgToImage(ci);

    // make your editing stuff;
    auto EditedImg = ActualImg;

    EditedImgR = makeImageToCompressedImageMsg(EditedImg);
    ComparisonImg = ActualImg;

    return EditedImgR;
}

Mat MotionCapture::makeCompressedImageMsgToImage(const sensor_msgs::msg::CompressedImage &ci)
{
    Mat frame;
    cv_bridge::CvImagePtr cv_ptr;
    try
    {
        cv_ptr = cv_bridge::toCvCopy(ci, sensor_msgs::image_encodings::RGB8);
    }
    catch (cv_bridge::Exception e)
    {
        RCLCPP_ERROR(rclcpp::get_logger("rclcpp"), "cv_bridge exception %s", e.what());
        return frame;
    }

    frame = cv_ptr->image;
    return frame;
}

sensor_msgs::msg::CompressedImage MotionCapture::makeImageToCompressedImageMsg(const Mat &frame)
{
    cv_bridge::CvImagePtr cv_ptr;
    sensor_msgs::msg::CompressedImage Cmsg;
     Cmsg.header.stamp = rclcpp::Clock().now(); // Set the timestamp to the current time
    Cmsg.header.frame_id = "camera"; // Set the frame ID
    //Cmsg.header = msg-> header;
    Cmsg.format = "jpeg";
    vector<int> params = {cv::IMWRITE_JPEG_QUALITY, 90};
    imencode(".jpeg", frame, Cmsg.data, params);
    return Cmsg;
}
