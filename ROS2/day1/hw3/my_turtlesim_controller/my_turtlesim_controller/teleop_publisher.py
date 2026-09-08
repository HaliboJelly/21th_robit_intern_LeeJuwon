import rclpy
from rclpy.node import Node
from geometry_msgs.msg import Twist
import time
import sys
import termios
import tty

from turtlesim.srv import SetPen


class TeleopPublisher(Node):

    def __init__(self):
        super().__init__('teleop_publisher')
        self.publisher_ = self.create_publisher(Twist, 'turtle1/cmd_vel', 10)
        self.pen_client = self.create_client(SetPen, 'turtle1/set_pen')

    def get_key(self):
        fd = sys.stdin.fileno()
        old = termios.tcgetattr(fd)     # 현재 터미널 설정 백업
        try:
            tty.setraw(fd)              # raw 모드로 전환
            key = sys.stdin.read(1)     # 한 글자만 읽음
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old)   # 원복
        return key

    def run(self):
        print('W: 삼각형  A: 사각형  S: 원  D: 별  Q: 종료')
        while rclpy.ok():
            key = self.get_key()
            if key == 'w':
                self.triangle()
            elif key == 'a':
                self.rectangle()
            elif key == 's':
                self.circle()
            elif key == 'd':
                self.star()
            elif key == 'q' or key == '\x03':
                break

    def set_pen(self, r, g, b, width):
        req = SetPen.Request()
        req.r = r
        req.g = g
        req.b = b
        req.width = width
        req.off = 0
        self.pen_client.call_async(req)
        time.sleep(0.2)      # 적용될 시간

    def star(self):
        self.set_pen(255, 255, 0, 3)
        for i in range(5):
            # 직진
            msg = Twist()
            msg.linear.x = 2.0
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.linear.x = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

            # 144도 회전
            msg.angular.z = 2.5133
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

    def triangle(self):
        self.set_pen(255, 0, 0, 6)
        for i in range(3):
            # 직진
            msg = Twist()
            msg.linear.x = 2.0
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.linear.x = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

            # 120도 회전
            msg.angular.z = 2.0944
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

    def rectangle(self):
        self.set_pen(0, 255, 0, 10)
        for i in range(4):
            # 직진
            msg = Twist()
            msg.linear.x = 2.0
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.linear.x = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

            # 90도 회전
            msg.angular.z = 1.5708
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(1.0)

            # 정지
            msg.angular.z = 0.0
            self.publisher_.publish(msg)
            self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
            time.sleep(0.1)

    def circle(self):
        self.set_pen(0, 0, 255, 16)
        msg = Twist()
        msg.linear.x = 10.0  # 전진 속도 (m/s)
        msg.angular.z = 7.0  # 회전 속도 (rad/s)
        self.publisher_.publish(msg)
        self.get_logger().info(f'Publishing: "linear.x={msg.linear.x}, angular.z={msg.angular.z}"')
        time.sleep(1.0)      # 한 바퀴 도는 시간

def main(args=None):
    rclpy.init(args=args)
    teleop_publisher = TeleopPublisher()
    teleop_publisher.run()

    teleop_publisher.destroy_node()
    rclpy.shutdown()

if __name__ == '__main__':
    main()