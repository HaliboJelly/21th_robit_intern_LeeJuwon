#include "camera_ui_pkg/ui_node/ui_node.hpp"

UiNode::UiNode()
{
    node_ = rclcpp::Node::make_shared("ui_node");
    sub_ = node_->create_subscription<sensor_msgs::msg::Image>(
        "camera/image", 10,
        std::bind(&UiNode::image_callback, this, std::placeholders::_1));
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
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);
    QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
    {
        std::lock_guard<std::mutex> lock(mtx_);     //이미지 잠그는거 쓸때
        img_ = img.copy();
    }
    emit img_ready();

    
}

QImage UiNode::get_img()
    {
        std::lock_guard<std::mutex> lock(mtx_);     //이미지 잠그는거 읽을때
        return img_;
    }