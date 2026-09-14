#ifndef MY_PUBLISHER_HPP
#define MY_PUBLISHER_HPP

#include "rclcpp_lifecycle/lifecycle_node.hpp"
#include "rclcpp_lifecycle/lifecycle_publisher.hpp"
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/int32.hpp"
#include "std_msgs/msg/float32.hpp"
#include "std_msgs/msg/bool.hpp"

class MyPublisher : public rclcpp_lifecycle::LifecycleNode
{
public:
	using CallbackReturn = rclcpp_lifecycle::node_interfaces::LifecycleNodeInterface::CallbackReturn;

	MyPublisher();
	~MyPublisher();

	CallbackReturn on_configure(const rclcpp_lifecycle::State &);
	CallbackReturn on_activate(const rclcpp_lifecycle::State &);
	CallbackReturn on_deactivate(const rclcpp_lifecycle::State &);
	CallbackReturn on_cleanup(const rclcpp_lifecycle::State &);
	CallbackReturn on_shutdown(const rclcpp_lifecycle::State &);
private:
	rclcpp::TimerBase::SharedPtr timer_;
	void timer_callback();

	rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Int32>::SharedPtr int_pub_;
	rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::String>::SharedPtr str_pub_;
	rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Float32>::SharedPtr float_pub_;
	rclcpp_lifecycle::LifecyclePublisher<std_msgs::msg::Bool>::SharedPtr bool_pub_;
	
	int count_;
};

#endif