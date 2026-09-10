#ifndef juwon_chat_TALKER_QNODE_HPP_
#define juwon_chat_TALKER_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <std_msgs/msg/string.hpp>

class TalkerNode : public QThread
{
  Q_OBJECT
public:
  TalkerNode();
  ~TalkerNode();
  void publish_msg(std::string text);

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr talker_;

Q_SIGNALS:
  void rosShutDown();
};

#endif