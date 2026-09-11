#ifndef TELEOP_PUBLISHER_HPP
#define TELEOP_PUBLISHER_HPP

#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <turtlesim/srv/set_pen.hpp>

class TeleopPublisher : public rclcpp::Node
{
public:
    TeleopPublisher();
    void run();

private:
    char get_key();
    void set_pen(int r, int g, int b, int width);
    void triangle();
    void rectangle();
    void circle();
    void star();

    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
    rclcpp::Client<turtlesim::srv::SetPen>::SharedPtr pen_client_;

    int tri_r, tri_g, tri_b, tri_width;
    int rect_r, rect_g, rect_b, rect_width;
    int circle_r, circle_g, circle_b, circle_width;
    int star_r, star_g, star_b, star_width;
};

#endif