import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from cv_bridge import CvBridge
import cv2


class ImagePublisher(Node):

    def __init__(self):
        super().__init__('image_publisher')

        self.declare_parameter('device', 0)
        self.declare_parameter('fps', 30.0)
        self.declare_parameter('image_topic', 'video_frames')

        device = self.get_parameter('device').value
        fps = self.get_parameter('fps').value

        self.publisher_ = self.create_publisher(Image, self.get_parameter('image_topic').value, 10)
        self.timer=self.create_timer(1/fps, self.timer_callback)

        self.cap = cv2.VideoCapture(device, cv2.CAP_V4L2)
        self.cap.set(cv2.CAP_PROP_FOURCC, cv2.VideoWriter_fourcc(*'MJPG'))
        self.cap.set(cv2.CAP_PROP_FRAME_WIDTH, 640)
        self.cap.set(cv2.CAP_PROP_FRAME_HEIGHT, 480)
        self.cap.set(cv2.CAP_PROP_FPS, fps)
        self.cap.set(cv2.CAP_PROP_BUFFERSIZE, 1)

        if not self.cap.isOpened():
            self.get_logger().error('fail to open')
            raise RuntimeError('cannot open camera')

        self.br = CvBridge()
        
        self.get_logger().info('image publisher node started')


    def timer_callback(self):

        ret, frame = self.cap.read()

        if ret == True:
            self.publisher_.publish(self.br.cv2_to_imgmsg(frame, 'bgr8'))
            self.get_logger().info('publisher video frame')

        else:
            self.get_logger().warn('falied to cap frame')


    def __del__(self):
        if self.cap.isOpened():
            self.cap.release()
            self.get_logger().info('camera released')


def main(args=None):
    rclpy.init(args=args)

    image_publisher = ImagePublisher()

    rclpy.spin(image_publisher)

    image_publisher.destroy_node()
    rclpy.shutdown()


if __name__=='__main__':
    main()