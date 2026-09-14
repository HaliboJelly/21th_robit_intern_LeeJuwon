#ifndef road_friend_QNODE_HPP_
#define road_friend_QNODE_HPP_

#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QPoint>
#include <QThread>
#include <QVector>
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "road_friend/msg/move.hpp"
#include "road_friend/msg/car_array.hpp"
#include "road_friend/msg/player_state.hpp"

class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();

  void sendMove(int dx, int dy);

protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  rclcpp::Subscription<std_msgs::msg::String>::SharedPtr light_sub_;
  rclcpp::Subscription<road_friend::msg::CarArray>::SharedPtr vehicle_sub_;
  rclcpp::Publisher<road_friend::msg::Move>::SharedPtr player_pub_;
  rclcpp::Subscription<road_friend::msg::PlayerState>::SharedPtr player_sub_;

Q_SIGNALS:
  void rosShutDown();
  void lightUpdated(QString light);
  void vehiclesUpdated(QVector<QPoint> cars);   // x, lane 쌍
  void playerUpdated(int row, int col, bool alive, int score);
};

#endif