import gymnasium as gym
import torch
from torch import nn
from collections import deque
import random
import numpy as np

random.seed(0)
np.random.seed(0)
torch.manual_seed(0)

env = gym.make("CartPole-v1")
env.action_space.seed(0)

def make_net():
    return nn.Sequential(
        nn.Linear(4, 64), nn.ReLU(),
        nn.Linear(64, 64), nn.ReLU(),
        nn.Linear(64, 2))
online = make_net()
target = make_net()
target.load_state_dict(online.state_dict())
target.requires_grad_(False)

memory = deque(maxlen=20000)

# before training (once)
optimizer = torch.optim.Adam(online.parameters(), lr=5e-4)
loss_fn = nn.SmoothL1Loss()

def select_action(state, epsilon):
    if random.random() < epsilon:
        return env.action_space.sample()
    with torch.no_grad():
        x = torch.as_tensor(state).unsqueeze(0)
        return online(x).argmax(dim=1).item()

def optimize_batch():
    batch = random.sample(memory, 64)
    states, actions, rewards, next_states, terms = zip(*batch)
    S = torch.tensor(np.array(states), dtype=torch.float32)
    A = torch.tensor(actions, dtype=torch.int64).unsqueeze(1)
    R = torch.tensor(rewards, dtype=torch.float32)
    NS = torch.tensor(np.array(next_states), dtype=torch.float32)
    T = torch.tensor(terms, dtype=torch.float32)

    prediction = online(S).gather(1, A).squeeze(1)
    with torch.no_grad():
        next_q = target(NS).max(dim=1).values
        y = R + 0.99 * (1 - T) * next_q

    loss = loss_fn(prediction, y)
    optimizer.zero_grad()
    loss.backward()
    nn.utils.clip_grad_norm_(online.parameters(), 10)
    optimizer.step()

state, _ = env.reset(seed=0)
for step in range(1, 40001):
    epsilon = max(0.05, 1 - 0.95 * (step - 1) / 15000)
    action = select_action(state, epsilon)
    ns, r, term, trunc, _ = env.step(action)
    memory.append((state.copy(), action, r, ns.copy(), term))
    if len(memory) >= 1000 and step % 4 == 0:
        optimize_batch()
    if step % 500 == 0:
        target.load_state_dict(online.state_dict())
    state = ns
    if term or trunc:
        state, _ = env.reset()

torch.save(online.state_dict(), "dqn.pt")
model = make_net()
model.load_state_dict(torch.load("dqn.pt", weights_only=True))
model.eval()

eval_returns = []
for ep in range(50):
    state, _ = env.reset(seed=1000 + ep)
    total = 0.0
    done = False
    while not done:
        with torch.no_grad():
            x = torch.as_tensor(state).unsqueeze(0)
            action = model(x).argmax(dim=1).item()
        state, r, term, trunc, _ = env.step(action)
        total += r
        done = term or trunc
    eval_returns.append(total)
print("eval mean", np.mean(eval_returns))
env.close()

render_env = gym.make("CartPole-v1", render_mode="human")
state, _ = render_env.reset(seed=1000)
total = 0.0
done = False
while not done:
    with torch.no_grad():
        x = torch.as_tensor(state).unsqueeze(0)
        action = model(x).argmax(dim=1).item()
    state, r, term, trunc, _ = render_env.step(action)
    total += r
    done = term or trunc
print("render return", total)
render_env.close()
