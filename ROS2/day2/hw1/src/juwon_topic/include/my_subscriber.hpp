#ifndef MY_SUBSCRIBER_HPP
#define MY_SUBSCRIBER_HPP

#include "rclcpp/rclcpp.hpp"
#include "custom_msg_interface/msg/vector_msg.hpp"
#include <iostream>
#include <vector>

class MySubscriber :public rclcpp::Node
{
public:
    MySubscriber();
    ~MySubscriber();

private:
	void vector_callback(const custom_msg_interface::msg::VectorMsg::SharedPtr msg);
	rclcpp::Subscription<custom_msg_interface::msg::VectorMsg>::SharedPtr sub_;
};


#endif