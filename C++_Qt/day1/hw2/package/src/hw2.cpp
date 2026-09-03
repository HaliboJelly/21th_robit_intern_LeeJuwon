/*

cout 순서
점 개수->최솟값->최댓값

*/
#include "../include/class.hpp"
#include <iostream>
#include <math.h>
#include <cstdlib>
#include <ctime>
using namespace std;


MyClass::MyClass(){
    arr = 0;
    n = 0;
    coor_max = 0;
    coor_min = 0;
    min_dist = 0;
    max_dist = 0;
    min_x = 0;
    min_y = 0;
    max_x = 0;
    max_y = 0;
    min_index = 0;
    max_index = 0;
    min_index2 = 0;
    max_index2 = 0;
    cout << "start constructor" << endl;
}

MyClass::~MyClass(){
    delete[] arr;
    cout << "소멸자 실행" << endl;
}

void MyClass::input() {
    cout << "Please define the number of points: \n";
    cin >> n;// 점입력개수

    cout << "Please define minimum of coordinate. value: \n";
    cin >> coor_min;// 최솟값

    cout << "Please define maximum of coordinate. value: \n";
    cin >> coor_max;// 최댓값
    cout << "\n\n\n";


//예외처리
    if(n<=0){   
        cout << "enter a positive integer greater than 0.";
        return;
    }
    if(coor_min<=0){   
        cout << "enter a positive integer greater than 0.";
        return;
    }
    if(coor_max<=0){   
        cout << "enter a positive integer greater than 0.";
        return;
    }
    if(coor_min >= coor_max){   
        cout << "the minimum value must be less than the maximum value."<< endl;
        return;
    }


    arr = new Point[n]; // 동적할당

    if(!arr) {
        cout << "failed to allocate memory.";
        return;
    }

}

void MyClass::calc() {      // 최대 최소 거리 계산  
    if(n<=0) {
        cout << "failed to allocate memory.";
        return;
    }

    for(int i=0; i<n; i++){
        arr[i].x = rand() % (coor_max - coor_min + 1) + coor_min; // 난수 생성
        arr[i].y = rand() % (coor_max - coor_min + 1) + coor_min; // 난수 생성
    }


    for (int i = 0; i < n; i++) {   //거리 계산 후 max와 min을 갖는 좌표와 번호를 저장
        for (int j = i + 1; j < n; j++) {
            double dist = sqrt(pow(arr[i].x - arr[j].x, 2) + pow(arr[i].y - arr[j].y, 2));
            if (dist < min_dist || min_dist == 0) {
                min_dist = dist;
                min_index = i;
                min_index2 = j;
                }
            if (dist > max_dist) {
                max_dist = dist;
                max_index = i;
                max_index2 = j;
            }
        }
    }

}

void MyClass::print() {
    if(n<=0) {
        cout << "failed to allocate memory.";
        return; 
    }

    cout << "Generated Random points\n";
    for (int i = 0; i < n; i++) {
        cout << "Point " << i + 1<<". " << arr[i].x << " , " << arr[i].y << "\n";
    }

    cout << "\n\n\n";
    cout << "--------Results--------\n";
    cout << "MinDist: " << min_dist << "\n";
    cout << "Pair of Min Coor. (x,y): P1(" << arr[min_index].x << ", " << arr[min_index].y << ") & P2(" << arr[min_index2].x << ", " << arr[min_index2].y << ")\n";
    cout << "MaxDist: " << max_dist << "\n";
    cout << "Pair of Max Coor. (x,y): P1(" << arr[max_index].x << ", " << arr[max_index].y << ") & P2(" << arr[max_index2].x << ", " << arr[max_index2].y << ")\n";
    cout << "===================COMPLETE===================\n";
}


int main() {
    
    srand(time(NULL)); // 난수 초기화

    MyClass obj; //생성

    obj.input();
    obj.calc();
    obj.print();

    return 0;
}
