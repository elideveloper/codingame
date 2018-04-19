#include <iostream>
#include <string>
#include <math.h>
#include <vector>

#define PI 3.14159265358979323846
const double G = 3.711;

const int MIN_POWER = 0;
const int MAX_POWER = 4;
const int MAX_ANGLE = 90;
const int MIN_ANGLE = -90;
const int ANGLE_STEP = 15;
const int POWER_STEP = 1;


struct Params {
	int x;
	int y;
	int verticSpeed;
	int horizSpeed;
	Params() {}
	Params(int x, int y, int speedV, int speedH) : x(x), y(y), verticSpeed(speedV), horizSpeed(speedH) {}
	double getDist(const Params& params) const {
		return sqrt((params.x - this->x)*(params.x - this->x) + (params.y - this->y)*(params.y - this->y));
	}
	double getDistSquare(const Params& params) const {
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
	double accelV = cos(deg2Rad(flyParams.angle)) * (flyParams.power) - G;
	double accelH = -sin(deg2Rad(flyParams.angle)) * (flyParams.power);
	int nextVS = currParams.verticSpeed + static_cast<int>(accelV);
	int nextHS = currParams.horizSpeed + static_cast<int>(accelH);
	int x = currParams.x + nextHS;
	int y = currParams.y + nextVS;
	return Params(x, y, nextVS, nextHS);
}

std::vector<FlyParams> getAllPossibleCombOfFlyParams(FlyParams currFlyParams) {
	std::vector<FlyParams> flyParamsArr;
	for (int i = boundBetween(currFlyParams.power - POWER_STEP, MIN_POWER, MAX_POWER); i <= boundBetween(currFlyParams.power + POWER_STEP, MIN_POWER, MAX_POWER); i += POWER_STEP) {
		for (int j = boundBetween(currFlyParams.angle - ANGLE_STEP, MIN_ANGLE, MAX_ANGLE); j <= boundBetween(currFlyParams.angle + ANGLE_STEP, MIN_ANGLE, MAX_ANGLE); j += ANGLE_STEP) {
			flyParamsArr.push_back(FlyParams(j, i));
		}
	}
	return flyParamsArr;
}

FlyParams findBestFutureParams(const std::vector<FlyParams>& allPossFlyParams, const Params& currPos, const Params& targetPos) {
	double flyParamsScore = 0.0;
	double bestFlyParamsScore = 99999999.0;
	FlyParams flyParams;
	double currDist = currPos.getDist(targetPos);
	if (currDist > 2000.0) {
		std::cerr << "Far away" << std::endl;
		for (FlyParams fp : allPossFlyParams) {
			Params p = computeNextPosition(fp, currPos);
			flyParamsScore = p.getDistSquare(targetPos);
			if (flyParamsScore < bestFlyParamsScore) {
				std::cerr << "flyParamsScore: " << flyParamsScore << " hSpeed: " << p.horizSpeed << " vSpeed: " << p.verticSpeed
					<< " angle: " << flyParams.angle << " power: " << flyParams.power << std::endl;
				bestFlyParamsScore = flyParamsScore;
				flyParams = fp;
			}
		}
	}
	else {
		std::cerr << "Near target" << std::endl;
		for (FlyParams fp : allPossFlyParams) {
			Params p = computeNextPosition(fp, currPos);
			flyParamsScore = 10 * abs(p.verticSpeed);
			flyParamsScore += 10 * abs(p.horizSpeed);
			flyParamsScore += abs(fp.angle);
			if (flyParamsScore < bestFlyParamsScore) {
				std::cerr << "flyParamsScore: " << flyParamsScore << " hSpeed: " << p.horizSpeed << " vSpeed: " << p.verticSpeed
					<< " angle: " << flyParams.angle << " power: " << flyParams.power << std::endl;
				bestFlyParamsScore = flyParamsScore;
				flyParams = fp;
			}
		}
	}
	return flyParams;
}

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
		//std::cerr << "Curr x: " << currParams.x << "Curr y: " << currParams.y << "Curr speed_x: " << currParams.horizSpeed << "Curr speed_y: " << currParams.verticSpeed << std::endl;

		// calculate optimal angle and power
		FlyParams flyParams = findBestFutureParams(getAllPossibleCombOfFlyParams(FlyParams(R, P)), currParams, targetPos);
		//std::cerr << flyParams.angle << " " << flyParams.power << std::endl;

		//Params params = computeNextPosition(flyParams, currParams);
		//std::cerr << "Next x: " << params.x << "Next y: " << params.y << "Next speed_x: " << params.horizSpeed << "Next speed_y: " << params.verticSpeed << std::endl;

		// R P. R is the desired rotation angle. P is the desired thrust power.
		std::cout << flyParams.angle << " " << flyParams.power << std::endl;
	}

	delete[] arrSurfX;
	delete[] arrSurfY;
}