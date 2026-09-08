#include "my_subscriber.hpp"

MySubscriber::MySubscriber() : Node("my_subscriber")
{
	my_str_subscriber_ = this->create_subscription<std_msgs::msg::String>(
		"my_str_topic",
		10,
		std::bind(&MySubscriber::topic_callback, this, std::placeholders::_1));
	my_int_subscriber_ = this->create_subscription<std_msgs::msg::Int32>(
		"my_int_topic",
		10,
		std::bind(&MySubscriber::int_callback, this, std::placeholders::_1));
	my_float_subscriber_ = this->create_subscription<std_msgs::msg::Float32>(
		"my_float_topic",
		10,
		std::bind(&MySubscriber::float_callback, this, std::placeholders::_1));
	my_bool_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
		"my_bool_topic",
		10,
		std::bind(&MySubscriber::bool_callback, this, std::placeholders::_1));
}

MySubscriber::~MySubscriber()
{
}

void MySubscriber::topic_callback(const std_msgs::msg::String::SharedPtr msg)
{
	RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data.c_str());
}
void MySubscriber::int_callback(const std_msgs::msg::Int32::SharedPtr msg)
{
	RCLCPP_INFO(this->get_logger(), "Received message: '%d'", msg->data);
}
void MySubscriber::float_callback(const std_msgs::msg::Float32::SharedPtr msg)
{
	RCLCPP_INFO(this->get_logger(), "Received message: '%f'", msg->data);
}
void MySubscriber::bool_callback(const std_msgs::msg::Bool::SharedPtr msg)
{
	RCLCPP_INFO(this->get_logger(), "Received message: '%s'", msg->data ? "true" : "false");
}
	

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<MySubscriber>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}