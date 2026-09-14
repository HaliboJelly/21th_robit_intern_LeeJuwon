#ifndef TRAFFIC_LIGHT_NODE_HPP
#define TRAFFIC_LIGHT_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

class TrafficLightNode : public rclcpp::Node
{
public:
    TrafficLightNode();

private:
    void timer_callback();

    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string state_;     // "RED" / "GREEN" / "YELLOW"
    double elapsed_;        // 현재 상태 지속 시간
    double r_time_;
    double g_time_;
    double y_time_;
    
};

#endif