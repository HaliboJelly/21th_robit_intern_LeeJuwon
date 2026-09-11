#ifndef traffic_sim_QNODE_HPP_
#define traffic_sim_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <QVector>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr light_sub_;
  rclcpp::Subscription<std_msgs::msg::Float32MultiArray>::SharedPtr vehicle_sub_;

Q_SIGNALS:
  void rosShutDown();
  void lightUpdated(QString light);
  void vehiclesUpdated(QVector<float> vehicles);
};

#endif