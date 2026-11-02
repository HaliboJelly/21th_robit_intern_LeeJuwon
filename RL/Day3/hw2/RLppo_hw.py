import argparse
import csv
import json
import platform
import time
from pathlib import Path

import gymnasium as gym
import mujoco
import numpy as np
import stable_baselines3 as sb3
import torch
from stable_baselines3 import PPO
from stable_baselines3.common.monitor import Monitor

HERE = Path(__file__).resolve().parent

# A: 기본 설정 / B: 제어 비용 가중치만 0.1 -> 0.01
CONFIGS = {
    "base": {"ctrl_cost_weight": 0.1},
    "ctrl001": {"ctrl_cost_weight": 0.01},
}
EVAL_SEED = 1000


def evaluate(model, episodes):
    # 두 조건 모두 기본 보상 정의의 환경에서 평가해야 총보상을 비교할 수 있다
    env = gym.make("Humanoid-v5")
    dt = env.unwrapped.dt
    rows = []
    for ep in range(episodes):
        obs, info = env.reset(seed=EVAL_SEED + ep)
        x_start = info["x_position"]
        com_distance = 0.0
        total = 0.0
        parts = {"reward_survive": 0.0, "reward_forward": 0.0, "reward_ctrl": 0.0, "reward_contact": 0.0}
        action_sq = 0.0
        steps = 0
        done = False
        while not done:
            action, _ = model.predict(obs, deterministic=True)
            obs, r, term, trunc, info = env.step(action)
            total += r
            com_distance += info["x_velocity"] * dt
            for key in parts:
                parts[key] += info[key]
            action_sq += np.mean(action ** 2)
            steps += 1
            done = term or trunc
        rows.append({
            "eval_seed": EVAL_SEED + ep,
            "return": total,
            "steps": steps,
            "end": "넘어짐" if term else "시간 제한",
            # 전진 보상과 같은 기준인 무게중심 속도, 몸통 속도는 넘어질 때 앞으로 쏠리는 값까지 포함된다
            "forward_speed": com_distance / (steps * dt),
            "torso_speed": (info["x_position"] - x_start) / (steps * dt),
            "action_sq_mean": action_sq / steps,
            **parts,
        })
    env.close()
    return rows


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--config", choices=CONFIGS, default="base")
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--steps", type=int, default=1_000_000)
    parser.add_argument("--episodes", type=int, default=20)
    parser.add_argument("--eval-only", action="store_true")
    args = parser.parse_args()

    torch.set_num_threads(1)
    run_dir = HERE / "runs" / f"{args.config}_seed{args.seed}"
    run_dir.mkdir(parents=True, exist_ok=True)

    if args.eval_only:
        summary = json.loads((run_dir / "summary.json").read_text(encoding="utf-8"))
        model = PPO.load(run_dir / "model", device="cpu")
        rows = evaluate(model, args.episodes)
        save_eval(run_dir, rows, summary, args.episodes)
        return

    env = Monitor(gym.make("Humanoid-v5", **CONFIGS[args.config]), str(run_dir / "train"))
    print("obs", env.observation_space.shape, "action", env.action_space.shape,
          "range", env.action_space.low[0], env.action_space.high[0])

    model = PPO("MlpPolicy", env, seed=args.seed, device="cpu", verbose=1)
    start = time.time()
    model.learn(total_timesteps=args.steps)
    train_time = time.time() - start
    model.save(run_dir / "model")
    env.close()

    model = PPO.load(run_dir / "model", device="cpu")
    rows = evaluate(model, args.episodes)

    summary = {
        "config": args.config,
        "env_kwargs": CONFIGS[args.config],
        "seed": args.seed,
        "env_steps": model.num_timesteps,
        "train_time_sec": round(train_time, 1),
        "versions": {
            "python": platform.python_version(),
            "os": platform.platform(),
            "cpu": platform.processor() or platform.machine(),
            "stable_baselines3": sb3.__version__,
            "gymnasium": gym.__version__,
            "mujoco": mujoco.__version__,
            "torch": torch.__version__,
        },
    }
    print(f"{args.config} seed {args.seed}: {model.num_timesteps} steps, {train_time:.0f}s")
    save_eval(run_dir, rows, summary, args.episodes)


def save_eval(run_dir, rows, summary, episodes):
    with open(run_dir / "eval.csv", "w", newline="", encoding="utf-8") as f:
        writer = csv.DictWriter(f, fieldnames=rows[0].keys())
        writer.writeheader()
        writer.writerows(rows)

    summary["eval_episodes"] = episodes
    for key in ["return", "steps", "forward_speed", "torso_speed", "action_sq_mean"]:
        values = [row[key] for row in rows]
        summary[key] = {"mean": float(np.mean(values)), "std": float(np.std(values)),
                        "min": float(min(values)), "max": float(max(values))}
    summary["fell"] = sum(row["end"] == "넘어짐" for row in rows)
    (run_dir / "summary.json").write_text(json.dumps(summary, ensure_ascii=False, indent=2), encoding="utf-8")

    print(f"eval {episodes}회 return 평균 {summary['return']['mean']:.1f} "
          f"(표준편차 {summary['return']['std']:.1f}), 유지 {summary['steps']['mean']:.0f} 스텝, "
          f"넘어짐 {summary['fell']}/{episodes}, 무게중심 전진 속도 {summary['forward_speed']['mean']:.2f} m/s")


if __name__ == "__main__":
    main()
