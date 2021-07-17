#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <thread>
#include <future>
#include "Point.hpp"
#include "testRange.hpp"

float fsqrt_(float f, int magic) {
	int i = *(int *)&f;
	i = (i >> 1) + magic;//0x1fbb4f2d;//0x1fbb7fec;
	float f1 = *(float *)&i;
	return 0.5F * (f1 + f / f1);
	//return f1;
}

double fsqrt__(double d, int magic) {
	float f = (float)d;
	int i = *(int *)&f;
	i = (i >> 1) + magic;//0x1fbb4f2d;//0x1fbb7fec;
	float f1 = *(float *)&i;
	return 0.5 * (f1 + f / f1);
	//return f1;
}

template <typename T, typename M>
MyPoint<M,T> testOne(T (*f)(T, int), T (*g)(T), M magic, std::string st) {
	MyPoint<M,T> p{magic, T(0.0)};
	long cnt = 0;
	T max = (T)0.0;
	//T in = (T)0.0;
	double total = 0.0;
	for (T ff = (T)0.2;; ff = std::nextafter(ff, (T)10.6)) {
		// for(int i = 1; i > 0 ; i++) {
		// float ff = *(int*)&i;
		// std::cout << *(float*)&i<< "\n";
		double t = abs(1.0 - ((double)f(ff, magic) / g(ff)));
		if (max <= t){
			max = t;
			//in = ff;
		}
		// std::cout<<t<<"\n";
		cnt++;
		total += t;

		if (ff - (T)3.5 > (T)0.0001) {
			break;
		}
	}
	p.err = max;
	std::cout << std::hex << p.magic << " : " << std::dec << p.err << "\n";
	//std::cout << std::string("\n").append(st); //<< magic;// << "\navg :" << total / cnt << "\nmax : " << max << "\nin : "<< in;
    return p;
}

template <typename T>
T min(T a, T b) {
  return (b < a) ? b : a;
}
/*
Best magic number is : 1fbb4f2d
And maximum relative error is : 0.0347475
16267.6 ms in i5-4590S $3.00GHz, 4GB ram
*/
int main() {
    //testMagicI(0x1fbb4f00,0x1fbb4fff); //0x1fbb7ff0);
    auto fu = [](int i) -> MyPoint<int, double> { return testOne<double, int>(fsqrt__, sqrt, i, "one sqrt itration done : "); };
    std::cout << "hello!\n";
    auto r = testRange<MyPoint<int, double>, decltype(fu), decltype(min<MyPoint<int, double>>)>(0x1fbb67a2, 0x1fbb67cc, fu, min, 1);
    std::cout << "\nBest magic number is : " << std::hex << r.magic << "\nAnd maximum relative error is : " << std::dec << r.err << std::endl;
}