#필요한 라이브러리 다운
import gymnasium as gym
import numpy as np
import matplotlib.pyplot as plt
import time

#파라미터 설정
num_episodes = 5000  #학습 에피소드 수 (에피소드란 시작부터 종료·중단까지의 한 번의 시행)
alpha = 0.1          #학습률 (새로운 경험을 Q 값에 얼마나 반영할지)
gamma = 0.95         #할인율 (미래 보상을 얼마나 반영할지, 0이면 즉시 보상만 봄)
eps_decay = 0.999    #엡실론 감소 속도 (epsilon = eps_decay ** episode, 1에 가까울수록 천천히 감소)
eps_min = 0.05       #엡실론 최솟값 (학습 후반에도 남겨둘 최소 탐험 확률)
is_slippery = False  #미끄러짐 여부 (True면 선택한 방향과 양옆으로 1/3씩 이동)
seed = 0             #시드 설정

print("에피소드 수:", num_episodes)
print("학습률:", alpha)
print("할인율:", gamma)
print("엡실론 감소 속도:", eps_decay, "/ 최솟값:", eps_min)
print("미끄러짐:", is_slippery)

#환경 및 q테이블 준비
env = gym.make("FrozenLake-v1",
map_name="4x4", is_slippery=is_slippery)
rng = np.random.default_rng(seed)
Q = np.zeros((env.observation_space.n,
env.action_space.n))
returns = []

#행동 선택 함수
def choose_action(state, epsilon, rng=rng):
    if rng.random() < epsilon:
        return int(rng.integers(4))
    best = np.flatnonzero(
        Q[state] == Q[state].max())
    return int(rng.choice(best))

#경험 수집과 q 갱신
for episode in range(num_episodes):
    state, _ = env.reset(seed=seed if episode == 0 else None)
    epsilon = max(eps_min, eps_decay ** episode)
    total = 0
    while True:
        action = choose_action(state, epsilon)
        ns, r, terminated, truncated, _ = env.step(action)
        target = r if terminated else r + gamma * Q[ns].max()
        Q[state, action] += alpha * (target - Q[state, action])
        total += r
        state = ns
        if terminated or truncated:
            break
    returns.append(total)
np.save("q_table.npy", Q)

#학습 평가와 분리
Q = np.load("q_table.npy")
eval_env = gym.make("FrozenLake-v1", is_slippery=is_slippery)
eval_rng = np.random.default_rng(1000)
wins = 0
lengths = []
for ep in range(200):
    state, _ = eval_env.reset(seed=1000 if ep == 0 else None)
    steps = 0
    while True:
        action = choose_action(state, 0.0, eval_rng)
        state, r, term, trunc, _ = eval_env.step(action)
        steps += 1
        if term or trunc:
            if r == 1:
                wins += 1
                lengths.append(steps)
            break
print("성공률:", wins / 200)
print("평균 성공 경로 길이:", np.mean(lengths) if lengths else "-")
eval_env.close()
env.close()

show_env = gym.make("FrozenLake-v1", is_slippery=is_slippery, render_mode="human")
state, _ = show_env.reset(seed=1000)
while True:
    state, r, term, trunc, _ = show_env.step(choose_action(state, 0.0, eval_rng))
    if term or trunc:
        break
time.sleep(3)
show_env.close()

#학습 성공률 곡선 (500 episode 구간 평균)
block = 500
rates = np.array(returns).reshape(-1, block).mean(axis=1)
episodes = np.arange(block, len(returns) + 1, block)
plt.plot(episodes, rates, marker="o", color="#7b1e2b")
plt.ylim(0, 1)
plt.xticks(episodes)
plt.xlabel("Episode")
plt.ylabel("Success rate")
plt.title(f"Training success rate (alpha={alpha}, gamma={gamma}, decay={eps_decay})")
plt.savefig("training_success_rate.png", dpi=150)
plt.show()
