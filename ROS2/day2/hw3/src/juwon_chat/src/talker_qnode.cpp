#include "../include/juwon_chat/talker_qnode.hpp"

TalkerNode::TalkerNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("talker_node");
  talker_ = node->create_publisher<std_msgs::msg::String>("chat_topic", 10);
  this->start();
}

TalkerNode::~TalkerNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
}

void TalkerNode::run()
{
  rclcpp::WallRate loop_rate(20);
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  rclcpp::shutdown();
  Q_EMIT rosShutDown();
}

void TalkerNode::publish_msg(std::string text)
{
	auto msg = std_msgs::msg::String();
	msg.data = text;
	talker_->publish(msg);
	RCLCPP_INFO(node->get_logger(), "sent: %s", text.c_str());
}