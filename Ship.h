#ifndef SHIP_H
#define SHIP_H
#include <string>
#include <vector>
using namespace std;

class Ship {
public:
    static const int DESTROYER_HP = 2;
    static const int SUBMARINE_HP = 3;
    static const int CRUISER_HP = 3;
    static const int BATTLESHIP_HP = 4;
    static const int CARRIER_HP = 5;
    static const string NORTH;
    static const string EAST;
    static const string SOUTH;
    static const string WEST;
    static const vector<string> ORIENTATIONS;

    Ship();
    Ship(string shipName, int shipLength);

    /**
     * Gets name of ship.
     * @return Ship name.
     */
    string GetName() const;

    /**
     * Gets length of ship.
     * @return Ship length.
     */
    int GetLength() const;

    /** 
     * Gets orientation of ship.
     * @return Ship orientation.
     */
    string GetOrientation() const;

    /**
     * Sets orientation of ship.
     * @param orientation Orientation of ship.
     */
    void SetOrientation(string shipOrientation);

    /**
     * Adds coordinate to let a ship occupy space on the board.
     * @param coord Coordinate of ship in vector of row, col.
     */
    void SetCoord(const vector<int>& coord);

    /**
     * Sets a coordinate to hit.
     * @param coord Coordinate to hit.
     */
    void SetHitCoord(const vector<int>& coord);

    /**
     * Gets all hit coordinates.
     * @return Vector of a vector<int> representing all hit coordinates.
     */
    vector<vector<int>> GetHitCoords() const;

    /**
     * Checks if the ship is sunk by comparing hitCoords to coordsList.
     */
    bool IsShipSunk() const;

    /**
     * Get all the coordinates of the ship.
     * @return Vector of row, col values all in one vector.
     */
    vector<vector<int>> GetCoords() const;
private:
    string name;
    int length;
    string orientation;
    vector<string> orientations;
    vector<vector<int>> coordsList;
    vector<vector<int>> hitCoords;
    
    /**
     * Makes all possible orientations as elements of orientations.
     */
    void MakeOrientations();
};
#endif