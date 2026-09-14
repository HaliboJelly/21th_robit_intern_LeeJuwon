from launch import LaunchDescription
from launch_ros.actions import LifecycleNode

def generate_launch_description():
    pub = LifecycleNode(
        package='tongsin2',
        executable='my_publisher',
        name='my_publisher',
        namespace='',
        output='screen')

    sub = LifecycleNode(
        package='tongsin2',
        executable='my_subscriber',
        name='my_subscriber',
        namespace='',
        output='screen')

    return LaunchDescription([pub, sub])