#include "../include/juwon_chat/listener_qnode.hpp"

ListenerNode::ListenerNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("listener_node");
  listener_ = node->create_subscription<std_msgs::msg::String>(
    "chat_topic", 10,
    std::bind(&ListenerNode::msg_callback, this, std::placeholders::_1));
  this->start();
}

ListenerNode::~ListenerNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
}

void ListenerNode::run()
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

void ListenerNode::msg_callback(const std_msgs::msg::String::SharedPtr msg)
{
	RCLCPP_INFO(node->get_logger(), "recv: %s", msg->data.c_str());
	Q_EMIT msgReceived(QString::fromStdString(msg->data));
}