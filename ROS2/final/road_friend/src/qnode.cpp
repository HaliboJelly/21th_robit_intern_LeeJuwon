#include "../include/road_friend/qnode.hpp"

QNode::QNode()
{
  int argc = 0;
  char** argv = NULL;
  rclcpp::init(argc, argv);
  node = rclcpp::Node::make_shared("road_friend");

  qRegisterMetaType<QVector<QPoint>>("QVector<QPoint>");

  light_sub_ = node->create_subscription<std_msgs::msg::String>(
      "traffic_light", 10,
      [this](const std_msgs::msg::String::SharedPtr msg)
      {
        RCLCPP_INFO(node->get_logger(), "light: %s", msg->data.c_str());
        Q_EMIT lightUpdated(QString::fromStdString(msg->data));
      });

  vehicle_sub_ = node->create_subscription<road_friend::msg::CarArray>(
      "vehicle_pos", 10,
      [this](const road_friend::msg::CarArray::SharedPtr msg)
      {
        QVector<QPoint> cars;
        for (size_t i = 0; i < msg->cars.size(); i++)
        {
          cars.push_back(QPoint((int)msg->cars[i].x, msg->cars[i].lane));
        }
        Q_EMIT vehiclesUpdated(cars);
      });

  player_pub_ = node->create_publisher<road_friend::msg::Move>("player_cmd", 10);
  
  player_sub_ = node->create_subscription<road_friend::msg::PlayerState>(
    "player_state", 10,
    [this](const road_friend::msg::PlayerState::SharedPtr msg)
    {
      Q_EMIT playerUpdated(msg->row, msg->col, msg->alive, msg->score);
    });

  this->start();
}

void QNode::sendMove(int dx, int dy)  //pushbtn에서 받아온 값을 퍼블리셔로 발행
{
  road_friend::msg::Move msg;
  msg.dx = dx;
  msg.dy = dy;
  player_pub_->publish(msg);
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