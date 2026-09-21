import os
from ament_index_python.packages import get_package_share_directory
from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    config = os.path.join(get_package_share_directory('camera_ui_pkg'), 'config', 'camera_ui.yaml')
    return LaunchDescription([
        Node(package='camera_ui_pkg', 
             executable='camera_node', 
             name='camera_node', 
             parameters=[config], 
             output='screen'),

        Node(package='camera_ui_pkg', 
             executable='ui_node', 
             name='ui_node', 
             output='screen'),
    ])