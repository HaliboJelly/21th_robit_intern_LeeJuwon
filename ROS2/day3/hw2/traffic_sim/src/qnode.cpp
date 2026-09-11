#include "../include/traffic_sim/qnode.hpp"

QNode::QNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("traffic_sim");

  qRegisterMetaType<QVector<float>>("QVector<float>");

  light_sub_ = node->create_subscription<std_msgs::msg::String>(
      "traffic_light", 10,
      [this](const std_msgs::msg::String::SharedPtr msg)
      {
        RCLCPP_INFO(node->get_logger(), "light: %s", msg->data.c_str());
        Q_EMIT lightUpdated(QString::fromStdString(msg->data));
      });

  vehicle_sub_ = node->create_subscription<std_msgs::msg::Float32MultiArray>(
      "vehicle_pos", 10,
      [this](const std_msgs::msg::Float32MultiArray::SharedPtr msg)
      {
        QVector<float> v;
        for (size_t i = 0; i < msg->data.size(); i++)
        {
          v.push_back(msg->data[i]);
        }
        Q_EMIT vehiclesUpdated(v);
      });

  this->start();
}

QNode::~QNode()
{
  if (rclcpp::ok())
  {
    rclcpp::shutdown();
  }
}

void QNode::run()
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