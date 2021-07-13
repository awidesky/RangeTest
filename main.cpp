#include <cmath>
#include <iostream>
#include <stdio.h>
#include <string>
#include <chrono>
#include "Point.hpp"
#include "testRange.hpp"

float fsqrt_(float f, int magic) {
	int i = *(int *)&f;
	i = (i >> 1) + magic;//0x1fbb4f2d;//0x1fbb7fec;
	float f1 = *(float *)&i;
	//return 0.5F * (f1 + f / f1);
	 return f1;
}

template <typename T, typename M>
Point<M,T> testOne(T (*f)(T, int), T (*g)(T), M magic, std::string st) {
	Point<M,T> p{magic, T(0.0)};
	long cnt = 0;
	T max = (T)0.0;
	//T in = (T)0.0;
	double total = 0.0;
	for (T ff = (T)0.2;; ff = std::nextafter(ff, (T)10.6)) {
		// for(int i = 1; i > 0 ; i++) {
		// float ff = *(int*)&i;
		// std::cout << *(float*)&i<< "\n";
		double t = fabs((f(ff, magic) / g(ff)) - 1.0);
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
	std::cout << std::string("\n").append(st); //<< magic;// << "\navg :" << total / cnt << "\nmax : " << max << "\nin : "<< in;
    return p;
}

template <typename T>
T min(T a, T b) {
  return (a < b) ? a : b;
}

/*
Best magic number is : 1fbb4f2d
And maximum relative error is : 0.0347475
16267.6 ms in i5-4590S $3.00GHz, 4GB ram
*/
int main() {
    //testMagicI(0x1f800000, 0x1fffffff);
    auto fu = [](int i) -> Point<int, float> { return testOne<float, int>(fsqrt_, sqrtf, i, "one sqrt itration done : "); };
    std::cout << "hello!\n";
    auto start = std::chrono::steady_clock::now();
    auto r = testRange<Point<int, float>, decltype(fu), decltype(min<Point<int, float>>)>(0x1fbb4f00, 0x1fbb4fff, fu, min, 4);
    auto end = std::chrono::steady_clock::now();
    std::cout << "\nBest magic number is : " << r.magic << "\nAnd maximum relative error is : " << r.err << std::endl;
    auto diff = end - start;
    std::cout << "\n" << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
}