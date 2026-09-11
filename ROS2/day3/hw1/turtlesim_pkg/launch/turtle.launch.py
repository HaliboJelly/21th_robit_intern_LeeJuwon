import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    config = os.path.join(
        get_package_share_directory('turtlesim_pkg'), 'config', 'params.yaml'
    )

    return LaunchDescription([
        Node(
            package='turtlesim',              #ROS 기본 패키지
            executable='turtlesim_node',
            name='turtlesim'
        ),
        Node(
            package='turtlesim_pkg',          #패키지
            executable='teleop_publisher',
            name='teleop_publisher',
            parameters=[config],
            output='screen',
            prefix='xterm -e'
        ),
    ])