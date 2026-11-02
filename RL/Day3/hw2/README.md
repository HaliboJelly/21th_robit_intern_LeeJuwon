# Humanoid-v5 PPO 비교 실험

Stable-Baselines3 PPO로 Humanoid-v5를 학습시키고, 제어 비용 가중치만 바꾼 두 설정을 비교한다.

## 실행 환경

- Ubuntu (Linux 7.0.0, x86_64), CPU만 사용
- Python 3.12.3
- 패키지 버전은 `requirements.txt` 참고
    - Gymnasium 1.4.0: https://github.com/Farama-Foundation/Gymnasium (release v1.4.0)
    - Stable-Baselines3 2.9.2: https://github.com/DLR-RM/stable-baselines3 (release v2.9.2)
    - MuJoCo 3.17.0
- 그래프의 한글은 NanumGothic 폰트를 사용한다. 없으면 `sudo apt install fonts-nanum`

```bash
pip install -r requirements.txt
```
