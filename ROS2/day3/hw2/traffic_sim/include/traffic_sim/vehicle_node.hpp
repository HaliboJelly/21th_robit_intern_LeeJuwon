#ifndef VEHICLE_NODE_HPP
#define VEHICLE_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"

class VehicleNode : public rclcpp::Node
{
public:
    VehicleNode();

private:
    void light_callback(const std_msgs::msg::String::SharedPtr msg);
    void timer_callback();

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
    rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;

    std::string light_;             // 현재 신호
    std::vector<float> vehicles_;   // 차량 x좌표들 (0번 차량 1번 차량 2번 차량... 순으로 저장됨)
    double spawn_elapsed_;          // 마지막 생성 후 경과

    double speed_;
    double stop_line_;
    double spawn_period_;
    double update_hz_;
    double road_end_;
};

#endif