#ifndef juwon_chat_LISTENER_QNODE_HPP_
#define juwon_chat_LISTENER_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <std_msgs/msg/string.hpp>

class ListenerNode : public QThread
{
  Q_OBJECT
public:
  ListenerNode();
  ~ListenerNode();

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr listener_;
  void msg_callback(const std_msgs::msg::String::SharedPtr msg);

Q_SIGNALS:
  void rosShutDown();
  void msgReceived(QString text);
};

#endif