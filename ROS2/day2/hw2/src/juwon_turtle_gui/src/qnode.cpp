/**
 * @file /src/qnode.cpp
 *
 * @brief Ros communication central!
 *
 * @date August 2024
 **/

/*****************************************************************************
** Includes
*****************************************************************************/

#include "../include/juwon_turtle_gui/qnode.hpp"

QNode::QNode()
{
	int argc = 0;
	char** argv = NULL;
	rclcpp::init(argc, argv);
	node = rclcpp::Node::make_shared("juwon_turtle_gui");
	cmd_vel_pub_ = node->create_publisher<geometry_msgs::msg::Twist>("/turtle1/cmd_vel", 10);
	set_pen_client_ = node->create_client<turtlesim::srv::SetPen>("/turtle1/set_pen");
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

void QNode::publish_cmd_vel(double linear, double angular)
{
	auto msg = geometry_msgs::msg::Twist();
	msg.linear.x = linear;
	msg.angular.z = angular;
	cmd_vel_pub_->publish(msg);
	RCLCPP_INFO(node->get_logger(), "linear.x=%.1f, angular.z=%.1f", linear, angular);
	Q_EMIT cmdVelUpdated(linear, angular);
}

void QNode::set_pen(int r, int g, int b, int width) //선 굵기,색상
{
	auto request = std::make_shared<turtlesim::srv::SetPen::Request>();
	request->r = r;
	request->g = g;
	request->b = b;
	request->width = width;
	request->off = 0;
	set_pen_client_->async_send_request(request);
	QThread::msleep(100);	// 적용될 시간
}

void QNode::straight()	//직진
{
	publish_cmd_vel(2.0, 0.0);
	QThread::msleep(1000);
}

void QNode::rotate120()	//120도 회전
{
	publish_cmd_vel(0.0, 2.094);
	QThread::msleep(1000);
}

void QNode::rotate90()	//90도 회전
{
	publish_cmd_vel(0.0, 1.5708);
	QThread::msleep(1000);
}

void QNode::rotate144()	//144도 회전
{
	publish_cmd_vel(0.0, 2.5133);
	QThread::msleep(1000);
}

void QNode::stop()	//정지
{
	publish_cmd_vel(0.0, 0.0);
	QThread::msleep(100);
}

void QNode::star()  //별
{
    set_pen(255, 255, 0, 3);
    for(int i=0; i<5; i++)
    {
        straight();
        stop();
        rotate144();
        stop();
    }
    stop();
}

void QNode::circle()    //원
{
    set_pen(0, 0, 255, 16);
    auto msg = geometry_msgs::msg::Twist();
    msg.linear.x = 10.0;
    msg.angular.z = 7.0;
    cmd_vel_pub_->publish(msg);
    RCLCPP_INFO(node->get_logger(), "linear.x=%.1f, angular.z=%.1f", msg.linear.x, msg.angular.z);
    QThread::msleep(1000);
}

void QNode::rectangle() //사각형
{
    set_pen(0, 255, 0, 10);
    for (int i=0; i<4; i++)
    {
        straight();
        stop();
        rotate90();
        stop();
    }
    stop();
}

void QNode::triangle()
{
    set_pen(255, 0, 0, 6);
    for (int i = 0; i < 3; i++)
    {
        straight();
        stop();
        rotate120();
        stop();
    }
    stop();
}
