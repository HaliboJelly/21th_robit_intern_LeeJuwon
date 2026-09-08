#ifndef MY_SUBSCRIBER_HPP
#define MY_SUBSCRIBER_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"
class MySubscriber : public rclcpp::Node
{
public:
	MySubscriber();
	~MySubscriber();

private:
	void topic_callback(const std_msgs::msg::String::SharedPtr msg);
	void int_callback(const std_msgs::msg::Int32::SharedPtr msg);
	void float_callback(const std_msgs::msg::Float32::SharedPtr msg);
	void bool_callback(const std_msgs::msg::Bool::SharedPtr msg);
	rclcpp::Subscription<std_msgs::msg::String>::SharedPtr my_str_subscriber_;
	rclcpp::Subscription<std_msgs::msg::Int32>::SharedPtr my_int_subscriber_;
	rclcpp::Subscription<std_msgs::msg::Float32>::SharedPtr my_float_subscriber_;
	rclcpp::Subscription<std_msgs::msg::Bool>::SharedPtr my_bool_subscriber_;
};

#endif