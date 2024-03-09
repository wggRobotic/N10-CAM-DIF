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
#include "Motioncapture.cpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

// Kevin is the guy for communication ;)
class Kevin : public rclcpp::Node
{
public:
  Kevin()
  : Node("CamDif"), count_(0)
  {
    subscription_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
      "cam_pub/compressed", 10, std::bind(&Kevin::topic_callback, this, _1));

    publisher_ = this->create_publisher<sensor_msgs::msg::CompressedImage>("camdif", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&Kevin::timer_callback, this));
  }

private:
  void timer_callback(){

  }
  void topic_callback(const sensor_msgs::msg::CompressedImage & msg)
  {
    message = mc.detection(msg);
    publisher_ ->publish(message);
  }
  sensor_msgs::msg::CompressedImage *messagePtr = new sensor_msgs::msg::CompressedImage();
  sensor_msgs::msg::CompressedImage &message = *messagePtr;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher_;
  rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscription_;
  size_t count_;
  Motioncapture mc;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Kevin>());
  rclcpp::shutdown();
  return 0;
}
