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

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

using namespace std::chrono_literals;
using std::placeholders::_1;

/* This example creates a subclass of Node and uses std::bind() to register a
 * member function as a callback from the timer. */

class Kevin : public rclcpp::Node
{
public:
  Kevin()
  : Node("CamDif"), count_(0)
  {
    subscription_ = this->create_subscription<std_msgs::msg::String>(
      "topic", 10, std::bind(&Kevin::topic_callback, this, _1));

    publisher_ = this->create_publisher<std_msgs::msg::String>("camdif", 10);
    timer_ = this->create_wall_timer(
      500ms, std::bind(&Kevin::timer_callback, this));
  }

private:
  void timer_callback(){

  }
  void topic_callback(const std_msgs::msg::String & msg) const
  {
    message = msg;
    publisher_ ->publish(message);
  }
  std_msgs::msg::String *messagePtr = new std_msgs::msg::String();
  std_msgs::msg::String &message = *messagePtr;
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscription_;
  size_t count_;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<Kevin>());
  rclcpp::shutdown();
  return 0;
}