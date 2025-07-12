#include "Player.h"
#include "Ship.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <cmath>
using namespace std;

Player::Player() {
    name = "";
    numCalls = 1;
    isClassic = false;
    isHardmode = false;

    MakeLetters();
    GenerateEmptyBoard(offensiveBoard);
    GenerateEmptyBoard(shipBoard);
    settingUpShips = true;
    callCount = 0;
    hitCount = 0;
    shipsDestroyed = 0;
    shipsLost = 0;
}

Player::Player(string playerName, bool isComputer, bool isClassic, bool isHardmode) {
    name = playerName;
    this->isComputer = isComputer;
    isClassic ? numCalls = 1 : numCalls = MAX_SHIPS;
    this->isClassic = isClassic;
    this->isHardmode = isHardmode;

    MakeLetters();
    GenerateEmptyBoard(offensiveBoard);
    GenerateEmptyBoard(shipBoard);
    settingUpShips = true;
    callCount = 0;
    hitCount = 0;
    shipsDestroyed = 0;
    shipsLost = 0;
}

void Player::DeleteShips() {
    for (int i = 0; i < MAX_SHIPS; i++) {
        delete ships[i];
        ships[i] = nullptr;
    }
}

string Player::GetName() const {
    return name;
}

void Player::MakeLetters() {
    int rowCount = 0;
    for (char row = STARTING_LETTER; row < (STARTING_LETTER + MAX_SIZE); row++) {
        letters[rowCount] = row;
        rowCount++;
    }
}

void Player::GenerateEmptyBoard(vector<vector<int>>& board) {
    for (int i = 0; i < MAX_SIZE; i++) {
        vector<int> emptyRow;

        for (int j = 0; j < MAX_SIZE; j++) {
            emptyRow.push_back(NOTHING_INT);
        }

        board.push_back(emptyRow);
    }
}

void Player::SetShipBoard(int row, int col, int value) {
    shipBoard[row][col] = value;
}

vector<vector<int>> Player::GetShipBoard() const {
    return shipBoard;
}

void Player::DisplayBoard(const vector<vector<int>>& board, string info, bool showLost) const {
    const char NOTHING_MARKER = '*';
    const char MISS_MARKER = '-';
    const char HIT_MARKER = 'x';
    const char SHIP_MARKER = 'O';
    const char SUNKEN_MARKER = '~';
    const char TOP = '-';
    const char SIDE = '|';

    const int NUMBER_OFFSET_L = 4;
    const int NUMBER_OFFSET_R = 2;
    const int NUMBER_SPACING = 2;
    const int LETTER_OFFSET = 1;
    const int GRID_SPACING = 2;
    const int BOARD_PADDING = 15;

    cout << "\n\n";

    // Creates the name, ship destroyed indicator, top bar, numbering 1-10,.
    string numbers = "";
    string topBottomBar = "";
    string numberOffsetL(NUMBER_OFFSET_L, ' ');
    string numberOffsetR(NUMBER_OFFSET_R, ' ');
    string numberSpacing(NUMBER_SPACING, ' ');
    numbers += numberOffsetL;

    for (int i = 1; i <= MAX_SIZE; i++) {
        numbers += to_string(i);
        (i != 10) ? numbers += numberSpacing : numbers += numberOffsetR; // Add offset to end as well
    }

    for (int i = 0; i < numbers.length() + 2; i++) { // Accounts for the sides (additional spacing)
        topBottomBar += TOP;
    }

    string description = name + "'s " + info;
    string shipsInfotring = "";

    if (!showLost) { // Display # of ships the player has taken instead of ships lost
        shipsInfotring = "Ships Taken: " + to_string(shipsDestroyed);
    } else {
        shipsInfotring = "Ships Lost: " + to_string(shipsLost);
    }
    
    const int SHIPS_DESTROYED_SPACING = topBottomBar.length() - shipsInfotring.length();

    cout << left << setw(BOARD_PADDING) << "" << setw(SHIPS_DESTROYED_SPACING) << description << shipsInfotring << "\n";
    cout << setw(BOARD_PADDING) << "" << topBottomBar << "\n";
    cout << setw(BOARD_PADDING) << "" << SIDE << numbers << SIDE << "\n";

    // Creates letters A-J and grid.
    string letterOffset(LETTER_OFFSET, ' ');
    string gridSpacing(GRID_SPACING, ' ');

    for (int row = 0; row < MAX_SIZE; row++) {
        string gridRow = "";
        cout << setw(BOARD_PADDING) << "" << SIDE << letterOffset << letters[row];
        
        for (int col = 0; col < MAX_SIZE; col++) {
            int coordinate = board[row][col];
            
            gridRow += gridSpacing;
            if (coordinate == MISS_INT) {
                gridRow += MISS_MARKER;
            } else if (coordinate == HIT_INT) {
                gridRow += HIT_MARKER;
            } else if (coordinate == SHIP_INT) {
                gridRow += SHIP_MARKER;
            } else if (coordinate == SUNKEN_INT) {
                gridRow += SUNKEN_MARKER;
            } else {
                gridRow += NOTHING_MARKER;
            }
        }
        
        while (gridRow.length() + 1 < numbers.length() - 1) { // Align right side of grid by adding needed spaces
            gridRow += " ";
        }

        cout << gridRow;
        cout << SIDE << "\n";
    }

    cout << setw(BOARD_PADDING) << "" << topBottomBar << "\n\n";
    cout << right;
}

void Player::GenerateShips() {
    // Instantiate the five types of ships in the game.
    vector<Ship> tempShips;

    Ship destroyer("Destroyer", Ship::DESTROYER_HP);
    tempShips.push_back(destroyer);
    Ship submarine("Submarine", Ship::SUBMARINE_HP);
    tempShips.push_back(submarine);
    Ship cruiser("Cruiser", Ship::CRUISER_HP);
    tempShips.push_back(cruiser);
    Ship battleship("Battleship", Ship::BATTLESHIP_HP);
    tempShips.push_back(battleship);
    Ship carrier("Carrier", Ship::CARRIER_HP);
    tempShips.push_back(carrier);
    
    // Let user/computer come up with coordinates and orientations to use.
    for (int i = 0; i < MAX_SHIPS; i++) {
        Ship tempShip = tempShips[i];
        string shipName = tempShip.GetName();
        int shipLength = tempShip.GetLength();
        vector<int> coord;
        string orientation;

        bool valid = false;
        if (!isComputer) {
            do {
                cout << "\n< " << shipName << " (" << shipLength << " spaces) >";
                coord = PromptUserCoord();
                orientation = PromptUserOrientation();
                
                if (IsValidCoord(shipBoard, coord, shipLength, orientation)) {
                    valid = true;
                } else {
                    cout << "Sorry, invalid ship.\n";
                }
            } while (!valid);
            cout << "\nShip set!\n";
        } else {
            do {
                coord = PromptComputerCoord();
                orientation = Ship::ORIENTATIONS[rand() % Ship::ORIENTATIONS.size()];

                if (IsValidCoord(shipBoard, coord, shipLength, orientation)) {
                    valid = true;
                }
            } while (!valid);
        }

        ships.push_back(CreateShip(shipName, shipLength, coord, orientation));
        DisplayBoard(shipBoard, "Ships", true);
    }

    settingUpShips = false;
}

vector<Ship*> Player::GetShips() {
    return ships;
}

Ship* Player::CreateShip(string name, int length, const vector<int>& coord, string orientation) {
    Ship* tempShip = new Ship(name, length);
    tempShip->SetOrientation(orientation);
    tempShip->SetCoord(coord);

    int row = coord[ROW_INDEX];
    int col = coord[COL_INDEX];

    shipBoard[row][col] = SHIP_INT;
    for (int i = 1; i < length; i++) {
        if (orientation == Ship::NORTH) {
            row--;
        } else if (orientation == Ship::EAST) {
            col++;
        } else if (orientation == Ship::SOUTH) {
            row++;
        } else {
            col--;
        }
        
        vector<int> newCoord;
        newCoord.push_back(row);
        newCoord.push_back(col);
        tempShip->SetCoord(newCoord);
        shipBoard[row][col] = SHIP_INT;
    }
    
    return tempShip;
}

int Player::GetShipSpacesLeft(Player& opponent) {
    int difference = 0;
    vector<Ship*> allShips = opponent.GetShips();
    vector<vector<int>> allShipCoords;
    vector<vector<int>> allShipHitCoords;

    for (int i = 0; i < MAX_SHIPS; i++) { // Gets all coordinates of ships
        Ship currShip = *allShips[i];

        for (vector<int> shipCoord : currShip.GetCoords()) {
            allShipCoords.push_back(shipCoord);
        }

        for (vector<int> shipHitCoord : currShip.GetHitCoords()) {
            allShipHitCoords.push_back(shipHitCoord);
        }
    }

    difference = allShipCoords.size() - allShipHitCoords.size();
    return difference;
}

vector<vector<int>> Player::GetHitsPrev() const {
    return prevTurnHits;
}

void Player::ResetHitsPrev() {
    prevTurnHits.clear();
}

int Player::GetShipsLost() const {
    return shipsLost;
}

void Player::IncShipsLost() {
    shipsLost++;
}

void Player::Turn(Player& opponent) {
    if (opponent.GetShipsDestroyed() < MAX_SHIPS) { // Check if there are still shots to take
        // Output status and prompt for user's coordinate.
        DisplayBoard(shipBoard, "Ships", true);
        DisplayBoard(offensiveBoard, "Shots", false);
        cout << "\nIt is Captain " << name << "'s turn. You have " << numCalls << " call" << ((numCalls > 1) ? "s." : ".") << "\n";
        
        vector<vector<int>> opponentHits = opponent.GetHitsPrev();
        int prevTimesHit = opponentHits.size();
        if (prevTimesHit > 0) {
            cout << "WARNING: " << opponent.GetName() << " has hit your fleet at";
            
            // Convert row ints to chars.
            vector<char> rowChars;
            for (vector<int> coord : opponentHits) {
                char rowChar;
                for (int i = 0; i < MAX_SIZE; i++) {
                    if (coord[ROW_INDEX] == i) {
                        rowChar = letters[i];
                        rowChars.push_back(rowChar);
                        break;
                    }
                }
            }

            for (int i = 0; i < prevTimesHit; i++) {
                vector<int> coord = opponentHits[i];
                cout << " " << rowChars[i] << to_string(coord[COL_INDEX] + 1);
                
                if (prevTimesHit > 1 && i == prevTimesHit - 2) {
                    cout << " and";
                }

                if (i < prevTimesHit - 2) {
                    cout << ",";
                }
            }
            cout << ".\n";
            opponent.ResetHitsPrev();
        } else {
            cout << "NOTE: " << opponent.GetName() << " has not hit your fleet.\n";
        }

        vector<vector<int>> playerCoords;
        vector<int> playerCoord;
        int boardValue;

        // Prompts player numCalls times, checking if their coordinates haven't already been called on the player's board.
        int currHitCounter = 0;
        
        for (int i = 0; i < numCalls; i++) {
            if (currHitCounter == GetShipSpacesLeft(opponent)) { // Player has already hit all their opponent's ships, so break loop
                break;
            }

            bool isPreviousCoord = true;

            do {
                isPreviousCoord = true;

                playerCoord = (isComputer) ? PromptComputerCoord() : PromptUserCoord();
                boardValue = offensiveBoard[playerCoord[ROW_INDEX]][playerCoord[COL_INDEX]];

                if (boardValue != MISS_INT && boardValue != HIT_INT && boardValue != SUNKEN_INT) { // See if coordinate was input before turn
                    isPreviousCoord = false;

                    for (int j = 0; j < playerCoords.size(); j++) { // See if the coordinate was already input in the turn
                        if (playerCoords[j] == playerCoord) {
                            isPreviousCoord = true;
                            break;
                        }
                    }
                }
                
                if (!isComputer && isPreviousCoord) {
                    cout << "You cannot call a previous coordinate, Captain.\n";
                }
            } while (isPreviousCoord);
            if (opponent.GetShipBoard()[playerCoord[ROW_INDEX]][playerCoord[COL_INDEX]] == SHIP_INT) {
                currHitCounter++;
            }
            offensiveBoard[playerCoord[ROW_INDEX]][playerCoord[COL_INDEX]] = TEMP_INT;
            playerCoords.push_back(playerCoord);
        }

        // Reset temporary states related to hardmode searching.
        for (int i = 0; i < MAX_SIZE; i++) {
            for (int j = 0; j < MAX_SIZE; j++) {
                if (offensiveBoard[i][j] == TEMP_INT) {
                    offensiveBoard[i][j] = NOTHING_INT;
                }
            }
        }

        for (SearchData sd : searches) {
            sd.priority = sd.spacesLeft;
        }

        // Checks the result of the coordinate call after the player has called all coordinates in their turns.
        string result = "\n";
        for (vector<int> coord : playerCoords) {
            result += ShootCoord(coord, opponent);
        }
        
        DisplayBoard(offensiveBoard, "Shots", false);
        cout << result;
    }
}

string Player::LowerTurn() {
    if (numCalls > 0 && !isClassic) {
        numCalls--;
        return "\nThe number of coordinates " + name + " can call has been decreased.";
    } else {
        return "";
    }
}

bool Player::IsValidCoord(const vector<vector<int>>& board, const vector<int>& coord) const {
    int row = coord[ROW_INDEX];
    int col = coord[COL_INDEX];

    if ((row >= 0 && row <= MAX_SIZE - 1) && (col >= 0 && col <= MAX_SIZE - 1)) { // Within bounds
        if (board[row][col] == NOTHING_INT) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

bool Player::IsValidCoord(const vector<vector<int>>& board, const vector<int>& coord, int length, string direction) const {
    int row = coord[ROW_INDEX];
    int col = coord[COL_INDEX];
    
    if (IsValidCoord(board, coord)) { // Within bounds and empty
        for (int i = 0; i < length; i++) { // Iterate over grid and check values after first checking if out of bounds
            int boardValue = board[row][col];

            if (boardValue == SHIP_INT) {
                return false;
            }

            if (direction == Ship::NORTH) {
                if (row - 1 < 0 && i < length - 1) { // Make sure that coordinate isn't on its last iteration -- it's still valid
                    return false;
                }
                row--;
            } else if (direction == Ship::EAST && i < length - 1) {
                if (col + 1 >= MAX_SIZE) {
                    return false;
                }
                col++;
            } else if (direction == Ship::SOUTH && i < length - 1) {
                if (row + 1 >= MAX_SIZE) {
                    return false;
                }
                row++;
            } else {
                if (col - 1 < 0 && i < length - 1) {
                    return false;
                }
                col--;
            }
        }

        return true;
    } else {
        return false;
    }
}

vector<int> Player::PromptUserCoord() const {
    vector<int> coord;
    bool valid = false;
    string userStr;
    cout << "\n";
    
    do {
        vector<int> newCoord;
        coord = newCoord;

        cout << "Please enter a valid coordinate (ex: A1): ";
        getline(cin, userStr);
        
        if (userStr.length() >= NUM_DIMENSIONS) {
            char rowChar = toupper(userStr[0]);
            int row;
            bool foundChar = false;

            for (int i = 0; i < MAX_SIZE; i++) {
                if (letters[i] == rowChar) {
                    row = i;
                    foundChar = true;
                    break;
                }
            }

            string colStr = userStr.substr(1);
            int col;
            bool colIsInt = true;

            for (int i = 0; i < colStr.length(); i++) { // Checks if the rest of the string are numbers
                if (!isdigit(colStr.at(i))) {
                    colIsInt = false;
                }
            }
            
            if (colIsInt) {
                col = stoi(colStr) - 1; // Convert for easy use with array
            }

            if (foundChar && colIsInt) {
                coord.push_back(row);
                coord.push_back(col);
                valid = IsValidCoord(offensiveBoard, coord);
            }
        }

        if (!valid) {
            cout << "Sorry, invalid coordinate. ";
        }
    } while (!valid);
    
    return coord;
}

vector<int> Player::PromptComputerCoord() {
    vector<int> coord; 

    if (settingUpShips) { // Coords procedure while setting up ships
        coord = GetRandCoord();
    } else { 
        if (isHardmode && searches.empty()) { // First check if there are hit coordinates to bounce off of.
            bool foundHitCoord = false;
            SearchData tempSd;
            for (int i = 0; i < MAX_SIZE; i++) {
                for (int j = 0; j < MAX_SIZE; j++) {
                    if (offensiveBoard[i][j] == HIT_INT) {
                        foundHitCoord = true;
                        SearchData newSd;
                        tempSd = newSd;
                        vector<int> searchCoord;
                        
                        searchCoord.push_back(i);
                        searchCoord.push_back(j);
                        tempSd.startPos = searchCoord;
                        tempSd.possibleDirs = GetPossibleDirs(searchCoord);

                        // Only use a hit that isn't surrounded by previous shots.
                        if (tempSd.possibleDirs.size() > 0) {
                            tempSd.currDir = GetRandDir(tempSd);
                            tempSd.startPos = searchCoord;
                            tempSd.currPos = GetNext(searchCoord, tempSd.currDir);
                            tempSd.futurePos = tempSd.currPos;
                            tempSd.previousPos.push_back(tempSd.currPos);
                            searches.push_back(tempSd);
                        }
                    }
                }
            }
            
            if (foundHitCoord) {
                return tempSd.currPos;
            }
        }

        if (!isHardmode || (isHardmode && searches.empty())) { // Default decision for coordinate calling
            coord = GetRandCoord();
        } else { // Procedure for hardmode and searching for a likely coordinate
            AssignShipsToSearch();
            bool finishSearch = false;
            int randInd = GetRandSearchData();
            SearchData& sd = searches[randInd];
            bool validCurrDir = true;
            
            if (sd.futurePos != sd.currPos) { // Check if the current position is going to go out of bounds
                if (IsValidCoord(offensiveBoard, sd.futurePos) || (!IsValidCoord(offensiveBoard, sd.futurePos) && IsValidCoord(offensiveBoard, sd.currPos))) {
                    validCurrDir = true;
                } else {
                    validCurrDir = false;
                }
            }

            if (!sd.currDirSuccess && sd.possibleDirs.size() > 0) {
                FinishDir(sd);

                if (sd.possibleDirs.size() == 0) {
                    finishSearch = true;
                } else {
                    finishSearch = false;
                }
            } else if (!sd.currDirSuccess && sd.possibleDirs.empty()) {
                finishSearch = true;
            } else { // Continue calling in the same direction unless it is out of bounds
                if (!validCurrDir) { // Either the current position is out of bounds or already called during salvo
                    FinishDir(sd);
                    
                    if (sd.possibleDirs.size() > 0) {
                        finishSearch = false;
                    } else {
                        finishSearch = true;
                    }
                }
            }

            if (finishSearch) {
                searches.clear();
                coord = GetRandCoord();
            } else {
                coord = sd.currPos;
                sd.currPos = GetNext(sd.currPos, sd.currDir);
                sd.futurePos = GetNext(sd.currPos, sd.currDir);
                sd.previousPos.push_back(coord);
                UpdPriorities();
            }
        }
    }

    return coord;
}

vector<int> Player::GetRandCoord() const {
    vector<int> coord; 
    int randRow = rand() % MAX_SIZE;
    int randCol = rand() % MAX_SIZE;
    coord.push_back(randRow);
    coord.push_back(randCol);
    return coord;
}

vector<string> Player::GetPossibleDirs(const vector<int>& coord) const {
    vector<string> dirs;

    if (IsValidCoord(offensiveBoard, {coord[ROW_INDEX] - 1, coord[COL_INDEX]})) {
        dirs.push_back(Ship::NORTH);
    }

    if (IsValidCoord(offensiveBoard, {coord[ROW_INDEX], coord[COL_INDEX] + 1})) {
        dirs.push_back(Ship::EAST);
    }

    if (IsValidCoord(offensiveBoard, {coord[ROW_INDEX] + 1, coord[COL_INDEX]})) {
        dirs.push_back(Ship::SOUTH);
    }

    if (IsValidCoord(offensiveBoard, {coord[ROW_INDEX], coord[COL_INDEX] - 1})) {
        dirs.push_back(Ship::WEST);
    }

    return dirs;
}

string Player::GetRandDir(const SearchData sd) const {
    return sd.possibleDirs[rand() % sd.possibleDirs.size()];
}

void Player::FinishDir(SearchData& sd) {
    if (sd.possibleDirs.size() > 0) {
        string lastDir = sd.currDir;
        bool pickRandDir = true;
        int add = 0;

        for (int i = 0; i < sd.possibleDirs.size(); i++) {
            if (sd.possibleDirs[i] == lastDir) {
                add = i;
                break;
            }
        }

        sd.possibleDirs.erase(sd.possibleDirs.begin() + add);

        // Use a direction in the same axis. Otherwise, randomize the next direction.
        if (lastDir == Ship::NORTH && IsValidCoord(offensiveBoard, {sd.startPos[ROW_INDEX] + 1, sd.startPos[COL_INDEX]})) {
            sd.currDir = Ship::SOUTH;
            pickRandDir = false;
        } else if (lastDir == Ship::SOUTH && IsValidCoord(offensiveBoard, {sd.startPos[ROW_INDEX] - 1, sd.startPos[COL_INDEX]})) {
            sd.currDir = Ship::NORTH;
            pickRandDir = false;
        } else if (lastDir == Ship::EAST && IsValidCoord(offensiveBoard, {sd.startPos[ROW_INDEX], sd.startPos[COL_INDEX] - 1})) {
            sd.currDir = Ship::WEST;
            pickRandDir = false;
        } else if (lastDir == Ship::WEST && IsValidCoord(offensiveBoard, {sd.startPos[ROW_INDEX], sd.startPos[COL_INDEX] + 1})) {
            sd.currDir = Ship::EAST;
            pickRandDir = false;
        }
    
        if (pickRandDir && sd.possibleDirs.size() > 0) {
            sd.possibleDirs = GetPossibleDirs(sd.startPos);
            sd.currDir = GetRandDir(sd);
        } 

        sd.currPos = GetNext(sd.startPos, sd.currDir);
        sd.currDirSuccess = true;
    } else {
        sd.currDirSuccess = false;
    }
}

vector<int> Player::GetNext(const vector<int>& coord, string direction) const {
    vector<int> newCoord = {coord[ROW_INDEX], coord[COL_INDEX]};

    if (direction == Ship::NORTH) {
        newCoord[ROW_INDEX]--;
    } else if (direction == Ship::EAST) {
        newCoord[COL_INDEX]++;
    } else if (direction == Ship::SOUTH) {
        newCoord[ROW_INDEX]++;
    } else {
        newCoord[COL_INDEX]--;
    }

    return newCoord;
}

int Player::GetRandSearchData() const {
    int minInd = 0;

    // Chooses the index that yields the least priority, unless the index is negative
    for (int i = 0; i < searches.size(); i++) {
        int possiblePriority = searches[i].priority;
        int minPriority = searches[minInd].priority;
        if (abs(possiblePriority) < abs(minPriority)) {
            minInd = i;
        }
    }

    return minInd;
}

int Player::GetSearchData(const vector<int>& coord) const {
    for (int i = 0; i < searches.size(); i++) {
        SearchData sd = searches[i];

        for (int j = 0; j < sd.previousPos.size(); j++) {
            if (sd.previousPos[j] == coord) {
                return i;
            }
        }
    }

    return 0;
}

void Player::AssignShipsToSearch() {
    if (!searches.empty() && !shipsHit.empty()) {
        for (SearchData& sd : searches) { // Go through every search
            bool foundShip = false;
            int spacesLeft;

            int count = 0;
            while (!foundShip && count < shipsHit.size()) { // Go through every ship and its hit coordinates to try to match search
                Ship ship = shipsHit[count];
                vector<vector<int>> shipHitCoords = ship.GetHitCoords();
                spacesLeft = ship.GetLength();

                for (int i = 0; i < shipHitCoords.size(); i++) {
                    if (shipHitCoords[i] == sd.startPos) { // Decrease the spaces left for every hit coordinate
                        foundShip = true;
                        spacesLeft--;
                    }
                }

                count++;
            }

            sd.spacesLeft = spacesLeft;
            sd.priority = sd.spacesLeft;
        }

        shipsHit.clear();
    }
}

void Player::UpdPriorities() {
    for (SearchData& sd : searches) {
        sd.priority--;
    }
}

string Player::ShootCoord(const vector<int>& coord, Player& opponent) {
    int row = coord[ROW_INDEX];
    int col = coord[COL_INDEX];
    
    // Find if a ship was hit and, if so, its name.
    vector<vector<int>> opponentBoard = opponent.GetShipBoard();
    vector<Ship*> opponentShips = opponent.GetShips();
    string status = "";

    bool opponentShipFound = false;
    int count = 0;

    if (opponentBoard[row][col] == SHIP_INT) {
        prevTurnHits.push_back(coord);
        if (searches.size() > 0) {
            int ind = GetSearchData(coord);
            searches[ind].currDirSuccess = true;
        }

        while (!opponentShipFound && count < MAX_SHIPS) { // Iterate through ship vector, checking which one was hit
            Ship* opponentShip = opponent.GetShips()[count];
            string opponentShipName = opponentShip->GetName();
            vector<vector<int>> shipCoords = opponentShip->GetCoords();
            
            for (int i = 0; i < opponentShip->GetLength(); i++) { // Iterate through ship coordinates
                vector<int> shipCoord = shipCoords[i];
                int shipRow = shipCoord[ROW_INDEX];
                int shipCol = shipCoord[COL_INDEX];

                if (row == shipRow && col == shipCol) { // Ship has been found
                    status += "hit on " + opponentShipName + "!";
                    opponent.SetShipBoard(row, col, HIT_INT);
                    offensiveBoard[row][col] = HIT_INT;
                    opponentShip->SetHitCoord(coord);
                    hitCount++;
                    opponentShipFound = true;
                    shipsHit.push_back(*opponentShip); // Save the ship data so that its HP can be assigned to a search

                    if (opponentShip->IsShipSunk()) { // Ship has sunken.
                        status += "\n" + opponentShipName + " has been sunk!";
                        status += opponent.LowerTurn(); // Lower turn for Salvo gamemode
                        opponent.IncShipsLost();
                        shipsDestroyed++;
                        searches.clear();

                        for (vector<int> hitCoord : opponentShip->GetHitCoords()) { // Set player and opponent coordinates to sunken
                            int hitCoordRow = hitCoord[ROW_INDEX];
                            int hitCoordCol = hitCoord[COL_INDEX];
                            opponent.SetShipBoard(hitCoordRow, hitCoordCol, SUNKEN_INT);
                            offensiveBoard[hitCoordRow][hitCoordCol] = SUNKEN_INT;
                        }
                    }

                    break;
                }
            }

            count++;
        }
    } else {
        status += "miss.";
        offensiveBoard[row][col] = MISS_INT;
        if (searches.size() > 0) {
            int ind = GetSearchData(coord);
            searches[ind].currDirSuccess = false;
        }
    }
    
    char rowChar;
    for (int i = 0; i < MAX_SIZE; i++) { // Convert row to char
        if (row == i) {
            rowChar = letters[i];
        }
    }
    
    callCount++;
    string result = "";
    result += rowChar + to_string(col + 1) + " was a " + status + "\n"; // Increase col by one (index is 1 less)
    return result;
}

string Player::PromptUserOrientation() const {
    string userOrtn;
    bool foundOrientation = false;

    do {
        cout << "Please enter a valid ship orientation (ex: NORTH): ";
        getline(cin, userOrtn);

        for (int i = 0; i < userOrtn.length(); i++) { // toupper() string
            userOrtn[i] = toupper(userOrtn[i]);
        }

        for (int i = 0; i < Ship::ORIENTATIONS.size(); i++) {
            string orientation = Ship::ORIENTATIONS[i];
            if (userOrtn == orientation) {
                foundOrientation = true;
                break;
            }
        }

        if (!foundOrientation) {
            cout << "Sorry, invalid orientation. ";
        }
    } while (!foundOrientation);

    return userOrtn;
}

int Player::GetShipsDestroyed() const {
    return shipsDestroyed;
}

int Player::GetHitCount() const {
    return hitCount;
}

int Player::GetCallCount() const {
    return callCount;
}

double Player::GetAccuracy() const {
    return (callCount > 0) ? (static_cast<double>(hitCount) / callCount) * 100.0 : 0.0;
}