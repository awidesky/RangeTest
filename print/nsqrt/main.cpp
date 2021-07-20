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
		float t = fabs(1.0 - (f(ff, magic) / g(ff)));
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
1fbb67a2 : 0.000601172
1fbb67a3 : 0.000601172
1fbb67a4 : 0.000601053
1fbb67a5 : 0.000601053
1fbb67a6 : 0.000601053
1fbb67a7 : 0.000601053
1fbb67a8 : 0.000601053
1fbb67a9 : 0.000601053
1fbb67aa : 0.000601053
1fbb67ab : 0.000601053
1fbb67ac : 0.000601053
1fbb67ad : 0.000601053
1fbb67ae : 0.000601053
1fbb67af : 0.000601053
1fbb67b0 : 0.000601053
1fbb67b1 : 0.000601053
1fbb67b2 : 0.000601053
1fbb67b3 : 0.000601053
1fbb67b4 : 0.000601053
1fbb67b5 : 0.000601053
1fbb67b6 : 0.000601053
1fbb67b7 : 0.000601053
1fbb67b8 : 0.000601053
1fbb67b9 : 0.000601053
1fbb67ba : 0.000601053
1fbb67bb : 0.000601053
1fbb67bc : 0.000601053
1fbb67bd : 0.000601053
1fbb67be : 0.000601053
1fbb67bf : 0.000601053
1fbb67c0 : 0.000601053
1fbb67c1 : 0.000601053
1fbb67c2 : 0.000601053
1fbb67c3 : 0.000601053
1fbb67c4 : 0.000601053
1fbb67c5 : 0.000601053
1fbb67c6 : 0.000601053
1fbb67c7 : 0.000601053
1fbb67c8 : 0.000601053
1fbb67c9 : 0.000601053
1fbb67ca : 0.000601172
1fbb67cb : 0.000601172
*/
int main() {
    //testMagicI(0x1fbb4f00,0x1fbb4fff); //0x1fbb7ff0);
    auto fu = [](int i) -> Point<int, float> { return testOne<float, int>(fsqrt_, sqrtf, i, "one sqrt itration done : "); };
    std::cout << "hello!\n";
    auto r = testRange<Point<int, float>, decltype(fu), decltype(min<Point<int, float>>)>(0x1fbb67a2, 0x1fbb67cc, fu, min, 1);
    std::cout << "\nBest magic number is : " << std::hex << r.magic << "\nAnd maximum relative error is : " << std::dec << r.err << std::endl;
}