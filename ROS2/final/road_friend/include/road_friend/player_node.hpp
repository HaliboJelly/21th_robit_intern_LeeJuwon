#ifndef PLAYER_NODE_HPP
#define PLAYER_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "road_friend/msg/move.hpp"
#include "road_friend/msg/car_array.hpp"
#include "road_friend/msg/player_state.hpp"
#include "road_friend/srv/reset.hpp"

class PlayerNode : public rclcpp::Node
{
public:
    PlayerNode();

private:
    void move_callback(const road_friend::msg::Move::SharedPtr msg);
    void car_callback(const road_friend::msg::CarArray::SharedPtr msg);
    void request_reset();
    void publish_state();

    rclcpp::Subscription<road_friend::msg::Move>::SharedPtr move_sub_;
    rclcpp::Subscription<road_friend::msg::CarArray>::SharedPtr car_sub_;
    rclcpp::Publisher<road_friend::msg::PlayerState>::SharedPtr publisher_;
    rclcpp::Client<road_friend::srv::Reset>::SharedPtr reset_client_;
    rclcpp::TimerBase::SharedPtr reset_timer_;
    
    int row_;
    int col_;
    bool alive_;
    int score_;
    int deaths_;

    int col_count_;
    double col_width_;
    double player_width_;
    double car_width_;
};

#endif