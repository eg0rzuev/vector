#include <iostream>
#include "vectorZ.h"
int main() {
    std::cout << "Hello, World!" << std::endl;
    Vector X;
    X.push(10);
    X.push(20);
    X.push(30);
    X.push(-200);
    X.push(100);
    X.push(100);
    X.push(-10);
    X.push(-20);
    X.push(-30);
    X.push(0);
    X.push(300);
    X[0] = 1;
    X.push(11);
  //  std::cout << X[0] << std::endl;
  //  std::cout << X[1] << std::endl;
  //  std::cout << X[-2] << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;
    std::cout << X.pop() << " is popped" << std::endl;


    return 0;
}