#include <iostream>
#include <string>
#include <math.h>
#include <vector>

#define PI 3.14159265358979323846
const double G = 3.711;

const int ANGLE_LIMIT = 15;
const int POWER_LIMIT = 1;
const unsigned numAngles = ANGLE_LIMIT * 2 + 1;
const unsigned numPowers = POWER_LIMIT * 2 + 1;

struct Params {
	int x;
	int y;
	int verticSpeed;
	int horizSpeed;
	Params() {}
	Params(int x, int y, int speedV, int speedH) : x(x), y(y), verticSpeed(speedV), horizSpeed(speedH) {}
	double getDist(const Params& params) {
		return sqrt((params.x - this->x)*(params.x - this->x) + (params.y - this->y)*(params.y - this->y));
	}
	double getDistSquare(const Params& params) {
		return (params.x - this->x)*(params.x - this->x) + (params.y - this->y)*(params.y - this->y);
	}
};

struct FlyParams {
	int angle;
	int power;
	FlyParams() {}
	FlyParams(int angle, int power) : angle(angle), power(power) {}
};

double deg2Rad(int angle) {
	return angle * PI / 180.0;
}

int boundBetween(int x, int min, int max) {
	if (x < min) return min;
	if (x > max) return max;
	return x;
}

Params computeNextPosition(const FlyParams& flyParams, const Params& currParams) {
	double accelV = cos(deg2Rad(flyParams.angle)) * (-flyParams.power) + G;
	double accelH = sin(deg2Rad(flyParams.angle)) * (-flyParams.power);
	int nextVS = currParams.verticSpeed + accelV;
	int nextHS = -currParams.horizSpeed + accelH;
	int x = currParams.x - nextVS;
	int y = currParams.y + nextHS;
	return Params(x, y, nextVS, nextHS);
}

std::vector<FlyParams> getAllPossibleCombOfFlyParams(FlyParams currFlyParams) {
	// здесь надо ограничить некорректные параметрыуыпкпк
	std::vector<FlyParams> flyParamsArr;
	for (int i = currFlyParams.power - POWER_LIMIT, ic= 0; i <= currFlyParams.power + POWER_LIMIT; i++, ic++) {
		for (int j = currFlyParams.angle - ANGLE_LIMIT, jc = 0; j <= currFlyParams.angle + ANGLE_LIMIT; j++, jc++) {
			flyParamsArr.push_back(FlyParams(boundBetween(j, -90, 90), boundBetween(i, 0, 4)));
		}
	}
	return flyParamsArr;
}

FlyParams findBestFutureParams(const std::vector<FlyParams>& allPossFlyParams, const Params& currPos, const Params& targetPos) {
	// unit c++ посмотреть чтобы сортировать как интегер а потом оставлять указатели
	double flyParamsScore = 0.0;
	double bestFlyParamsScore = 999999999.0;
	FlyParams flyParams;
	for (FlyParams fp : allPossFlyParams) {
		Params p = computeNextPosition(fp, currPos);
		flyParamsScore = p.getDistSquare(targetPos);			// пока только так считаем
		if (flyParamsScore < 4000000) {
			flyParamsScore += fabs(p.horizSpeed) * 20.0;
			flyParamsScore += fabs(p.verticSpeed) * 10.0;
			flyParamsScore += fabs(fp.angle) * 50.0;
			//std::cerr << "flyParamsScore: " << flyParamsScore << std::endl;
		}
		if (flyParamsScore < bestFlyParamsScore) {
			std::cerr << "flyParamsScore: " << flyParamsScore << " hSpeed: " << p.horizSpeed << " vSpeed: " << p.verticSpeed 
				<< " angle: " << flyParams.angle << " power: " << flyParams.power << std::endl;
			bestFlyParamsScore = flyParamsScore;
			flyParams = fp;
		}
	}
	return flyParams;
}

/*
надо считать на несколько тиков вперед из-за большой инертности
*/

int main()
{
	int numSurfPoints;
	int plainXStart, plainXEnd, plainY;
	std::cin >> numSurfPoints; std::cin.ignore();
	int* arrSurfY = new int[numSurfPoints];
	int* arrSurfX = new int[numSurfPoints];
	for (int i = 0; i < numSurfPoints; i++) {
		std::cin >> arrSurfX[i] >> arrSurfY[i]; std::cin.ignore();
		if (i > 0 && arrSurfY[i] == arrSurfY[i - 1] && arrSurfX[i] - arrSurfX[i - 1] >= 1000) {
			plainY = arrSurfY[i];
			plainXStart = arrSurfX[i - 1];
			plainXEnd = arrSurfX[i];
			//std::cerr << "Start Flat: " << plainXStart << "End Flat: " << plainXEnd << std::endl;
		}
	}

	// choose target point
	Params targetPos((plainXStart + plainXEnd) / 2.0, plainY, 0, 0);
	std::cerr << targetPos.x << " " << targetPos.y << std::endl;

	// game loop
	while (1) {
		Params currParams;
		int F; // the quantity of remaining fuel in liters.
		int R; // the rotation angle in degrees (-90 to 90).
		int P; // the thrust power (0 to 4).
		std::cin >> currParams.x >> currParams.y >> currParams.horizSpeed >> currParams.verticSpeed >> F >> R >> P; std::cin.ignore();

		// calculate optimal angle and power
		FlyParams flyParams = findBestFutureParams(getAllPossibleCombOfFlyParams(FlyParams(R, P)), currParams, targetPos);
		//std::cerr << flyParams.angle << " " << flyParams.power << std::endl;

		// R P. R is the desired rotation angle. P is the desired thrust power.
		std::cout << flyParams.angle << " " << flyParams.power << std::endl;
	}

	delete[] arrSurfX;
	delete[] arrSurfY;
}