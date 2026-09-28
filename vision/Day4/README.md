카메라 영상을 받아 YOLO26n(COCO 사전학습)으로 CPU 객체 탐지를 하고, 결과를 화면에 띄움

카메라 → cam_pub → /video_frames → pytorch_node(YOLO 추론) → /ultralytics/detection/image + 화면 표시



## 설치
sudo apt install python3-pip ros-jazzy-rmw-cyclonedds-cpp
pip3 install ultralytics "numpy<2" --break-system-packages

- `numpy<2`: apt로 설치된 cv_bridge가 numpy 1.x 기준이라 2.x면 import 에러가 난다.
- 카메라 권한이 없으면 `sudo usermod -aG video $USER` 후 재로그인.

## 실행
FastDDS는 raw 이미지 전송 시 몇 초씩 끊김 → CycloneDDS 사용

echo 'export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp' >> ~/.bashrc
source ~/.bashrc
 
cd ~/Documents/21th_robit_intern_LeeJuwon/vision
colcon build --packages-select tongsin_py
source install/setup.bash
ros2 launch tongsin_py tongsin.launch.py

