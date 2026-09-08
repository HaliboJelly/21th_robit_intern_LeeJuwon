#include "my_publisher.hpp"

using namespace std::chrono_literals;

MyPublisher::MyPublisher() : Node("my_publisher")
{
	count_ = 0;
	str_pub_ = this->create_publisher<std_msgs::msg::String>("my_str_topic", 10);
	int_pub_ = this->create_publisher<std_msgs::msg::Int32>("my_int_topic", 10);
	float_pub_ = this->create_publisher<std_msgs::msg::Float32>("my_float_topic", 10);
	bool_pub_ = this->create_publisher<std_msgs::msg::Bool>("my_bool_topic", 10);
	timer_ = this->create_wall_timer(1s, std::bind(&MyPublisher::timer_callback, this));
}

MyPublisher::~MyPublisher()
{
}

void MyPublisher::timer_callback()
{
	auto msg = std_msgs::msg::String();
	auto int_msg = std_msgs::msg::Int32();
	auto float_msg = std_msgs::msg::Float32();
	auto bool_msg = std_msgs::msg::Bool();
	msg.data = "Hello World: " + std::to_string(count_++);
	int_msg.data = count_;
	float_msg.data = static_cast<float>(count_) * 1.1f;
	bool_msg.data = count_ % 2 == 0;
	str_pub_->publish(msg);
	int_pub_->publish(int_msg);
	float_pub_->publish(float_msg);
	bool_pub_->publish(bool_msg);
	RCLCPP_INFO(this->get_logger(), "Published message: '%s'", msg.data.c_str());
}

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<MyPublisher>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}