#ifndef MY_PUBLISHER_HPP
#define MY_PUBLISHER_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"

class MyPublisher : public rclcpp::Node
{
public:
	MyPublisher();
	~MyPublisher();

private:
	rclcpp::TimerBase::SharedPtr timer_;
	void timer_callback();

	rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr int_pub_;
	rclcpp::Publisher<std_msgs::msg::String>::SharedPtr str_pub_;
	rclcpp::Publisher<std_msgs::msg::Float32>::SharedPtr float_pub_;
	rclcpp::Publisher<std_msgs::msg::Bool>::SharedPtr bool_pub_;
	int count_;
};

#endif