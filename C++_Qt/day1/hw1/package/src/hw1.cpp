/*
클래스1개, 동적할당
*/
#include "../include/class.hpp"
#include <iostream>
using namespace std;

MyClass::MyClass(){
    arr = 0;
    n = 0;
    sum = 0;
    max = 0;
    min = 0;
    cout << "생성자 실행" << endl;
}

MyClass::~MyClass(){
    delete[] arr;
    cout << "소멸자 실행" << endl;
}


void MyClass::input() {
    cout << "몇 개의 원소를 할당하겠습니까? "<< endl;
    cin >> n;// 정수입력개수

    if(n<=0){   
        cout << "0보다 큰 정수를 입력하세요."<< endl;
        return;
    }

    arr = new int[n]; // 동적할당

    if(!arr) {
        cout << "메모리에 할당할 수 없음"<< endl;
        return;
    }

    for(int i=0; i<n; i++){
        cout << "정수형 데이터 입력:"<< endl; //프롬프트출력
        cin >> arr[i];  //정수입력
    }
}

void MyClass::calc() {
    if(n<=0) {
        cout << "계산할 데이터가 없습니다." << endl;
        return;
    }

    sum = 0;
    max = arr[0];
    min = arr[0];

    for(int i=0; i<n; i++){
        sum += arr[i];  //전체합
        if(arr[i] > max) {  //최댓값
            max = arr[i];
        }
        if(arr[i] < min) {  //최솟값
            min = arr[i];
        }
    }
}

void MyClass::print() {
    if(n<=0) {
        cout << "출력할 데이터가 없습니다." << endl;
        return;
    }

    cout << "최댓값: " << max << endl;
    cout << "최솟값: " << min << endl;
    cout << "전체합: " << sum << endl;
    cout << "평균: " << (double)sum/n << endl;
}


int main() {

    MyClass obj;

    obj.input();
    obj.calc();
    obj.print();

    return 0;
}
