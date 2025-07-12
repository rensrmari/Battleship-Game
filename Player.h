#ifndef PLAYER_H
#define PLAYER_H
#include "Ship.h"
#include <vector>
#include <string>
using namespace std;

class Player {
public:
    Player();
    Player(string playerName, bool isComputer, bool isClassic, bool isHardmode);

    static const int MAX_SHIPS = 5;
    static const int MAX_SIZE = 10;
    
    /**
     * Gets player name.
     * @return Player name.
     */
    string GetName() const;

    /**
     * Allows player to place down MAX_SHIPS down on the board.
     */
    void GenerateShips();

    /**
     * Finds out how many ships were destroyed.
     * @return Number of ships destroyed in the board.
     */
    int GetShipsDestroyed() const;

    /**
     * It is player's turn: prompt coordinates to call numTurns times and records and announces the result of the shot.
     * @param opponent Opponent of player.
     */
    void Turn(Player& opponent);

    /**
     * Gets the number of hits the player has gotten.
     * @return Number of hits.
     */
    int GetHitCount() const;

    /**
     * Gets the number of calls the player has done.
     * @return Number of calls.
     */
    int GetCallCount() const;

    /**
     * Gets accuracy of the player by dividing turnCount by hitCount.
     * @return Accuracy as a double.
     */
    double GetAccuracy() const;
    
    /**
     * Deletes all ship objects.
     */
    void DeleteShips();
private:
    const int ROW_INDEX = 0;
    const int COL_INDEX = 1;
    const int NUM_DIMENSIONS = 2;
    const char STARTING_LETTER = 'A';
    const int TEMP_INT = -2;
    const int NOTHING_INT = -1;
    const int MISS_INT = 0;
    const int HIT_INT = 1;
    const int SHIP_INT = 2;
    const int SUNKEN_INT = 3;

    string name;
    bool isComputer;
    bool isClassic;

    bool isHardmode;
    struct SearchData {
        vector<int> startPos;
        vector<int> currPos;
        vector<int> futurePos;
        vector<vector<int>> previousPos;
        vector<string> possibleDirs;
        string currDir = "";
        bool currDirSuccess = true;
        int spacesLeft = -1;
        int priority = -1;
    };
    vector<SearchData> searches;
    vector<Ship> shipsHit;

    int numCalls;
    char letters[MAX_SIZE];
    vector<vector<int>> offensiveBoard;
    vector<vector<int>> shipBoard;
    bool settingUpShips;
    int callCount;
    int hitCount;
    vector<vector<int>> prevTurnHits;
    vector<Ship*> ships;
    int shipsDestroyed;
    int shipsLost;

    /**
     * Assigns all the letters in order to LETTERS (A-J).
     */
    void MakeLetters();

    /**
     * Sets all elements of board to -1 (nothing).
     */
    void GenerateEmptyBoard(vector<vector<int>>& board);

    /**
     * Displays a battleship board with its name.
     * @param board The board of a player.
     * @param info Additional info about the board.
     * @param showLost Whether or not the number of ships the player has lost will be shown.
     */
    void DisplayBoard(const vector<vector<int>>& board, string info, bool showLost) const;

    /**
     * Sets a coordinate of player's ship board to a particular value.
     * @param row Row of coordinate.
     * @param col Column of coordinate.
     * @param value Value to be assigned to the coordinate.
     */
    void SetShipBoard(int row, int col, int value);

    /**
     * Gets the player's board containing the ships.
     * @return Vector in vector representing battleship grid.
     */
    vector<vector<int>> GetShipBoard() const;

    /**
     * Returns the ships of the player.
     */
    vector<Ship*> GetShips();

    /**
     * Makes a ship with given data on the board and assigns the coordinates to Ship object too.
     * @param name Name of the ship.
     * @param length Length of ship.
     * @param coord Row and column in vector.
     * @param orientation Orientation of ship.
     */
    Ship* CreateShip(string name, int length, const vector<int>& coord, string orientation);

    /**
     * Gets the number of ship spaces opponent has left.
     * @param opponent Opponent of current player.
     * @return Number of spaces total ships are occupying.
     */
    int GetShipSpacesLeft(Player& opponent);

    /**
     * Gets the coordinates that were hit in the previous turn.
     * @return A vector of the previous coordinates that were hit.
     */
    vector<vector<int>> GetHitsPrev() const;

    /**
     * Resets the counter for times the player has hit their opponent's ships previously.
     */

    void ResetHitsPrev();

    /**
     * Gets the number of ships player has lost.
     * @return Number of ships lost.
     */
    int GetShipsLost() const;

    /**
     * Increases the number of ships the player has lost by one.
     */
    void IncShipsLost();

    /**
     * Decrease the player's number of turns for salvo gamemode.
     * @return Decrease message in a string.
     */
    string LowerTurn();

    /**
     * Determines if a certain coordinate is valid.
     * @param board Board to be checked.
     * @param coord Coordinate to check in a vector of row and column.
     * @return Whether or not the space is valid.
     */
    bool IsValidCoord(const vector<vector<int>>& board, const vector<int>& coord) const;
    /**
     * Determines if a certain coordinate allows valid coordinates within a given length and direction when setting up ships.
     * @param board The board to check.
     * @param coord Coordinate to check in a vector of row and column.
     * @param length Length.
     * @param direction Direction.
     * @return Whether or not the space is valid.
     */
    bool IsValidCoord(const vector<vector<int>>& board, const vector<int>& coord, int length, string direction) const;

    /**
     * Prompts user for coordinates within the grid.
     * @return Coordinate represented as row and column in vector.
     */
    vector<int> PromptUserCoord() const;

    /**
     * Creates random coordinates for the computer.
     * If the computer is in hardmode, coordinates adjacent to previous hits (not surrounded by previous shots) will be chosen.
     * @return Coordinate represent as row and column in vector.
     */
    vector<int> PromptComputerCoord();

    /**
     * Creates a random coordinate within the board.
     * @return A random coordinate.
     */
    vector<int> GetRandCoord() const;

    /**
     * Checks the possible directions that the coordinate allows.
     * @param coord A coordinate whose sides are being checked.
     * @return All the valid directions of the search.
     */
    vector<string> GetPossibleDirs(const vector<int>& coord) const;

    /**
     * Gets a random direction out of the possible directions.
     * @param sd The search data whose directions are being required.
     * @return A random direction.
     */
    string GetRandDir(const SearchData sd) const;

    /**
     * Removes the current direction being used in the search, changing it, and resets the current position to the start.
     * @param sd The search data to modify.
     */
    void FinishDir(SearchData& sd);

    /**
     * Gets the coordinate in the specified direction.
     * @param coord Coordinate to begin at.
     * @param direction Direction of the next coordinate.
     * @return The coordinate at the specified direction.
     */
    vector<int> GetNext(const vector<int>& coord, string direction) const;

    /**
     * Picks a random search data's index.
     * @return Search's index.
     */
    int GetRandSearchData() const;

    /**
     * Finds the search data with the coordinate.
     * @param coord A coordinate to check.
     * @return A search's index.
     */
    int GetSearchData(const vector<int>& coord) const;

    /**
     * Assigns the length of a particular ship to a search.
     */
    void AssignShipsToSearch();

    /**
     * Decreases all search priorities.
     */
    void UpdPriorities();

    /**
     * Takes action upon receiving an inputted coordinate.
     * @param coord Vector of row and column values that is on the board.
     * @param opponent Opponent of the player.
     * @return The result of the coordinate call.
     */
    string ShootCoord(const vector<int>& coord, Player& opponent);

    /**
     * Prompts user for orientation of ship.
     * @return Orientation of ship as string.
     */
    string PromptUserOrientation() const;
};

#endif