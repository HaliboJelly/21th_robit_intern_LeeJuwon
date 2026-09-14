#include "my_publisher.hpp"

using namespace std::chrono_literals;

MyPublisher::MyPublisher() : rclcpp_lifecycle::LifecycleNode("my_publisher")
{
	count_ = 0;
}

MyPublisher::~MyPublisher()
{
}

MyPublisher::CallbackReturn MyPublisher::on_configure(const rclcpp_lifecycle::State &)
{
	rclcpp::QoS int_qos(rclcpp::KeepLast(10));
	int_qos.reliable();
	int_qos.transient_local();

	rclcpp::QoS str_qos(rclcpp::KeepLast(10));
	str_qos.reliable();
	str_qos.durability_volatile();

	rclcpp::QoS float_qos(rclcpp::KeepLast(5));
	float_qos.best_effort();
	float_qos.durability_volatile();

	rclcpp::QoS bool_qos(rclcpp::KeepLast(1));
	bool_qos.reliable();
	bool_qos.transient_local();

	int_pub_ = this->create_publisher<std_msgs::msg::Int32>("my_int_topic", int_qos);
	str_pub_ = this->create_publisher<std_msgs::msg::String>("my_str_topic", str_qos);
	float_pub_ = this->create_publisher<std_msgs::msg::Float32>("my_float_topic", float_qos);
	bool_pub_ = this->create_publisher<std_msgs::msg::Bool>("my_bool_topic", bool_qos);

	timer_ = this->create_wall_timer(1s, std::bind(&MyPublisher::timer_callback, this));

	RCLCPP_INFO(get_logger(), "on_configure");
	return CallbackReturn::SUCCESS;
}

MyPublisher::CallbackReturn MyPublisher::on_activate(const rclcpp_lifecycle::State & state)
{
	LifecycleNode::on_activate(state);
	RCLCPP_INFO(get_logger(), "on_activate");
	return CallbackReturn::SUCCESS;
}

MyPublisher::CallbackReturn MyPublisher::on_deactivate(const rclcpp_lifecycle::State & state)
{
	LifecycleNode::on_deactivate(state);
	RCLCPP_INFO(get_logger(), "on_deactivate");
	return CallbackReturn::SUCCESS;
}


MyPublisher::CallbackReturn MyPublisher::on_cleanup(const rclcpp_lifecycle::State &)
{
	timer_.reset();
	str_pub_.reset();
	int_pub_.reset();
	float_pub_.reset();
	bool_pub_.reset();
	RCLCPP_INFO(get_logger(), "on_cleanup");
	return CallbackReturn::SUCCESS;
}

MyPublisher::CallbackReturn MyPublisher::on_shutdown(const rclcpp_lifecycle::State &)
{
	timer_.reset();
	str_pub_.reset();
	int_pub_.reset();
	float_pub_.reset();
	bool_pub_.reset();
	RCLCPP_INFO(get_logger(), "on_shutdown");
	return CallbackReturn::SUCCESS;
}

void MyPublisher::timer_callback()
{	
	if (!str_pub_->is_activated())
	{
		RCLCPP_INFO(this->get_logger(), "inactive - not published");
		return;
	}
	
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
	rclcpp::executors::SingleThreadedExecutor exe;
	auto node = std::make_shared<MyPublisher>();
	exe.add_node(node->get_node_base_interface());
	exe.spin();
	rclcpp::shutdown();
	return 0;
}


	