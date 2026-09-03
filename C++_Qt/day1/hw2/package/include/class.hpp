#pragma once

struct Point {  // 구조체 정의
    int x;
    int y;
};

class MyClass
{
private:
    Point *arr;
    int n;  // 점입력개수
    int coor_max, coor_min; // 좌표의 최댓값과 최솟값
    double min_dist, max_dist;  // 거리 결과
    int min_x, min_y, max_x, max_y;     // 최솟값과 최댓값을 갖는 점의 좌표
    int min_index = 0;  //최솟값을 갖는 좌표의 번호
    int max_index = 0;  //최댓값을 갖는 좌표의 번호
    int min_index2 = 0, max_index2 = 0;  //최솟값과 최댓값을 갖는 좌표의 번호
public:
    MyClass();
    ~MyClass();
    void input();
    void calc();
    void print();
};