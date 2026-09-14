#ifndef MY_SUBSCRIBER_HPP
#define MY_SUBSCRIBER_HPP

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"

class MySubscriber : public rclcpp_lifecycle::LifecycleNode
{
public:
	using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

	MySubscriber();
	~MySubscriber();

	CallbackReturn on_configure(const rclcpp_lifecycle::State &);
	CallbackReturn on_activate(const rclcpp_lifecycle::State &);
	CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
	CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
	CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);

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