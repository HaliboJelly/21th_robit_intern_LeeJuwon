cmake_minimum_required(VERSION 3.8)
project(color_detect_pkg)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_CURRENT_SOURCE_DIR}/ui)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(cv_bridge REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets)
find_package(ament_index_cpp REQUIRED)

include_directories(include)

add_executable(camera_node
  src/camera_node/camera_node.cpp
)
ament_target_dependencies(camera_node rclcpp sensor_msgs cv_bridge ament_index_cpp)
target_link_libraries(camera_node ${OpenCV_LIBS})

add_executable(ui_node
  src/ui_node/main.cpp
  src/ui_node/main_window.cpp
  src/ui_node/ui_node.cpp
  include/color_detect_pkg/ui_node/main_window.hpp
  include/color_detect_pkg/ui_node/ui_node.hpp
  ui/mainwindow.ui
)
ament_target_dependencies(ui_node rclcpp sensor_msgs cv_bridge)
target_link_libraries(ui_node ${OpenCV_LIBS} Qt5::Widgets)

install(TARGETS camera_node ui_node
  DESTINATION lib/${PROJECT_NAME}
)
install(DIRECTORY launch config images
  DESTINATION share/${PROJECT_NAME}
)

ament_package()