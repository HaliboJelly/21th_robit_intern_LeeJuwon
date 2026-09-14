#ifndef VEHICLE_NODE_HPP
#define VEHICLE_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
#include "road_friend/msg/car_array.hpp"
#include "road_friend/msg/player_state.hpp"

struct Car
{
    int lane;
    float x;
};


class VehicleNode : public rclcpp::Node
{
public:
    VehicleNode();

private:
    void light_callback(const std_msgs::msg::String::SharedPtr msg);
    void timer_callback();

    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
    rclcpp::Publisher<road_friend::msg::CarArray>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<road_friend::msg::PlayerState>::SharedPtr player_sub_;

    std::string light_;             // 현재 신호
    std::vector<Car> vehicles_;  // 차량 x좌표들 (0번 차량 1번 차량 2번 차량... 순으로 저장됨)
    double spawn_elapsed_;          // 마지막 생성 후 경과

    double speed_scale_;
    double speed_step_;
    double stop_line_;
    double spawn_period_;
    double update_hz_;
    double road_end_;

    std::vector<double> lane_dirs_;    // 1.0 = 오른쪽, -1.0 = 왼쪽
    std::vector<double> lane_speeds_;
    int lane_count_;
};

#endif