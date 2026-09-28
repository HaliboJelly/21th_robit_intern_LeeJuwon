import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    cfg = os.path.join(get_package_share_directory('tongsin_py'), 'config', 'tongsin.yaml')
    return LaunchDescription([
        Node(package='tongsin_py', executable='cam_pub', parameters=[cfg], output='screen'),
        Node(package='tongsin_py', executable='pytorch_node', parameters=[cfg], output='screen'),
    ])