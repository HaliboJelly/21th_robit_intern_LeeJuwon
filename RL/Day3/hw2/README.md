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

## 설정

| 이름 | 설정 | 차이 |
|---|---|---|
| A (`base`) | 기본 설정 | `ctrl_cost_weight=0.1` (Humanoid-v5 기본값) |
| B (`ctrl001`) | 변경 설정 | `ctrl_cost_weight=0.01` |

나머지는 모두 같다.
- PPO: SB3 기본값 (learning_rate 3e-4, n_steps 2048, batch_size 64, n_epochs 10, gamma 0.99, gae_lambda 0.95, clip_range 0.2, ent_coef 0.0, vf_coef 0.5, max_grad_norm 0.5, MlpPolicy 64-64)
- 관측 정규화: 사용하지 않음
- 학습 예산: 100만 스텝 (실제 1,001,472)
- 학습 seed: 0, 1
- 평가: 20회, `deterministic=True`, 평가 seed 1000~1019, 두 조건 모두 기본 보상 환경에서 평가

## 실행 방법

`hw2` 폴더에서 실행한다.

```bash
# 1. 학습 + 평가 (실행 하나에 약 9~10분)
python RLppo_hw.py --config base --seed 0
python RLppo_hw.py --config base --seed 1
python RLppo_hw.py --config ctrl001 --seed 0
python RLppo_hw.py --config ctrl001 --seed 1

# 저장된 모델만 다시 평가
python RLppo_hw.py --config base --seed 0 --eval-only

```

`RLppo_hw.py` 옵션: `--config {base,ctrl001}`, `--seed`, `--steps` (기본 1000000), `--episodes` (기본 20), `--eval-only`

## 결과 파일

```
runs/{설정}_seed{번호}/
    model.zip            학습된 모델 (checkpoint)
    train.monitor.csv    학습 중 에피소드별 보상(r), 길이(l), 시간(t)
    eval.csv             평가 에피소드별 보상, 스텝, 종료 원인, 전진 속도, 제어 입력 크기, 보상 4항목
    summary.json         설정, 실제 환경 스텝, 학습 시간, 패키지 버전, 평가 평균·표준편차·최소·최대
results/
    learning_curve.png   학습 보상 (2만 스텝 구간 평균)
    episode_length.png   학습 중 에피소드 길이
    eval_compare.png     평가 보상 비교
    speed_vs_length.png  평가 에피소드별 유지 시간과 전진 속도
    reward_parts.png     평가 보상의 항목별 구성
    action_saturation.png 행동이 범위 끝(±0.4)에 붙은 비율
    analysis_log.txt     위 그래프의 수치
    videos/              모델별 대표 영상과 실패 사례, 선정 기준은 videos.txt
```