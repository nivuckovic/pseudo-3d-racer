#pragma once
#include <math.h>
#include <cstdlib>

inline void clamp(float& x) {
	if (isinf(x)) {
		x = 10000.f;
	}
}

inline float project(float d, float dx, float dz, float offset = 0, float curveOffset = 0) {
	return offset + curveOffset + d * dx / dz;
}

inline float calculateScaleLN(float X) {
	float sol = -log(X) * 5.f;

	return (sol > 0) ? sol : 0;
}

inline double calculateScaleSideCar(float X) {
	double sol = 1.5f + pow(1 / (X * 3.f), 2);
	int max = 30;

	return (sol < max) ? sol : max;
}

inline float calculateScaleObject(float X) {
	return pow(X - 1, 2) / 0.15f + 0.5f;
}

inline float easeIn(float a, float b, float percent) {
	return a + (b - a) * pow(percent, 2);
}

inline float easeOut(float a, float b, float percent) {
	return a + (b - a) * (1 - pow(1 - percent, 2));
}

inline double easeInOut(float a, float b, float percent) {
	return a + (b - a) * (-(cos(percent * 3.14) / 2) + 0.5f);
}

inline float calculateScale(float dy, float coef) {
	return exp(dy / coef);
}

inline float randomNumber(float a, float b) {
	return a + (float)std::rand() / (float)RAND_MAX * (b - a);
}

inline float degToRad(int deg) {
	return deg * (3.14f / 180.f);
}