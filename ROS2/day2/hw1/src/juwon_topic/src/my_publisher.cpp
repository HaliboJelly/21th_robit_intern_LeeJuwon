#include "my_publisher.hpp"


MyPublisher::MyPublisher() : Node("my_publisher")
{
	pub_ = this->create_publisher<custom_msg_interface::msg::VectorMsg>("my_vector_topic", 10);
}

MyPublisher::~MyPublisher()
{
}

void MyPublisher::input_data()
{	
	vec_.clear();
	int input;
	int num;
	std::cout << "Enter number of integers: " << std::endl;
	std::cin >> num;
	if (num >= 0)
	{
		for (int i = 0; i < num; ++i)
		{
			std::cout << "Enter int" << std::endl;
			std::cin >> input;
			vec_.push_back(input);
		}
	}
	else
	{
		std::cout << "???" << std::endl;
		return;
	}
}

void MyPublisher::publish_data()
{
	auto msg = custom_msg_interface::msg::VectorMsg();
	msg.b = vec_;
	pub_->publish(msg);
	RCLCPP_INFO(this->get_logger(), "Published vector message with %zu elements.", vec_.size());
}

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<MyPublisher>();
	while (rclcpp::ok())
	{
		node->input_data();
		node->publish_data();
	}
	rclcpp::shutdown();
	return 0;
}