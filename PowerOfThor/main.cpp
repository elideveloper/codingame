#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

const unsigned HEIGHT = 18;
const unsigned WIDTH = 40;


double boundBetween(int min, int max, int val)
{
	if (val < min) return min;
	if (val > max) return max;
	return val;
}


class Map {
	double** tiles;
	int currX;
	int currY;

public:
	Map(unsigned x, unsigned y) {
		this->tiles = new double*[HEIGHT];
		for (int i = 0; i < HEIGHT; i++) {
			this->tiles[i] = new double[WIDTH];
		}
		this->currX = x;
		this->currY = y;
	}
	~Map() {
		for (int i = 0; i < HEIGHT; i++) {
			delete[] this->tiles[i];
		}
		delete[] this->tiles;
	}
	void unset() {
		for (int y = 0; y < HEIGHT; y++) {
			for (int x = 0; x < WIDTH; x++) {
				this->tiles[y][x] = 0.0;
			}
		}
	}
	void distributeLinearCharge(int tileX, int tileY, double charge, double subVal, unsigned manhRadius)
	{
		int mDist = 0;
		int yManh = 0;
		for (int y = 0; y < HEIGHT; y++) {
			yManh = abs(y - tileY);
			if (manhRadius > 0 && yManh > manhRadius) {
				continue;
			}
			for (int x = 0; x < WIDTH; x++) {
				mDist = abs(x - tileX) + yManh;
				if (manhRadius > 0 && mDist > manhRadius) {
					continue;
				}
				this->tiles[y][x] += charge - subVal * mDist;
			}
		}
	}
	string findBestStep() {
		unsigned serachRadius = 1;
		double bestScore = 0.0;
		unsigned bestX = this->currX;
		unsigned bestY = this->currY;
		string direction;
		for (int y = boundBetween(0, HEIGHT - 1, this->currY - serachRadius); y <= boundBetween(0, HEIGHT - 1, this->currY + serachRadius); y++) {
			for (int x = boundBetween(0, WIDTH - 1, this->currX - serachRadius); x <= boundBetween(0, WIDTH - 1, this->currX + serachRadius); x++) {
				if (this->tiles[y][x] > bestScore) {
					bestScore = this->tiles[y][x];
					bestX = x;
					bestY = y;
				}
			}
		}
		if (bestY > this->currY) {
			direction = "S";	// S
		}
		else if (bestY < this->currY) {
			direction = "N";	// N
		}
		if (bestX < this->currX) {
			direction += "W";	// W
		}
		else if (bestX > this->currX) {
			direction += "E";	// E
		}
		this->currX = bestX;
		this->currY = bestY;
		return direction;
	}

};



int main()
{
	int lightX; // the X position of the light of power
	int lightY; // the Y position of the light of power
	int initialTX; // Thor's starting X position
	int initialTY; // Thor's starting Y position
	cin >> lightX >> lightY >> initialTX >> initialTY; cin.ignore();

	Map m(initialTX, initialTY);
	double charge = 1000.0;
	m.unset();
	m.distributeLinearCharge(lightX, lightY, charge, charge / (WIDTH + HEIGHT), WIDTH + HEIGHT);
	

	while (1) {
		int remainingTurns; // The remaining amount of turns Thor can move. Do not remove this line.
		cin >> remainingTurns; cin.ignore();
		// A single line providing the move to be made: N NE E SE S SW W or NW
		cout << m.findBestStep() << endl;
	}
}