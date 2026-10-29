#README파일을 먼저 읽어주세요

#필요한 라이브러리 다운
import torch
from torch import nn
from torch.utils.data import TensorDataset
import numpy as np
import struct
import os
import matplotlib.pyplot as plt
import random

device = "cuda" if torch.cuda.is_available() else "cpu"

#시드 설정
random.seed(777)
torch.manual_seed(777)
if device == "cuda":
    torch.cuda.manual_seed_all(777)

#파라미터 설정
training_epochs = 15  #에포크 설정(에포크는 총 학습횟수를 나타냄)
batch_size = 100    #배치 설정(배치란 학습을 진행할 때 한 번에 처리하는 데이터의 양을 의미함)
learning_rate = 0.1 #학습률 설정하기 (기본적인 경사하강법을 이용하기에 그 점을 유의할 것 *SDG)

print("학습 에포크 수: ", training_epochs)
print("배치 사이즈:", batch_size)
print("학습률:", learning_rate)


# MNIST dataset
DATA_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'archive.')

def load_idx_images(path):
    with open(path, 'rb') as f:
        _, num, rows, cols = struct.unpack('>IIII', f.read(16))
        data = np.frombuffer(f.read(), dtype=np.uint8).reshape(num, 1, rows, cols)
    return torch.tensor(data, dtype=torch.float32) / 255.0 

def load_idx_labels(path):
    with open(path, 'rb') as f:
        _, num = struct.unpack('>II', f.read(8))
        data = np.frombuffer(f.read(), dtype=np.uint8)
    return torch.tensor(data, dtype=torch.long)

mnist_train = TensorDataset(load_idx_images(os.path.join(DATA_DIR, 'train-images.idx3-ubyte')),
                            load_idx_labels(os.path.join(DATA_DIR, 'train-labels.idx1-ubyte')))

mnist_test = TensorDataset(load_idx_images(os.path.join(DATA_DIR, 't10k-images.idx3-ubyte')),
                           load_idx_labels(os.path.join(DATA_DIR, 't10k-labels.idx1-ubyte')))

# dataset loader
data_loader = torch.utils.data.DataLoader(dataset=mnist_train, #어떤 데이터 로드할거냐
                                          batch_size=batch_size, #몇개씩 잘라서 들고올래
                                          shuffle=True, #100개 불러올 때 무작위로 불러올래?
                                          drop_last=True) #100개 씩 불러올 때 남는 게 있는데 남는 것 버릴래?


model = nn.Sequential(
    nn.Flatten(),
    nn.Linear(784, 128), #28*28=784개의 입력값을 128개의 출력값으로 변환
    nn.ReLU(),
    nn.Linear(128, 10) #128개의 입력값을 10개의 출력값으로 변환
).to(device)

criterion = nn.CrossEntropyLoss()
optimizer = torch.optim.SGD(model.parameters(), lr=learning_rate) #기본적인 경사하강법 이용 SDG (Adam과 다름)

#mini-betch 학습 loop
for epoch in range(training_epochs):
    model.train() #학습모드
    total_loss = 0 #총 손실값
    for X, Y in data_loader: #데이터 로더에서 X,Y를 불러옴 // X는 이미지, Y는 라벨
        X = X.to(device)
        Y = Y.to(device)

        optimizer.zero_grad() #기울기 초기화
        logits = model(X) #모델에 X를 넣어 예측값을 구함
        loss = criterion(logits, Y) #예측값과 실제값을 비교하여 손실값 계산
        loss.backward() #손실값을 기준으로 역전파 수행
        optimizer.step() #가중치 갱신

        total_loss += loss.item() * X.size(0) #손실값을 배치 사이즈만큼 곱하여 총 손실값에 더함

    train_loss = total_loss / len(data_loader.dataset)
    print("에포크: ", "%04d" %(epoch + 1), "    loss=", "{:.9f}".format(train_loss))


#검증
model.eval() #평가모드
with torch.no_grad(): #test set으로 데이터를 다룰 때에는 gradient를 주면 안된다
    X_test = mnist_test.tensors[0].to(device) #이미지
    Y_test = mnist_test.tensors[1].to(device)   #라벨

    prediction = model(X_test)
    test_loss = criterion(prediction, Y_test)
    print('test loss:', test_loss.item())
    correct_prediction = torch.argmax(prediction, 1) == Y_test #결과값이랑 실제값이랑 같은지 확인
    accuracy = correct_prediction.float().mean() #평균으로 전체 정확도 확인
    print('accuracy:', accuracy.item())
