#include "my_subscriber.hpp"

MySubscriber::MySubscriber() : rclcpp_lifecycle::LifecycleNode("my_subscriber")
{
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


MySubscriber::CallbackReturn MySubscriber::on_configure(const rclcpp_lifecycle::State &)
{
	RCLCPP_INFO(get_logger(), "on_configure");
	return CallbackReturn::SUCCESS;
}

MySubscriber::CallbackReturn MySubscriber::on_activate(const rclcpp_lifecycle::State &)
{
	rclcpp::QoS str_qos(rclcpp::KeepLast(10));
	str_qos.reliable();
	str_qos.durability_volatile();

	rclcpp::QoS int_qos(rclcpp::KeepLast(100));
	int_qos.reliable();
	int_qos.durability_volatile();

	rclcpp::QoS float_qos(rclcpp::KeepLast(5));
	float_qos.best_effort();
	float_qos.durability_volatile();

	rclcpp::QoS bool_qos(rclcpp::KeepLast(1));
	bool_qos.reliable();
	bool_qos.transient_local();

	my_str_subscriber_ = this->create_subscription<std_msgs::msg::String>(
		"my_str_topic",
		str_qos,
		std::bind(&MySubscriber::topic_callback, this, std::placeholders::_1));

	my_int_subscriber_ = this->create_subscription<std_msgs::msg::Int32>(
		"my_int_topic",
		int_qos,
		std::bind(&MySubscriber::int_callback, this, std::placeholders::_1));

	my_float_subscriber_ = this->create_subscription<std_msgs::msg::Float32>(
		"my_float_topic",
		float_qos,
		std::bind(&MySubscriber::float_callback, this, std::placeholders::_1));

	my_bool_subscriber_ = this->create_subscription<std_msgs::msg::Bool>(
		"my_bool_topic",
		bool_qos,
		std::bind(&MySubscriber::bool_callback, this, std::placeholders::_1));

	RCLCPP_INFO(get_logger(), "on_activate");
	return CallbackReturn::SUCCESS;
}


MySubscriber::CallbackReturn MySubscriber::on_deactivate(const rclcpp_lifecycle::State &)
{
	my_str_subscriber_.reset();
	my_int_subscriber_.reset();
	my_float_subscriber_.reset();
	my_bool_subscriber_.reset();

	RCLCPP_INFO(get_logger(), "on_deactivate");
	return CallbackReturn::SUCCESS;
}


MySubscriber::CallbackReturn MySubscriber::on_cleanup(const rclcpp_lifecycle::State &)
{
	my_str_subscriber_.reset();
	my_int_subscriber_.reset();
	my_float_subscriber_.reset();
	my_bool_subscriber_.reset();

	RCLCPP_INFO(get_logger(), "on_cleanup");
	return CallbackReturn::SUCCESS;
}

MySubscriber::CallbackReturn MySubscriber::on_shutdown(const rclcpp_lifecycle::State &)
{
	my_str_subscriber_.reset();
	my_int_subscriber_.reset();
	my_float_subscriber_.reset();
	my_bool_subscriber_.reset();

	RCLCPP_INFO(get_logger(), "on_shutdown");
	return CallbackReturn::SUCCESS;
}


int main(int argc, char **argv)
{
	rclcpp::init(argc, argv);
	rclcpp::executors::SingleThreadedExecutor exe;
	auto node = std::make_shared<MySubscriber>();
	exe.add_node(node->get_node_base_interface());
	exe.spin();
	rclcpp::shutdown();
	return 0;
}