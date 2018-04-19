#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
    // game loop
    while (1) {
        int highestIndex = 0;
        int highestValue = 0;
        for (int i = 0; i < 8; i++) {
            int mountainH; // represents the height of one mountain.
            cin >> mountainH; cin.ignore();
            if (mountainH > highestValue) {
                highestIndex = i;
                highestValue = mountainH;
            }
        }

        cout << to_string(highestIndex) << endl; // The index of the mountain to fire on.
    }
}