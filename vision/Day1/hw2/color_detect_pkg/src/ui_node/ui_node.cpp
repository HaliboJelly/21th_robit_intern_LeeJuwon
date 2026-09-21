#include "color_detect_pkg/ui_node/ui_node.hpp"

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

    cv::Mat hsv, mask_r1, mask_r2, mask_r, mask_b, mask_g, view;
    
    cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::inRange(hsv, cv::Scalar(0, 70, 70), cv::Scalar(10, 255, 255), mask_r1);	// 빨강 낮은 H
    cv::inRange(hsv, cv::Scalar(170, 70, 70), cv::Scalar(179, 255, 255), mask_r2);	// 빨강 높은 H
    cv::bitwise_or(mask_r1, mask_r2, mask_r);
    cv::inRange(hsv, cv::Scalar(100, 70, 70), cv::Scalar(130, 255, 255), mask_b);	// 파랑
    cv::inRange(hsv, cv::Scalar(40, 40, 40), cv::Scalar(80, 255, 255), mask_g);	// 초록

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));	// 5x5 원형
    cv::morphologyEx(mask_r, mask_r, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_r, mask_r, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask_b, mask_b, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_b, mask_b, cv::MORPH_CLOSE, kernel);
    cv::morphologyEx(mask_g, mask_g, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask_g, mask_g, cv::MORPH_CLOSE, kernel);

    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(mask_r, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        if (cv::contourArea(contours[i]) < 500)	// 작은 덩어리 무시
        {
            continue;
        }
        cv::rectangle(frame, cv::boundingRect(contours[i]), cv::Scalar(0, 0, 255), 3);	// BGR 빨강
    }

    cv::findContours(mask_b, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        if (cv::contourArea(contours[i]) < 500)
        {
            continue;
        }
        cv::rectangle(frame, cv::boundingRect(contours[i]), cv::Scalar(255, 0, 0), 3);	// 파랑
    }

    cv::findContours(mask_g, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    for (size_t i = 0; i < contours.size(); i++)
    {
        if (cv::contourArea(contours[i]) < 500)
        {
            continue;
        }
        cv::rectangle(frame, cv::boundingRect(contours[i]), cv::Scalar(0, 255, 0), 3);	// 초록
    }

    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);	// Qt는 RGB
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