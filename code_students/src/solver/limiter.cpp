#include "solver/limiter.hpp"

#include <algorithm>
#include <iostream>

limiter_base::limiter_base() {}

limiter_minmod::limiter_minmod(double theta) { this->theta = theta; }

double limiter_minmod::compute(double first, double second, double third) {
	int sign;
	double min;
	if(first < 0 && second < 0 && third < 0){
		sign = -1;
		min = std::min(std::abs(first),std::abs(second));
		min = std::min(std::abs(min),std::abs(third));
		return sign * min;
	}else if (first > 0 && second > 0 && third > 0) {
		sign = 1;
		min = std::min(std::abs(first),std::abs(second));
		min = std::min(std::abs(min),std::abs(third));
		return sign*min;
	}else {
		return 0;
	}
}