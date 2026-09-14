#ifndef GAME_NODE_HPP
#define GAME_NODE_HPP

#include "rclcpp/rclcpp.hpp"
#include "road_friend/srv/reset.hpp"

class GameNode : public rclcpp::Node
{
public:
    GameNode();

private:
    void reset_callback(const std::shared_ptr<road_friend::srv::Reset::Request> req,
                        std::shared_ptr<road_friend::srv::Reset::Response> res);

    rclcpp::Service<road_friend::srv::Reset>::SharedPtr service_;

    int start_row_;
    int start_col_;
};

#endif