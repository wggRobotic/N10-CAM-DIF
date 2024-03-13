// Copyright 2016 Open Source Robotics Foundation, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <chrono>
#include <functional>
#include <memory>
#include <string>
#include <opencv2/opencv.hpp>
#include <sensor_msgs/msg/compressed_image.hpp>
#include <cv_bridge/cv_bridge.h>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "image_transport/image_transport.hpp"
#include <cam_dif/Motioncapture.hpp>

using namespace std::chrono_literals;
using std::placeholders::_1;

// Kevin is the guy for communication ;)
class Kevin : public rclcpp::Node
{
public:
  Kevin()
      : Node("Kevin")
  {
  }

  void init(image_transport::ImageTransport &it)
  {
    sub = it.subscribe("/cam_pub", 10, std::bind(&Kevin::topic_callback, this, std::placeholders::_1));
    pub = it.advertise("/kevin", 10);
  }

private:
  void topic_callback(const sensor_msgs::msg::Image::ConstSharedPtr &msg)
  {
    auto width = msg->width;
    auto height = msg->height;
    auto step = msg->step;
    auto data = msg->data;

    if (prev_data.empty())
    {
      for (size_t i = 0; i < data.size(); i++)
        prev_data.push_back(data[i]);
    }
    else
    {
      for (uint32_t y = 0; y < height; y++)
      {
        for (uint32_t x = 0; x < step; x ++)
        {
          auto base = x + y * step;
            auto a = data[base];
            auto b = prev_data[base];

            data[base] = a - b;
            prev_data[base] = a;
        }
      }
      auto message = std::make_shared<sensor_msgs::msg::Image>();
      message->width = width;
      message->height = height;
      message->step = step;
      message->encoding = msg->encoding;
      message->header = msg->header;
      message->is_bigendian = msg->is_bigendian;
      message->data = data;
      pub.publish(message);
    }
  }

  image_transport::Subscriber sub;
  image_transport::Publisher pub;

  std::vector<uchar> prev_data;

  MotionCapture mc;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);

  auto kevin = std::make_shared<Kevin>();
  image_transport::ImageTransport it(kevin);
  kevin->init(it);
  rclcpp::spin(kevin);
  rclcpp::shutdown();
  return 0;
}
