#include "Ship.h"
using namespace std;

const string Ship::NORTH = "NORTH";
const string Ship::EAST = "EAST";
const string Ship::SOUTH = "SOUTH";
const string Ship::WEST = "WEST";
const vector<string> Ship::ORIENTATIONS = {NORTH, EAST, SOUTH, WEST};

Ship::Ship() {
    name = "";
    length = 0;
    orientation = NORTH;
}

Ship::Ship(string shipName, int shipLength) {
    name = shipName;
    length = shipLength;
}

string Ship::GetName() const {
    return name;
}

int Ship::GetLength() const {
    return length;
}

string Ship::GetOrientation() const {
    return orientation;
}

void Ship::SetOrientation(string shipOrientation) {
    orientation = shipOrientation;
}

void Ship::SetCoord(const vector<int>& coord) {
    coordsList.push_back(coord);
}
    
void Ship::SetHitCoord(const vector<int>& coord) {
    hitCoords.push_back(coord);
}

vector<vector<int>> Ship::GetHitCoords() const {
    return hitCoords;
}

bool Ship::IsShipSunk() const {
    int equalCount = 0;
    
    for (int i = 0; i < hitCoords.size(); i++) {
        vector<int> hitCoord = hitCoords[i];

        for (int j = 0; j < coordsList.size(); j++) { // Compare the coordinate against all ship coordinates
            vector<int> shipCoord = coordsList[j];
            
            if (hitCoord == shipCoord) {
                equalCount++;
                break;
            }
        }
    }

    if (equalCount == length) {
        return true;
    } else {
        return false;
    }
}

vector<vector<int>> Ship::GetCoords() const {
    return coordsList;
}