import rclpy
from rclpy.node import Node
from std_msgs.msg import String, Int32, Float32, Bool


class MySubscriber(Node):

    def __init__(self):
        super().__init__('my_py_subscriber')
        self.str_sub = self.create_subscription(
            String, 'my_str_topic', self.str_callback, 10)
        self.int_sub = self.create_subscription(
            Int32, 'my_int_topic', self.int_callback, 10)
        self.float_sub = self.create_subscription(
            Float32, 'my_float_topic', self.float_callback, 10)
        self.bool_sub = self.create_subscription(
            Bool, 'my_bool_topic', self.bool_callback, 10)

    def str_callback(self, msg):
        self.get_logger().info('Received string: {0}'.format(msg.data))

    def int_callback(self, msg):
        self.get_logger().info('Received int: {0}'.format(msg.data))

    def float_callback(self, msg):
        self.get_logger().info('Received float: {0}'.format(msg.data))

    def bool_callback(self, msg):
        self.get_logger().info('Received bool: {0}'.format(msg.data))


def main(args=None):
    rclpy.init(args=args)
    node = MySubscriber()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.get_logger().info('Keyboard Interrupt (SIGINT)')
    finally:
        node.destroy_node()
        rclpy.shutdown()


if __name__ == '__main__':
    main()