#include "ball_distance_pkg/ui_node/ui_node.hpp"

UiNode::UiNode()
{
    node_ = rclcpp::Node::make_shared("ui_node");
    sub_ = node_->create_subscription<sensor_msgs::msg::Image>(
        "/ball/image", 10,
        std::bind(&UiNode::image_callback, this, std::placeholders::_1));
    dist_sub_ = node_->create_subscription<std_msgs::msg::Float64MultiArray>(
        "/ball/distance", 10,
        std::bind(&UiNode::dist_callback, this, std::placeholders::_1));
    this->start();
}

UiNode::~UiNode()
{
    rclcpp::shutdown();
    this->wait();
}

void UiNode::run()
{
    rclcpp::spin(node_);
}

void UiNode::image_callback(const sensor_msgs::msg::Image::SharedPtr msg)
{
    cv::Mat frame;
    try
    {
        frame = cv_bridge::toCvCopy(msg, "bgr8")->image;
    }
    catch (cv_bridge::Exception &e)
    {
        RCLCPP_ERROR(node_->get_logger(), "cv_bridge: %s", e.what());
        return;
    }
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);	// Qt는 RGB
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    {
        std::lock_guard<std::mutex> lock(mtx_);	// 쓸 때 잠금
        img_ = img.copy();
    }
    emit img_ready();
}

void UiNode::dist_callback(const std_msgs::msg::Float64MultiArray::SharedPtr msg)
{
    if (msg->data.size() < 2)
    {
        return;
    }
    {
        std::lock_guard<std::mutex> lock(mtx_);	// 쓸 때 잠금
        dist_a_ = msg->data[0];
        dist_b_ = msg->data[1];
    }
    emit dist_ready();
}

QImage UiNode::get_img()
{
    std::lock_guard<std::mutex> lock(mtx_);	// 읽을 때 잠금
    return img_;
}

void UiNode::get_dist(double &a, double &b)
{
    std::lock_guard<std::mutex> lock(mtx_);	// 읽을 때 잠금
    a = dist_a_;
    b = dist_b_;
}