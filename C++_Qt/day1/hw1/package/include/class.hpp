#pragma once


class MyClass
{
private:
    int *arr;
    int n;
    int sum, max, min;
public:
    MyClass();
    ~MyClass();
    void input();
    void calc();
    void print();
};