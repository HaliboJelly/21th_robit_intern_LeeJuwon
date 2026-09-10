#include "my_subscriber.hpp"

MySubscriber::MySubscriber() : Node("my_subscriber")
{
    sub_ = this->create_subscription<custom_msg_interface::msg::VectorMsg>("my_vector_topic",10,std::bind(&MySubscriber::vector_callback,this,std::placeholders::_1));
}


MySubscriber::~MySubscriber()
{
}

void MySubscriber::vector_callback(const custom_msg_interface::msg::VectorMsg::SharedPtr msg)
{
	RCLCPP_INFO(this->get_logger(), "Received message: '%zu'", msg->b.size());
    for (size_t i=0; i < msg->b.size(); i++)
    {
        RCLCPP_INFO(this->get_logger(), "int32_b[%zu] = %d", i, msg->b[i]);
    }
}

int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	auto node = std::make_shared<MySubscriber>();
	rclcpp::spin(node);
	rclcpp::shutdown();
	return 0;
}