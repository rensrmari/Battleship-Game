// File: marianoClarence_FinalProject.cpp
// Name: Clarence Mariano
// Date: 10/28/2024
// Course: CompSci 1
// Description: This program will let the user play games of battleship.
// Usage: This program will ask the user to choose whether they would like to go against another player or the
//        computer, ask them to set up their ships, and ask them to shoot at coordinates.

#include "Player.h"
#include "Ship.h"
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <limits>
#include <ctime>
#include <cctype>
#include <cstdlib>
#include <sstream>
#include <fstream>
using namespace std;

void DisplayMainMenu();
void DisplayTutorial();
void DisplayOpponentsMenu(bool& isComputer, bool& isHardmode);
void DisplayOpponent(bool isComputer, bool isHardmode);
void DisplayModesMenu(bool& isClassic);
void DisplayMode(bool isClassic);
void PlayGame(bool isComputer, bool isClassic, bool isHardmode);

void CreateIndicator(string msg, vector<string> labels, int index);
char ReadUserLetter();

int main(){
    const char PLAY_CHAR = 'P';
    const char HOW_CHAR = 'H';
    const char MODES_CHAR = 'M';
    const char OPPONENT_CHAR = 'O';
    const char QUIT_CHAR = 'Q';

    // Constantly prompts user to input a letter, checking if they quit the game or start the game from the menu.
    bool isComputer = true;
    bool isClassic = true;
    bool isHardmode = false;
    char menuInput;

    do {
        DisplayMainMenu();
        menuInput = toupper(ReadUserLetter()); // Prompt user for letter in the main menu

        if (menuInput == HOW_CHAR) { // User enters rules menu
            DisplayTutorial();
        }
        
        if (menuInput == OPPONENT_CHAR) { // User enters opponent select
            DisplayOpponentsMenu(isComputer, isHardmode);
        } 
        
        if (menuInput == MODES_CHAR) { // User enters modes menu
            DisplayModesMenu(isClassic);
        }
    
    } while(!( (menuInput == QUIT_CHAR) || (menuInput == PLAY_CHAR) )); // Loop ends if user quits or plays
    
    // Begins game if user enters play.
    if (menuInput == PLAY_CHAR) {
        PlayGame(isComputer, isClassic, isHardmode);
    }

    // Thanks user for playing the game in a battleship-like way.
    cout << "\nFair winds and following seas.\n";

    return 0;
}

/** 
 * Reads the letter a user inputs.
 * @return The letter that the user inputs in lowercase.
 */
char ReadUserLetter() {
    const long long MAX_INPUT = numeric_limits<streamsize>::max();

    char userLetter;
    cin >> userLetter;
    cin.ignore(MAX_INPUT, '\n');

    return userLetter;
}

/**
 * Displays the main menu and its buttons, prompting the user to enter a letter.
 */
void DisplayMainMenu() {
    const int MENU_WIDTH = 17;

    cout << "\n\n<<<<<<<<<<<<<<<<< BATTLESHIP >>>>>>>>>>>>>>>>>>" << "\n" << left
         << setw(MENU_WIDTH) << "" << "> PLAY (P)" << "\n"
         << setw(MENU_WIDTH) << "" << "> HOW TO PLAY (H)" << "\n"
         << setw(MENU_WIDTH) << "" << "> MODES (M)" << "\n"
         << setw(MENU_WIDTH) << "" << "> OPPONENT SELECT (O)\n"
         << setw(MENU_WIDTH) << "" << "> QUIT (Q)" << "\n"
         << "Enter a letter to proceed: ";

    cout << right;
}

/**
 * Displays a how-to for the game.
 */
void DisplayTutorial() {
    const int SPACING = 5;
    string spacing(SPACING, ' ');

    cout << "\nHow To Play (Classic):\n"
         << spacing << "1. Once you press \"P\", players pick where and how to place down their ships (they cannot overlap).\n"
         << spacing << spacing << "a. You will be prompted to enter the coordinates and orientations of 5 ships with varying hit points.\n"
         << spacing << spacing << spacing << "i. Destroyer: 2 HP\n"
         << spacing << spacing << spacing << "ii. Submarine: 3 HP\n"
         << spacing << spacing << spacing << "iii. Cruiser: 3 HP\n"
         << spacing << spacing << spacing << "iv. Battleship: 4 HP\n"
         << spacing << spacing << spacing << "v. Carrier: 5 HP\n"
         << spacing << spacing << "b. Your ships will be represented as \"O\".\n"
         << spacing << "2. Players take turns calling a coordinate (A-J, 1-10) to try to hit their opponent's ships.\n"
         << spacing << spacing << "a. Misses will be recorded as \"-\".\n"
         << spacing << spacing << "b. Hits will be recorded as \"x\".\n"
         << spacing << spacing << "c. Sunken ships will be recorded as \"~\".\n"
         << spacing << "3. If an opponent's ship is hit, the player whose turn it is will be notified of it.\n"
         << spacing << "4. Once all of the spaces a ship has are hit, it will sink, which will also be announced.\n"
         << spacing << "5. The player who has sunken all 5 ships wins.\n\n";

    cout << "How To Play (Salvo):\n"
         << spacing << "1. This gamemode is similar to Classic, except:\n"
         << spacing << spacing << "a. Players begin with 5 attempts to call coordinates.\n"
         << spacing << spacing << "b. A loss of a ship results in one less coordinate call.\n\n";

    cout << "Enter anything to go back: ";

    ReadUserLetter();
}

/**
 * Displays dialogue with user's option indicated.
 * @param msg Opening message.
 * @param labels The labels the display will show.
 * @param index The index whose label should be indicated.
 */
void CreateIndicator(string msg, vector<string> labels, int index) {
    const char INDICATOR_L = '<';
    const char INDICATOR_R = '>';
    const int OFFSET = 5;
    const int SPACING = 3;

    string spacing(SPACING, ' ');
    
    cout << "\n\n" << msg << ":\n" << left;

    for (int i = 0; i < labels.size(); i++) {
        string label = labels[i];
        cout << setw(OFFSET) << "";

        if (i == index) {
            cout << INDICATOR_L << spacing << label << spacing << INDICATOR_R;
        } else {
            cout << label;
        }

        cout << "\n";
    }
}

/**
 * Displays the opponents menu.
 * @param isComputer Whether or not the opponent is a computer.
 */
void DisplayOpponentsMenu(bool& isComputer, bool& isHardmode) {
    const char EASY_COMPUTER_CHAR = 'E';
    const char HARD_COMPUTER_CHAR = 'H';
    const char OTHER_USER_CHAR = 'O';

    DisplayOpponent(isComputer, isHardmode);
    cout << "\nEnter letter to choose an opponent: ";
    
    // Reads user input to determine the opponent.
    char menuInput = toupper(ReadUserLetter());

    if (menuInput == OTHER_USER_CHAR) {
        isComputer = false;
        isHardmode = false;
        DisplayOpponent(isComputer, isHardmode);
        cout << "\nYou will be playing against another user.";
    } else if (menuInput == EASY_COMPUTER_CHAR) {
        isComputer = true;
        isHardmode = false;
        DisplayOpponent(isComputer, isHardmode);
        cout << "\nYou will be playing against the easy computer.";
    } else if (menuInput == HARD_COMPUTER_CHAR) {
        isComputer = true;
        isHardmode = true;
        DisplayOpponent(isComputer, isHardmode);
        cout << "\nYou will be playing against the hard computer.";
    } else {
        DisplayOpponent(isComputer, isHardmode);
        cout << "\nYour opponent selection has not been changed.";
    }

    cout << "\nEnter anything to go back: ";

    ReadUserLetter();
}

/**
 * Indicates the opponent the user would like to go against.
 * @param isComputer Whether or not the opponent is a computer.
 * @param isHardmode Whether or not the computer player is going to be hard to fight against.
 */
void DisplayOpponent(bool isComputer, bool isHardmode) {
    string prompt = "Choose an opponent";
    vector<string> labels = { "EASY COMPUTER (E)", "HARD COMPUTER (H)", "OTHER USER (O)" };

    if (isComputer && !isHardmode) {
        CreateIndicator(prompt, labels, 0);
    } else if (isComputer && isHardmode) {
        CreateIndicator(prompt, labels, 1);
    } else {
        CreateIndicator(prompt, labels, 2);
    }
}

/**
 * Displays the modes of the game.
 * @param isClassic Whether or not the mode is classic.
 */
void DisplayModesMenu(bool& isClassic) {
    const char CLASSIC_CHAR = 'C';
    const char SALVO_CHAR = 'S';

    DisplayMode(isClassic);
    cout << "\nEnter letter to choose a mode: ";

    char menuInput = toupper(ReadUserLetter());
    
    if (menuInput == SALVO_CHAR) {
        isClassic = false;
        DisplayMode(isClassic);
        cout << "\nWant a challenge? Salvo it is.";
    } else if (menuInput == CLASSIC_CHAR) {
        isClassic = true;
        DisplayMode(isClassic);
        cout << "\nCan't go wrong with a classic game of battleship.";
    } else {
        DisplayMode(isClassic);
        cout << "\nThe mode has not been changed.";
    }

    cout << "\nEnter anything to go back: ";

    ReadUserLetter();
}

/**
 * Indicates the current mode.
 * @param isClassic Whether or not the mode is classic.
 */
void DisplayMode(bool isClassic) {
    string prompt = "Choose mode";
    vector<string> labels = { "CLASSIC (C)", "SALVO (S)" };

    if (isClassic) {
        CreateIndicator(prompt, labels, 0);
    } else {
        CreateIndicator(prompt, labels, 1);
    }
}

/**
 * Starts the game.
 * @param isComputer Whether or not the other player is a computer.
 * @param isClassic Whether or not the mode is classic.
 * @param isHardmode Whether or not the computer will be hard to play against.
 */
void PlayGame(bool isComputer, bool isClassic, bool isHardmode) {
    const string USER_NAME = "USER";
    const string OPP_USER_NAME = "SECOND USER";
    const string EASY_COMPUTER_NAME = "EASY COMPUTER";
    const string HARD_COMPUTER_NAME = "HARD COMPUTER";
    const int NUM_PLAYERS = 2;

    // Create player names.
    string userName;
    string opponentName = isHardmode ? HARD_COMPUTER_NAME : EASY_COMPUTER_NAME;
    cout << "\nGreetings, captains. What are your names?\n";
    cout << "Player 1: ";
    getline(cin, userName);
    cout << "Player 2: ";

    if (!isComputer) {
        getline(cin, opponentName);
    } else {
        cout << opponentName;
    }

    // Deciding player to start.
    Player user(userName, false, isClassic, false);
    Player opponent(opponentName, isComputer, isClassic, isHardmode);
    vector<Player*> players;

    srand(time(0));
    int randNum = rand() % NUM_PLAYERS; // 0 = User starts, 1 = Opponent starts
    string startingPlayer = (randNum == 0) ? user.GetName() : opponent.GetName();

    cout << "\n\nGreetings, Captain " << user.GetName() << " and Captain " << opponent.GetName() << ".\n";
    cout << "The first player will be...\n" << startingPlayer << ".\n";
    
    // Order players array considering startingPlayer (first is first, second is second).
    Player* userPtr = &user;
    Player* opponentPtr = &opponent;
    if (startingPlayer == user.GetName()) {
        players.push_back(userPtr);
        players.push_back(opponentPtr);
    } else {
        players.push_back(opponentPtr);
        players.push_back(userPtr);
    }
    
    // Place down ships on board.
    for (int i = 0; i < NUM_PLAYERS; i++) {
        cout << "\nCaptain " << players[i]->GetName() << ", please place down your ships.\n";
        players[i]->GenerateShips();
    }

    cout << "\nThank you for taking charge of your fleet.";

    // Loop turns until win.
    const int MAX_SHIPS = Player::MAX_SHIPS;
    
    cout << "\nNow, Captain " << startingPlayer << " goes first.\n";

    while (user.GetShipsDestroyed() != MAX_SHIPS && opponent.GetShipsDestroyed() != MAX_SHIPS) {
        for (int i = 0; i < NUM_PLAYERS; i++) { 
            Player* player = players[i];
            Player* otherPlayer = (i == 0) ? players[1] : players[0]; // pointer to opposing player
            player->Turn(*otherPlayer);
        }
    }

    user.DeleteShips();
    opponent.DeleteShips();

    // Game ends, output loss message.
    cout << "\nCaptain ";
    string loserName = "";

    if (user.GetShipsDestroyed() != MAX_SHIPS) {
        loserName = userName;
    } else {
        loserName = opponentName;
    }

    cout << loserName << " has been defeated!\n\n";

    // Output stats (ships destroyed and accuracy).
    const string OUTPUT_FILE = "outputfile.txt";
    const int USER_SPACING = 25;
    const int OPPONENT_SPACING = 20;
    const int ACCURACY_PRECISION = 2;
    const char DIVIDER = '-';

    ofstream outputFile(OUTPUT_FILE);
    
    if (!outputFile.is_open()) {
        cout << "\nError opening file.\n";
        return;
    }

    // Output game stats.
    ostringstream playerRow; // Player names
    string userResult = "";
    string opponentResult = "";

    if (loserName == user.GetName()) {
        userResult = userName + " (lost)";
        opponentResult = opponentName + " (won)";
    } else {
        userResult = userName + " (won)";
        opponentResult = opponentName + " (lost)";
    }

    playerRow << left << setw(USER_SPACING) << "Player" << setw(OPPONENT_SPACING) << userResult << opponentResult << "\n";
    string playerRowStr = playerRow.str();

    ostringstream tableHeader; // Header of table
    tableHeader << left;
    tableHeader << "Game Stats:\n";
    string divider(playerRowStr.length() - 1, DIVIDER); // Account for newline (increases string length)
    tableHeader << divider << "\n";
    string tableHeaderStr = tableHeader.str();

    ostringstream shipsDestroyedRow; // Ships destroyed
    shipsDestroyedRow << left;
    shipsDestroyedRow << setw(USER_SPACING) << "Ships Destroyed" << setw(OPPONENT_SPACING) << user.GetShipsDestroyed() << opponent.GetShipsDestroyed() << "\n";
    string shipsDestroyedRowStr = shipsDestroyedRow.str();

    ostringstream hits;
    hits << left;
    hits << setw(USER_SPACING) << "Hits" << setw(OPPONENT_SPACING) << user.GetHitCount() << opponent.GetHitCount() << "\n";
    string hitsStr = hits.str();

    ostringstream calls;
    calls << left;
    calls << setw(USER_SPACING) << "Calls" << setw(OPPONENT_SPACING) << user.GetCallCount() << opponent.GetCallCount() << "\n";
    string callsStr = calls.str();

    ostringstream userAccuracy; // Appends percentage and keeps precision of accuracies
    userAccuracy << fixed << setprecision(ACCURACY_PRECISION) << user.GetAccuracy() << "%";
    ostringstream opponentAccuracy;
    opponentAccuracy << fixed << setprecision(ACCURACY_PRECISION) << opponent.GetAccuracy() << "%";
    ostringstream accuracies;
    accuracies << left << fixed << setprecision(ACCURACY_PRECISION);
    accuracies << setw(USER_SPACING) << "Accuracy" << setw(OPPONENT_SPACING) << userAccuracy.str() << opponentAccuracy.str() << "\n";
    string accuraciesStr = accuracies.str();

    // Outpt everything to terminal.
    cout << tableHeaderStr << playerRowStr << shipsDestroyedRowStr << hitsStr << callsStr << accuraciesStr;

    // Output to file.
    outputFile << tableHeaderStr << playerRowStr << shipsDestroyedRowStr << hitsStr << callsStr << accuraciesStr;
}