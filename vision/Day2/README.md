# ball_distance_pkg

노란 공을 HSV로 검출하고, 단안 카메라 거리 추정 두 가지(방법 A: 실제 크기 기반, 방법 B: 지면 평면 가정)로 거리를 계산하는 패키지

## 주의

`insta360_usb_cam` 패키지를 지우지 말아주세요.
카메라 노드 역할을 하는 패키지로, `ball_distance_pkg`의 launch 파일이 이 패키지의 launch를 함께 실행합니다. 없으면 영상과 카메라 정보(`/camera1/info`)를 받을 수 없어 실행되지 않습니다.

## 실행 방법

```bash
source install/setup.bash
ros2 launch ball_distance_pkg camera_ui.launch.py
```
