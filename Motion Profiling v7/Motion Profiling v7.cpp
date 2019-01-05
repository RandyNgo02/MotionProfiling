// Motion Profiling v7.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include "math.h"
#include <iostream>

#include "Data.h"

using namespace std;

double pi = 3.14159265359;

double max_velocity, width, driftiness, angle;

double fresnelSineCalc(double input) {
	return fresnelSineData[int(input / increment)] * (1 - (abs(input / increment) - int(abs(input / increment)))) + fresnelSineData[int(input / increment) + 1] * (abs(input / increment) - int(abs(input / increment)));
}

double fresnelCosineCalc(double input) {
	return fresnelCosineData[int(input / increment)] * (1 - (abs(input / increment) - int(abs(input / increment)))) + fresnelCosineData[int(input / increment) + 1] * (abs(input / increment) - int(abs(input / increment)));
}

double getDirection(double x, double y) {
	if (x < 0) {
		return (3 * pi / 2) - atan(y / x);
	} else if (x > 0) {
		return (pi / 2) - atan(y / x);
	} else {
		if (y > 0) {
			return 0;
		} else {
			return pi;
		}
	}
}

double calcFirstPathX(double s, double t) {
	return max_velocity * sqrt((t * s) / angle) * fresnelSineCalc(sqrt((angle * s) / t)) + (width / driftiness) * cos((angle * s) / t) - (width / driftiness);
}

double calcFirstPathY(double s, double t) {
	return max_velocity * sqrt((t * s) / angle) * fresnelCosineCalc(sqrt((angle * s) / t)) - (width / driftiness) * sin((angle * s) / t);
}

double calcInversePathX(double s, double t) {
	return max_velocity * sqrt((t * (t - s)) / angle) * fresnelSineCalc(sqrt((angle * (t - s)) / t)) + (width / driftiness) * cos((angle * (t - s)) / t) - (width / driftiness);
}

double calcInversePathY(double s, double t) {
	return max_velocity * sqrt((t * (t - s)) / angle) * fresnelCosineCalc(sqrt((angle * (t - s)) / t)) - (width / driftiness) * sin((angle * (t - s)) / t);
}

double calcPathX(double skew, double duration) {
	return calcFirstPathX(skew, duration) - (sqrt(pow(calcInversePathX(skew, duration), 2) + pow(calcInversePathY(skew, duration), 2)) * cos(atan(calcInversePathY(skew, duration) / calcInversePathX(skew, duration)) + angle));
}

double calcPathY(double skew, double duration) {
	return calcFirstPathY(skew, duration) + (sqrt(pow(calcInversePathX(skew, duration), 2) + pow(calcInversePathY(skew, duration), 2)) * sin(atan(calcInversePathY(skew, duration) / calcInversePathX(skew, duration)) + angle));
}

int main() {
	double x1, y1, x2, y2, x3, y3;
	cout << "Max Velocity:";
	cin >> max_velocity;
	cout << "Width:";
	cin >> width;
	cout << "Driftiness:";
	cin >> driftiness;
	cout << "x1:";
	cin >> x1;
	cout << "y1:";
	cin >> y1;
	cout << "x2:";
	cin >> x2;
	cout << "y2:";
	cin >> y2;
	cout << "x3:";
	cin >> x3;
	cout << "y3:";
	cin >> y3;
	double a = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	double b = sqrt(pow((x3 - x2), 2) + pow((y3 - y2), 2));
	double c = sqrt(pow((x3 - x1), 2) + pow((y3 - y1), 2));
	double a2 = getDirection(x2 - x1, y2 - y1);
	double a3 = getDirection(x3 - x1, y3 - y1);
	double xf = c * sin(a3 - a2);
	double yf = c * cos(a3 - a2);
	angle = acos(((a * a) + (b * b) - (c * c)) / (2 * a * b));
	int sign;
	if (xf < 0) {
		sign = -1;
	} else {
		sign = 1;
	}
	double best_t = 0;
	double best_s = 0;
	angle = pi - angle;
	double time = (2 * angle * width) / (max_velocity * driftiness);
	double skewness = 0;
	double error = pow(calcPathX(0, (2 * angle * width) / (max_velocity * driftiness)) - xf, 2) + pow(calcPathY(0, (2 * angle * width) / (max_velocity * driftiness)) - yf, 2);
	while (time <= 15) {
		skewness = 0;
		while (skewness <= time) {
			if (pow(calcPathX(skewness, time) - xf, 2) + pow(calcPathY(skewness, time) - yf, 2) < error) {
				error = pow(calcPathX(skewness, time) - xf, 2) + pow(calcPathY(skewness, time) - yf, 2);
				best_t = time;
				best_s = skewness;
			}
			skewness += 0.01;
		}
		time += 0.01;
	}
	cout << endl << "Time: " << best_t << ", Skewness:" << best_s;
	//cout << endl << "Angle: " << sign * angle * 180 / pi;
	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file