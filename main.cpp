#include <iostream>
#include <vector>
#include "vectorZ.h"

int main() {
	Vector C(2);
	C.push(33);
	C.push(22);
	Vector A(2);
	A.push(1);
	A.push(2);
	Vector D(2);
	D.push(1);
	D.push(20);
	C = D + A;
 	std::cout << C.pop() << " is popped" << std::endl;
	std::cout << C.pop() << " is popped" << std::endl;
	
	C = D * A;

   	std::cout << C.pop() << " MULTIPLE" << std::endl;
   	std::cout << C.pop() << " MULTIPLE" << std::endl;
	//C.pop();
	//std::vector<int> x(20);
	//x[3] = 4;

    Vector X(2), Y(3);
   // Y.push(10);
    X.push(10);
    Vector Z2(10);
    Vector Z1(Z2);
    std::cout << X.pop() << " is popped" << std::endl;
   // std::cout << "if X equal to Y?   " << (X == Y) << std::endl;
    X.push(20);
    X.push(30);
    X.push(-200);
    X.push(100);
    X.push(100);
    X.push(-10);
    X.push(-20);
    X.push(-30);
    X.push(252545523);
    X.push(300);

    X[0] = 20;

    X[1] = 10;


    X.push(311);
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
    std::cout << X.peek() << " is peeked" << std::endl;
    //X.peek() = 3;	
    //X.pop() =  3;
    Vector V(X);
    for(int i = 0; i < 10; i++)
    {
        V.push(i);
    }
    for(int i = 0; i < 10; i++)
    {
        V.pop();
    }
    X = V;
   for(int i = 0; i < 1000; i++)
    {
        V += X;
    }
    for(int i = 0; i < 1000; i++)
    {
        X *= V;
    }
    for(int i = 0; i < 1000; i++)
    {
	//
    }
    Vector Z(V);
	Z[0] = 3230;
	Z.push(3);
    Vector* z;
    z = NEW Vector;
    z -> push(33);   
    z -> push(40);
    z -> push(100);  
    for(int i(0); i < 3; i++)
    {	   
	std::cout << z -> pop() << " is popped" << std::endl;
    }	
    DELETE(z);
	return 0;
}
