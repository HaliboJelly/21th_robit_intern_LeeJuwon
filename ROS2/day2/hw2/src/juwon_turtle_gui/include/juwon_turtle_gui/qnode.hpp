/**
 * @file /include/juwon_turtle_gui/qnode.hpp
 *
 * @brief Communications central!
 *
 * @date February 2011
 **/
/*****************************************************************************
** Ifdefs
*****************************************************************************/

#ifndef juwon_turtle_gui_QNODE_HPP_
#define juwon_turtle_gui_QNODE_HPP_

/*****************************************************************************
** Includes
*****************************************************************************/
#ifndef Q_MOC_RUN
#include <rclcpp/rclcpp.hpp>
#endif
#include <QThread>
#include <geometry_msgs/msg/twist.hpp>
#include <turtlesim/srv/set_pen.hpp>
/*****************************************************************************
** Class
*****************************************************************************/
class QNode : public QThread
{
  Q_OBJECT
public:
  QNode();
  ~QNode();
  void publish_cmd_vel(double linear, double angular);  //움직이기
  void set_pen(int r, int g, int b, int width);       //선 굵기 및 색깔 설정
  void straight();
  void rotate120();
  void rotate90();
  void rotate144();
  void stop();
  void star();
  void circle();
  void rectangle();
  void triangle();
protected:
  void run();

private:
  std::shared_ptr<rclcpp::Node> node;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
  rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr set_pen_client_;
Q_SIGNALS:
  void rosShutDown();
  void cmdVelUpdated(double linear, double angular);
};

#endif /* juwon_turtle_gui_QNODE_HPP_ */
