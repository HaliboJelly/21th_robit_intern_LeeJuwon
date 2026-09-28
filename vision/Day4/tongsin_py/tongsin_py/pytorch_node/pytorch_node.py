import time
import cv2
import cv_bridge
import rclpy
from rclpy.node import Node
from sensor_msgs.msg import Image
from ultralytics import YOLO

class UltralyticsNode(Node):
    def __init__(self):
        super().__init__("ultralytics")
        self.declare_parameter("model", "yolo26n.pt")
        self.declare_parameter("image_topic", "video_frames")
        self.declare_parameter("result_topic", "/ultralytics/detection/image")
        self.declare_parameter("target_ids", [-1])
        self.declare_parameter("class_names", [""])
        ids = list(self.get_parameter("target_ids").value)
        self.classes = None if -1 in ids else ids
        self.names = dict(enumerate(self.get_parameter("class_names").value))
        self.bridge = cv_bridge.CvBridge()
        self.model = YOLO(self.get_parameter("model").value)
        self.publisher = self.create_publisher(Image, self.get_parameter("result_topic").value, 5)
        self.create_subscription(Image, self.get_parameter("image_topic").value, self.callback, 10)

    def callback(self, message):
        t0 = time.time()
        image = self.bridge.imgmsg_to_cv2(message, desired_encoding="bgr8")
        r = self.model(image, classes=self.classes, device="cpu", verbose=False)[0]
        r.names = self.names
        annotated = r.plot()
        self.publisher.publish(self.bridge.cv2_to_imgmsg(annotated, encoding="bgr8"))
        cv2.imshow("yolo", annotated)
        cv2.waitKey(1)
        s = r.speed
        self.get_logger().info(f"pre {s['preprocess']:.1f} / inf {s['inference']:.1f} / post {s['postprocess']:.1f} / total {(time.time()-t0)*1000:.1f} ms", throttle_duration_sec=1.0)

def main(args=None):
    rclpy.init(args=args)
    node = UltralyticsNode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        pass
    finally:
        cv2.destroyAllWindows()
        node.destroy_node()
        rclpy.shutdown()

if __name__ == "__main__":
    main()