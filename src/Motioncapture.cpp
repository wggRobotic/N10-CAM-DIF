#include <cam_dif/Motioncapture.hpp>

void MotionCapture::detection(sensor_msgs::msg::Image::SharedPtr &ImageOutput, const sensor_msgs::msg::Image::ConstSharedPtr &ImageInput,std::vector<uchar> prev_data)
{
    auto width = ImageInput->width;
    auto height = ImageInput->height;
    auto step = ImageInput->step;
    auto data = ImageInput->data;

    if (prev_data.empty())
    {
      for (size_t i = 0; i < data.size(); i++)
        prev_data.push_back(data[i]);
    }
    else
    {
      size_t comp = step / width;
      for (size_t i = 0; i < data.size(); i += comp)
      {
        uchar avg = 0;
        for (int j = 0; j < comp; j++)
        {
          auto a = data[i + j];
          auto b = prev_data[i + j];
          auto c = a - b;

          avg += c;

          prev_data[i + j] = a;
        }

        avg = (avg < 200 ? 0 : 255);
        for (int j = 0; j < comp; j++)
          data[i + j] = avg;
      }
    
    ImageOutput->width = width;
    ImageOutput->height = height;
    ImageOutput->step = step;
    ImageOutput->encoding =ImageInput->encoding;
    ImageOutput->header =ImageInput->header;
    ImageOutput->is_bigendian =ImageInput->is_bigendian;
    ImageOutput->data = data;
    ImageOutput->data = data;
    
}

