import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32, Float32, Bool


class MyPublisher(Node):

    def __init__(self):
        super().__init__('my_py_publisher')
        self.str_pub = self.create_publisher(String, 'my_str_topic', 10)
        self.int_pub = self.create_publisher(Int32, 'my_int_topic', 10)
        self.float_pub = self.create_publisher(Float32, 'my_float_topic', 10)
        self.bool_pub = self.create_publisher(Bool, 'my_bool_topic', 10)
        self.timer = self.create_timer(1, self.timer_callback)
        self.count = 0

    def timer_callback(self):
        str_msg = String()
        int_msg = Int32()
        float_msg = Float32()
        bool_msg = Bool()

        self.count += 1
        str_msg.data = 'Hello from Python: {0}'.format(self.count)
        int_msg.data = self.count
        float_msg.data = self.count * 1.1
        bool_msg.data = self.count % 2 == 0

        self.str_pub.publish(str_msg)
        self.int_pub.publish(int_msg)
        self.float_pub.publish(float_msg)
        self.bool_pub.publish(bool_msg)
        self.get_logger().info('Published: {0}'.format(str_msg.data))


def main(args=None):
    rclpy.init(args=args)
    node = MyPublisher()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Keyboard Interrupt (SIGINT)')
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()