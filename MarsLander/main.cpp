#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// need simulator
// try use a GA to find parameters of Lander
// goal is near point in flat zone

int main()
{
	int numSurfPoints;
	int plainXStart, plainXEnd;
	std::cin >> numSurfPoints; std::cin.ignore();
	int* arrSurfY = new int[numSurfPoints];
	int* arrSurfX = new int[numSurfPoints];
	for (int i = 0; i < numSurfPoints; i++) {
		std::cin >> arrSurfX[i] >> arrSurfY[i]; std::cin.ignore();
		if (i > 0 && arrSurfY[i] == arrSurfY[i - 1] && arrSurfX[i] - arrSurfX[i - 1] >= 1000) {
			plainXStart = arrSurfX[i - 1];
			plainXEnd = arrSurfX[i];
			std::cerr << "Start Flat: " << plainXStart << "End Flat: " << plainXEnd << std::endl;
		}
	}

	// game loop
	while (1) {
		int X;
		int Y;
		int HS; // the horizontal speed (in m/s), can be negative.
		int VS; // the vertical speed (in m/s), can be negative.
		int F; // the quantity of remaining fuel in liters.
		int R; // the rotation angle in degrees (-90 to 90).
		int P; // the thrust power (0 to 4).
		std::cin >> X >> Y >> HS >> VS >> F >> R >> P; std::cin.ignore();

		// Write an action using std::cout. DON'T FORGET THE "<< std::endl"
		// To debug: cerr << "Debug messages..." << std::endl;


		// R P. R is the desired rotation angle. P is the desired thrust power.
		std::cout << "0 3" << std::endl;
	}
}