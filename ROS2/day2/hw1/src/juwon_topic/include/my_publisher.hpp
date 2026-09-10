#ifndef MY_PUBLISHER_HPP
#define MY_PUBLISHER_HPP

#include "rclcpp/rclcpp.hpp"
#include "custom_msg_interface/msg/vector_msg.hpp"
#include <iostream>
#include <vector>

class MyPublisher : public rclcpp::Node
{
public:
	MyPublisher();
	~MyPublisher();
	void input_data();	// cin으로 받아서 vec_에 저장
	void publish_data();	// vec_ 담아서 발행

private:
	rclcpp::Publisher<custom_msg_interface::msg::VectorMsg>::SharedPtr pub_;
	std::vector<int32_t> vec_;
};

#endif