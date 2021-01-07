// main.cpp : This file contains the 'main' function. Program execution begins and ends there.
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

/**
 * Cross Platform ANSI Escape Codes
 * @copyright: https://gist.github.com/gmes78/75de1d0653fe3f7070518aa8b31e0a53
 */
#ifdef _WIN32
#define MY_UNICODE
#include <windows.h>
#include <wincon.h>

#ifndef DISABLE_NEWLINE_AUTO_RETURN
#define DISABLE_NEWLINE_AUTO_RETURN 0x0008
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_INPUT
#define ENABLE_VIRTUAL_TERMINAL_INPUT 0x0200
#endif

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

bool setUpConsole() {
	// Change console's dimensions
	//HWND console = GetConsoleWindow();
	//RECT r;
	//GetWindowRect(console, &r); // Stores the console's current dimensions
	//MoveWindow(console, r.left, r.top, 1200, 600, TRUE); // 1200 width, 600 height

	HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hIn == INVALID_HANDLE_VALUE || hOut == INVALID_HANDLE_VALUE) {
		return (false);
	}
	DWORD dwOriginalInMode = 0;
	DWORD dwOriginalOutMode = 0;
	if (!GetConsoleMode(hIn, &dwOriginalInMode)) {
		return (false);
	}
	if (!GetConsoleMode(hOut, &dwOriginalOutMode)) {
		return (false);
	}
	DWORD dwInMode = dwOriginalInMode | ENABLE_VIRTUAL_TERMINAL_INPUT;
	if (!SetConsoleMode(hIn, dwInMode)) {
		return (false);
	}
	DWORD dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(hOut, dwOutMode)) {
		dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwOutMode)) {
			return (false);
		}
	}
	return (true);
}

#endif //_WIN32

//==================================================

#include <cstdlib>

#include <iostream>
#include <iomanip> 
#include <string>
#include <vector>
#include <fstream> 
#include <conio.h> 
#include <stdio.h>
#define __STDC_WANT_LIB_EXT1__ 1
#include <time.h>
// #include <ctime>
//#include <chrono>
#include <algorithm>

// CLASSES 
#include "Auxiliar.h"
#include "Player.h"
#include "Board.h"
#include "Pool.h"

// DEFINITION OF GLOBAL VARIABLES
// Position of reference of the beginning of the board on the screen
short const BOARD_START_LINE = 4;
short const BOARD_START_COLUMN = 3;
// Number of lines deviated from 'BOARD_START_LINE'  
short const LINES_AWAY_FROM_BOARD = 1;
// Number of columns deviated from the end of the board
short const COLS_AWAY_FROM_BOARD = 5;
// Standardize the value of the atribute to use on the function sleep()
unsigned int const TIME_TO_WAIT = 1700;

//==================================================

/**
 * Display a specific message on the screen
 * @param warning - message to be written
 * @param color - text color to write the message
 */
void writeWarning(const std::string &warning, const std::string &color = WARNING) {
	writeColoredText(color, warning);
} // writeWarning

//==================================================

/**
 * Display in the correct position the title of the game
 */
void formatingGameTitleOuput() {
	std::string title = "Scrabble Junior Game";

	setCursorPosition(6, 1);
	std::cout << repeatString("-", title.size());
	setCursorPosition(6, 2);
	std::cout << title;
	setCursorPosition(1, 4);
} // formatingGameTitleOuput

//==================================================

/**
 * Clear the right side next to the board
 * @param boardWidth - number of columns of the board
 */
void clearRightSideOfScreen(const unsigned int boardWidth) {
	short startColumn = BOARD_START_COLUMN + (short)boardWidth * 3 + COLS_AWAY_FROM_BOARD;

	for (short l = 0; l < 20; l++) {
		clearLine(startColumn, BOARD_START_LINE + l);
	}
	setCursorPosition(startColumn, BOARD_START_LINE + LINES_AWAY_FROM_BOARD);
} // clearSideOfScreen

//==================================================

/**
 * Check if the user is ready to start playing
 * @param startColumn - reference column where to write the text
 * @param stratLine - reference line where to write the text
 * return exit the program if user press "ESC", TRUE otherwise
 */
void startTheGame(const short startColumn, const short stratLine) {
	setCursorPosition(startColumn, stratLine);
	std::cout << "<Press any key to start playing>";
	setCursorPosition(startColumn, stratLine + 1);
	std::cout << "       (OR ESC to leave) ";

	char input = (char)_getch();
	if (27 == (int)input) {
		setCursorPosition(0, BOARD_START_LINE + 22);
		exit(1);
	}
	
} // startTheGame

//==================================================

/**
 * Ask user if he would like to play again and validate his answer
 * @return FALSE when the user enters 'N' or 'n'; TRUE when he enters 'Y' or 'y'
 */
bool isToPlayAgain() {
	std::string answer;
	bool validInput;
	bool playAgain = false;

	do {
		validInput = true;
		clearLine(0, 10);
		setCursorPosition(1, 10);
		std::cout << " Would you like to try again (Y|N)? ";
		if (!isInputCorrect(answer) || 1 != answer.length()) {
			validInput = false;
		}

		if (validInput) {
			if ((char)toupper(answer.at(0)) == 'Y') {
				playAgain = true;
			}
			else if ((char)toupper(answer.at(0)) == 'N') {
				playAgain = false;
			}
			else {
				validInput = false;
			}
		}

		if (!validInput) {
			setCursorPosition(1, 11);
			writeWarning(" Write a valid answer!");
		}
	} while (!validInput);

	clearLine(0, 11);
	return (playAgain);
} // isToPlayAgain

//==================================================

/**
 * Save the scores of the players of the game in a file
 * @param players - all players on game
 */
void saveGameResultsOnFile(const std::vector<Player> &players) {
	std::string fileName = "GamesResults.txt";
	std::ofstream file;

	// If the file doesn't exit, it is created
	if (!canFileBeOpen(fileName)) {
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::trunc);
	}
	else {
		file.open(fileName, std::fstream::in | std::fstream::out | std::fstream::app);
	}

	// Get actual date and hour
	// Number of seconds since 1 of january of 1970
	time_t t = time(NULL);
	struct tm tm;

	// Convert this seconds to the local calendary according to the Operative System
	#ifdef MY_UNICODE  
		localtime_s(&tm, &t);
	#else
		tm = *localtime(&t);
	#endif

	// [FORMAT: Y-m-d H:m:s]
	char now[20];
	strftime(now, sizeof(now), "%Y-%m-%d %X", &tm);

	// Write on file [FORMAT: (date) -> name (n chips) | ... ]
	file << "(" << now << ") -> ";
	for (size_t p = 0; p < players.size(); p++) {
		Player player = players.at(p);
		file << player.getName() << " (" << player.getChips() << " chips)";
		if (p != players.size() - 1) {
			file << " | ";
		}
	}
	file << "\n";

	file.close();
} // saveGameResultsOnFile

//==================================================

/**
 * Announce the winner of the game
 * @param players - all players on game
 */
void annouceWinner(const std::vector<Player> &players, const bool isGameImpossible = false) {
	clearScreen();
	formatingGameTitleOuput();
	if (!isGameImpossible) {
		std::cout << " It's the end of the game!";
	}
	else {
		std::cout << " The game has no ending! There are no other possible moves!";
	}

	saveGameResultsOnFile(players);

	std::vector<size_t> winners;
	short greaterNumberChips = 0;

	// Get player(s) with the highest number of chips
	for (size_t p = 0; p < players.size(); p++) {
		Player player = players.at(p);
		if (player.getChips() > greaterNumberChips) {
			greaterNumberChips = player.getChips();
			winners.resize(0);
			winners.push_back(p);
		}
		else if (player.getChips() == greaterNumberChips) {
			winners.push_back(p);
		}
	}

	// Formatting output message
	if (winners.size() == 1) {
		std::cout << "\n The winner of the game is ";
	}
	else {
		std::cout << "\n The winners of the game are ";
	}

	for (size_t i = 0; i < winners.size(); i++) {
		Player player = players.at(winners.at(i));
		// Display on screen player's name in uppercase
		std::string str = player.getName();
		std::transform(str.begin(), str.end(), str.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });
		writeColoredText(player.getColor(), str);
		if (i != winners.size() - 1) {
			std::cout << "; ";
		}
	}

	std::cout << " with a total of " << greaterNumberChips << " chips." << std::endl;
	std::cout << "\n Thank you for playing!" << std::endl;
} // annouceWinner

//==================================================

/**
 * Detect if the game ended
 * @param bag - all letters needed to construct the board
 * @param players - all players on game
 * @return TRUE if the game ended; FALSE otherwise
 */
bool isEndOfGame(const Pool &bag, const std::vector<Player> &players) {
	// The game ends when the pool his empty and all tiles are in place (player's hand empty)
	if (0 == bag.getSize()) {
		for (const auto & player : players) {
			if (0 != player.getHandSize()) {
				return (false);
			}
		}

		annouceWinner(players);
		return (true);
	}

	return (false);
} // isEndOfGame

//==================================================

/**
 * Dected if the game can be finished
 * @param consetivePass - number of consecutive pass
 * @param players - all players on game
 * @return TRUE if the game as no end; FALSE otherwise
 */
bool isImpossibleGame(const unsigned int consetivePass, const std::vector<Player> &players) {
	// If the number of times that players are force to pass equals the amount of them
	if (consetivePass >= players.size()) {
		annouceWinner(players, true);
		return (true);
	}

	return (false);
} // isImpossibleGame

//==================================================

/**
 * Display the main information of the player
 * @param boardWidth - number of columns of the board
 * @param player - current player
 * @param playerIdx - index of the player
 */
void writePlayerHeader(const unsigned int boardWidth, const Player &player, const unsigned int playerIdx) {
	short startSideCol = BOARD_START_COLUMN + (short)boardWidth * 3 + COLS_AWAY_FROM_BOARD;
	setCursorPosition(startSideCol, BOARD_START_LINE + LINES_AWAY_FROM_BOARD);
	std::cout << "Turn of:  PLAYER " << playerIdx + 1 << " -> ";
	writeColoredText(player.getColor(), player.getName());

	player.showHand(startSideCol, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 1);

	setCursorPosition(startSideCol, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 5);
} // writePlayerHeader

//==================================================

/**
 * Ask player the letter he wants to play and validate his answer
 * @param player - current player
 * @param question - the question we want to ask
 * @param board - board of the game
 * @param linesSpace - number of lines between player's header and the line we want to write it
 * param toReplace - indicates whether the letter is to be replaced or played
 * @param fCoords - coordinates where player cannot play
 * @return the letter chosen
 */
char askPlayerValidLetter(const Player &player, const std::string &question, const Board &board, const short linesSpace, const bool toReplace, const std::string &fCoords = "") {
	std::string letterStr;
	char letter = ' ';
	bool validInput;

	short startQuestionLine = BOARD_START_LINE + LINES_AWAY_FROM_BOARD + linesSpace;
	short startQuestionCol = BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD;
	setCursorPosition(startQuestionCol, startQuestionLine);

	// Validate answer
	do {
		validInput = true;

		clearLine(startQuestionCol, startQuestionLine);
		setCursorPosition(startQuestionCol, startQuestionLine);
		std::cout << question;
		// Clear Error Line
		clearLine(startQuestionCol, startQuestionLine + 1);

		setTextColor(player.getColor());

		validInput = isInputCorrect(letterStr) && (1 == letterStr.size());

		setTextColor(NO_COLOR);
		setCursorPosition(startQuestionCol, startQuestionLine + 1);

		// Gave Invalid Awnser
		if (!validInput) {
			writeWarning("Input must be a caracter!");
		}
		else {
			letter = (char)toupper((int)letterStr.at(0));

			// Conditions required
			validInput = false;
			if (!player.isLetterOnHand(letter)) {
				writeWarning("The letter must be in your hand!");
			}
			else if (!toReplace) {
				if (!board.isFirstLetterOfWord(letter)) {
					writeWarning("You can't do any move with this letter!");
				}
				else {
					bool flag = false;

					if (!fCoords.empty()) {
						flag = board.isLetterAllValidCoordsForbidden(letter, fCoords);
					}

					if (flag) {
						writeWarning("You can't do any move with this letter!");
					}
					else {
						validInput = true;
					}
				}
			}
			else {
				validInput = true;
			}
		}

		if (!validInput) {
			Sleep(TIME_TO_WAIT);
		}
	} while (!validInput);

	return (letter);
} // askPlayerValidLetter

//==================================================

/**
 * Ask player in that coordinates we want to play and validate his answer
 * @param board - board of the game
 * @param letterToMove - letter we want to play
 * @param forbiddenCoordinates - coordinates where player cannot play
 * @param playerColor - color of the current player
 * @return the coordinates chosen
 */
std::string askPlayerValidCoordinate(const Board &board, const char letterToMove, const std::string &forbiddenCoordinates, const std::string &playerColor) {
	std::string coordsToPlay;
	bool validInput;

	short startQuestionLine = BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 7;
	short startQuestionCol = BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD;
	setCursorPosition(startQuestionCol, startQuestionLine);

	// Validate answer
	do {
		validInput = true;

		clearLine(startQuestionCol, startQuestionLine);
		setCursorPosition(startQuestionCol, startQuestionLine);
		std::cout << "In what Coordinates [Eg.: Ca]: ";
		// Clear Error Line
		clearLine(startQuestionCol, startQuestionLine + 1);

		setTextColor(playerColor);

		validInput = isInputCorrect(coordsToPlay) && (2 == coordsToPlay.size());

		setTextColor(NO_COLOR);
		setCursorPosition(startQuestionCol, startQuestionLine + 1);

		// Gave Invalid Awnser
		if (!validInput) {
			writeWarning("Invalid Input!");
		}
		else {
			coordsToPlay = std::string() + (char)(toupper(coordsToPlay[0])) + (char)(tolower(coordsToPlay[1]));

			// Conditions required
			validInput = false;
			if (!board.isCoordinatesInBoardRange(coordsToPlay)) {
				writeWarning("This coordinate is not part of the board!");
			}
			else if (!board.coordinatesMatchLetter(coordsToPlay, letterToMove)) {
				writeWarning("The letter on the board doesn't match!");
			}
			else if (board.isLetterAlreadyPlayed(coordsToPlay)) {
				writeWarning("That tile was already played!");
			}
			else if (std::string::npos != forbiddenCoordinates.find(coordsToPlay)) {
				writeWarning("You can't play the next letter of the same word!");
			}
			else if (!board.isFirstLetterOfWord(coordsToPlay)) {
				writeWarning("Is not the first or the next letter of a word!");
			}
			else {
				validInput = true;
			}
		}

		if (!validInput) {
			Sleep(TIME_TO_WAIT);
		}
	} while (!validInput);

	return (coordsToPlay);
} // askPlayerValidCoordinate

//==================================================

/**
 * Ask player to replace at most 2 of his tiles, if possible
 * @param player - current player
 * @param bag - all letters needed to construct the board
 * @param board - board of the game
 */
void changeTilesWithPool(Player &player, Pool &bag, const Board &board) {
	if (0 == player.getHandSize()) {
		writeWarning("You have no letters!");
	}
	else {
		unsigned int nTilesToRelplace = 1;

		// Pool has more than 1 tile and Player's hand has at least two tiles
		if (1 != bag.getSize() && 2 <= player.getHandSize()) {
			nTilesToRelplace = 2;
		}
		writeWarning("You can't play any tile: replace " + std::to_string(nTilesToRelplace) + " with the Pool!");

		short startSideCol = BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD;

		std::string lettersAdd = "";
		unsigned short tileNumber = 0;
		do {
			// Validate the letter - must be in player's hand
			char letterToReplace = askPlayerValidLetter(player, std::to_string(tileNumber + 1) + " letter you wish to replace: ", board, 7 + (short)tileNumber, true);

			// Add player new letter and put the one to replace in the pool
			player.removeLetterOfHand(letterToReplace);
			char letterToAdd = bag.getLetter(bag.randomLetterIdx());

			if (0 == tileNumber) {
				// Show the hand without the first tile to replace
				player.showHand(startSideCol, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 1);
			}

			player.addLetterOnHand(letterToAdd);
			lettersAdd += std::string() + "'" + (char)(letterToAdd)+"' ";
			bag.addLetter(letterToReplace);

			tileNumber++;
		} while (tileNumber < nTilesToRelplace);

		setCursorPosition(startSideCol, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 10);
		writeColoredText(player.getColor(), "You received " + std::to_string(nTilesToRelplace) + " from the pool: " + lettersAdd);
	}
} // changeTilesWithPool

//==================================================

/**
 * Ask a player what move we want to do
 * @param board - board of the game
 * @param player - current player
 * @param forbiddenCoordinates - coordinates where player cannot play
 */
void askPlayerMove(Board &board, Player &player, std::string &forbiddenCoordinates) {

	// Ask and validate letter
	char letterToMove = askPlayerValidLetter(player, "Letter you want to play: ", board, 6, false, forbiddenCoordinates);
	// Ask and validate coordinates
	std::string coordsToPlay = askPlayerValidCoordinate(board, letterToMove, forbiddenCoordinates, player.getColor());

	// Update the displayed board with the chosen move in the color of the player
	std::pair<int, int> intCoords = board.getIntCoordinates(coordsToPlay); // pair <line, column>
	setCursorPosition((short)(BOARD_START_COLUMN + intCoords.second * 3), (short)(BOARD_START_LINE + intCoords.first));
	writeColoredText(player.getBackgroundColor(), std::string() + (char)letterToMove);
	// Change the state of the tile at already played
	board.changeCoordinatesPlayability(coordsToPlay, true);

	// Remove the letter played from the hand of the player
	player.removeLetterOfHand(letterToMove);
	// pair<numberWordsFineshed, newForbiddenCoordinates>
	std::pair<int, std::string> newInfo = board.updateWords(coordsToPlay);
	player.addChips((short)newInfo.first);
	forbiddenCoordinates = newInfo.second;

	// Display the number of words finished with this move
	setCursorPosition(BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 8);
	writeColoredText(player.getColor(), "You completed " + std::to_string(newInfo.first) + " words [atual chips = " + std::to_string(player.getChips()) + "]");
} // askPlayerMove

//==================================================

/**
 * Check if the player was any valid move to do
 * @param board - board of the game
 * @param player - current player
 * @param forbiddenCoordinates - coordinates where player cannot play
 * @param isFirstPlayOfGame - indicates if is the fisrt turn of alls
 * @return TRUE if player can to a move; FALSE otherwise
 */
bool playerCanPlay(const Board &board, const Player &player, std::string &forbiddenCoordinates, const bool isFirstPlayOfGame) {
	if (isFirstPlayOfGame) {
		forbiddenCoordinates = "";
	}

	setCursorPosition(BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 6);

	for (size_t l = 0; l < player.getHandSize(); l++) {
		char letter = player.getLetterOnHand(l);

		// Conditions required to a move can be a valid one
		// Returns true as soon as it finds a letter that is possible to play
		if (board.isFirstLetterOfWord(letter)) {
			if (!forbiddenCoordinates.empty()) {
				if (!board.isLetterAllValidCoordsForbidden(letter, forbiddenCoordinates)) {
					return (true);
				}
			}
			else {
				return (true);
			}
		}
	}

	return (false);
} // playerCanPlay

//==================================================

/**
 * Display on the screen the board on his starting position
 * @param board - board of the game
 */
void displayBoardStartingPosition(const Board &board) {
	clearScreen();
	formatingGameTitleOuput();
	board.drawBoard(BOARD_START_LINE, BOARD_START_COLUMN);
	setCursorPosition(BOARD_START_COLUMN + (short)board.getColumns() * 3 + COLS_AWAY_FROM_BOARD, BOARD_START_LINE + LINES_AWAY_FROM_BOARD);
} // displayBoardStartingPosition

//==================================================

/**
 * Display on the screen the hand of all players and waits till they are ready to start
 * @param players - all players on game
 * @param board - board of the game
 * @param firstPlayerIdx - index of the first player to start the game
 */
void showPlayersHandBeforeGame(const std::vector<Player> &players, const Board &board, const size_t firstPlayerIdx) {
	clearScreen();
	displayBoardStartingPosition(board);

	short startColumn = BOARD_START_COLUMN + ((short)board.getColumns() * 3) + COLS_AWAY_FROM_BOARD;

	// Display the hand of all players on the right coordinates
	for (size_t p = 0; p < players.size(); p++) {
		setCursorPosition(startColumn, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + (short)p * 4);
		writeColoredText(players.at(p).getColor(), "PLAYER " + std::to_string(p + 1) + " - " + players.at(p).getName());
		if (p == firstPlayerIdx) {
			std::cout << " <- FIRST PLAYER";
		}
		players.at(p).showHand(startColumn, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + ((short)p * 4) + 1);
	}

	startTheGame(startColumn, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + (short)players.size() * 4 + 1);
} // showPlayersHandBeforeGame

//==================================================

/**
 * Add to the player hand the number of tiles that he played
 * @param player - current player
 * @param bag - all letters needed to construct the board
 * @param playerTotalShifts - total number of player shifts
 * @param boardWidth - number of columns of the board
 */
void updatePlayerHand(Player &player, Pool &bag, const unsigned int playerTotalShifts, const unsigned int boardWidth) {
	setCursorPosition(BOARD_START_COLUMN + (short)boardWidth * 3 + COLS_AWAY_FROM_BOARD, BOARD_START_LINE + LINES_AWAY_FROM_BOARD + 10);

	if (0 != bag.getSize()) {
		if (1 != playerTotalShifts) {
			std::string lettersAdd = "";
			size_t nShift = 1;
			// Add at most the number of tiles played or until there's no tile left on the pool
			while ((nShift < playerTotalShifts) && 0 != bag.getSize()) {
				int randomIdx = bag.randomLetterIdx();
				char letter = bag.getLetter(randomIdx);
				player.addLetterOnHand(letter);
				bag.removeLetter(randomIdx);
				lettersAdd += std::string() + "'" + (char)(letter)+"' ";

				nShift++;
			}

			// Display the letters add to the hand of the player
			writeColoredText(player.getColor(), "You received " + std::to_string(nShift - 1) + " from the pool: " + lettersAdd);

			Sleep(TIME_TO_WAIT + 500);
		}
	}
	else if (0 == bag.getSize()) {
		writeWarning("THE POOL IS EMPTY, YOU CANNOT RECEIVE ANY TILES!");

		Sleep(TIME_TO_WAIT + 500);
	}

} // updatePlayerHand

//==================================================

/**
 * Inicializes players' hand at max with 7 tiles
 * @param players - all players on game
 * @param bag - all letters needed to construct the board
 */
void inicializePlayersHand(std::vector<Player> &players, Pool &bag) {
	size_t tilesPerPlayer;
	size_t numberOfPlayers = players.size();

	if (numberOfPlayers * 7 <= bag.getSize()) {
		tilesPerPlayer = 7;
	}
	else {
		tilesPerPlayer = bag.getSize() / numberOfPlayers;
	}

	for (size_t p = 0; p < numberOfPlayers; p++) {
		Player player = players.at(p);
		for (size_t t = 0; t < tilesPerPlayer; t++) {
			int randomIdx = bag.randomLetterIdx();
			player.addLetterOnHand(bag.getLetter(randomIdx));
			bag.removeLetter(randomIdx);
		}

		players.at(p) = player;
	}
} // inicializePlayersHand

//==================================================

/**
 * Read the player atributes (name, age, and his letters' color)
 * @param numberOfThePlayer - corresponding index of the player to define
 * @param colors - all colors defined by the program
 * @return a defined player
 */
Player definePlayer(const size_t numberOfThePlayer, Colors &colors) {
	clearScreen();
	formatingGameTitleOuput();
	bool validInput;

	// Read the name of the player
	std::string name = "";
	do {
		validInput = true;
		std::cout << " Player " << numberOfThePlayer << " - Nickname (1 word; no more than 15 characters): ";
		validInput = isInputCorrect(name) && (15 >= name.size());

		if (!validInput) {
			writeWarning(" Invalid Name!\n");
		}
	} while (!validInput);

	std::cout << std::endl;

	// Read the age of the player
	std::string ageStr;
	int age = 0;
	do {
		validInput = true;
		std::cout << " Player " << numberOfThePlayer << " - Age: ";
		validInput = isInputCorrect(ageStr) && tryConvertToNumber(ageStr, age);

		if (validInput) {
			if (1 > age || 110 < age) {
				validInput = false;
			}
		}

		if (!validInput) {
			writeWarning(" Invalid Age!\n");
		}
	} while (!validInput);

	std::cout << std::endl;

	// Get colors chosen by other players (Colored letters are used to distinguish the tiles of each player)
	std::vector<int> emptyColorIdx = colors.writeAvailableColors();

	// Read the color of the player
	std::string colorNumberStr;
	int colorNumber = 0;
	do {
		validInput = true;
		std::cout << "\n Player " << numberOfThePlayer << " - Board Color (number): ";
		validInput = isInputCorrect(colorNumberStr) && tryConvertToNumber(colorNumberStr, colorNumber);

		if (validInput) {
			// Answer has to be between the limits of the array 
			// Answer cannot be found on the vector of empty indexes (colors already choosen by other players)
			if (1 > colorNumber || colorNumber > colors.getArrayLength() || std::find(emptyColorIdx.begin(), emptyColorIdx.end(), colorNumber - 1) != emptyColorIdx.end()) {
				validInput = false;
			}
		}

		if (!validInput) {
			writeWarning(" Invalid Color!");
		}
	} while (!validInput);

	// Define player
	Player player(name, age, colors.getColorAvailable(colorNumber - 1), colors.getBackgroundgColorAvailable(colorNumber - 1));
	colors.setColorAvailable("", colorNumber - 1);

	return (player);

} // definePlayer

//==================================================

/**
 * Format and display the warning of invalid answer
 * @param questionLength - Length of the question we asked
 * @param errorMessage - message of the error to display
 */
void warningOfSecondTry(const short questionLength, const std::string &errorMessage) {
	setCursorPosition(0, 5);
	writeWarning(errorMessage + "\n <Press any key to continue>\n");

	// User needs to press any key to continue
	int pressedKey = _getch();
	if (0 <= pressedKey) {
		clearLine(0, 5);
		clearLine(0, 6);
		setCursorPosition(questionLength + 1, (short)4);
	}
} // warningOfSecondTry

//==================================================

/**
 * Read the corresponding number of the first player to start the game
 * @param numberOfPlayers - total number of players
 * @return the index of the player chosen
 */
size_t askFirstPlayerOfGame(const size_t numberOfPlayers) {
	int numberFirstPlayer = 0;
	std::string numberFirstPlayerStr;
	bool validInput;
	bool secondTry = false;

	std::string question = " First Player of The Game (1 to " + std::to_string(numberOfPlayers) + " OR '0' to random): ";

	// Validate answer
	do {
		validInput = true;
		clearScreen();

		formatingGameTitleOuput();
		std::cout << question;

		// Gave invalid answer
		if (secondTry) {
			warningOfSecondTry((short)question.length(), " Not a valid answer!");
		}

		validInput = isInputCorrect(numberFirstPlayerStr) && tryConvertToNumber(numberFirstPlayerStr, numberFirstPlayer);

		if (validInput) {
			// Conditions required based on the answer
			if (0 > numberFirstPlayer || (int)numberOfPlayers < numberFirstPlayer) {
				validInput = false;
			}
			else if (0 == numberFirstPlayer) {
				numberFirstPlayer = generateRandomNumberBetween(0, numberOfPlayers - 1);
			}
			else {
				numberFirstPlayer--;
			}
		}

		if (!validInput) {
			secondTry = true;
		}
	} while (!validInput);

	return ((size_t)numberFirstPlayer);
} // askFirstPlayerOfGame

//==================================================

/**
 * Read the number of players that are going to play
 * @return total number of players
 */
int askHowManyPlayers() {
	int numberPlayers = 0;
	std::string numberPlayersStr;
	bool validInput;
	bool secondTry = false;

	std::string question = " How many players are going to Play the Game (2 to 4): ";

	// Validate answer
	do {
		validInput = true;
		clearScreen();

		formatingGameTitleOuput();
		std::cout << question;

		// Gave invalid answer
		if (secondTry) {
			warningOfSecondTry((short)question.length(), " Not a valid answer!");
		}

		validInput = isInputCorrect(numberPlayersStr) && tryConvertToNumber(numberPlayersStr, numberPlayers);

		if (validInput) {
			// Conditions required
			if (2 > numberPlayers || 4 < numberPlayers) {
				validInput = false;
			}
		}

		if (!validInput) {
			secondTry = true;
		}
	} while (!validInput);

	return (numberPlayers);
} // askHowManyPlayers

//==================================================

/**
 * Read the name of the file of the board
 * @return the name of the file
 */
std::string askBoardFileName() {
	std::string boardFile;
	bool validInput;
	bool secondTry = false;

	std::string question = " Name of the file of the board (without '.txt'): ";

	// Validate answer
	do {
		validInput = true;
		clearScreen();

		formatingGameTitleOuput();
		std::cout << question;

		// Gave invalid answer
		if (secondTry) {
			warningOfSecondTry((short)question.length(), " File can't be open!");
		}

		// The file must be able to be opened and his format must be '.txt'
		validInput = isBoardFileNameCorrect(boardFile) && canFileBeOpen(boardFile);

		if (!validInput) {
			secondTry = true;
		}

	} while (!validInput);

	return (boardFile);
} // askBoardFileName

//==================================================

int main()
{
	// Set up Windows Console to accept Ansi Escape Codes
	#ifdef MY_UNICODE  
	if (!setUpConsole()) {
		std::cerr << "\n Something went wrong setting up the console. The display is compromised, please check the code." << std::endl;
		std::cout << "\n <Press any key to leave>";
		if (char pressedKey = (char)_getch()) {
			return (-1);
		}
	}
	#endif
	srand((unsigned)time(NULL));

	do {
		clearScreen();

		// Get informations needed to start the game
		std::string boardFile = askBoardFileName();
		size_t numberOfPlayers = askHowManyPlayers();

		// Inicialize Board
		Board board(boardFile);
		// Inicialize Pool
		Pool pool(board.getAllBoardLetters());

		// Check if the board chosen can be played with this amount of players
		// Every player must have at least on tile in their hand
		if (numberOfPlayers > pool.getSize()) {
			writeWarning("\n This board is too small for the amount of players!");
			writeWarning("\n <Press any key to continue>\n\n");

			if (char key = (char)_getch()) {
				continue;
			}
		}

		// Define Players
		std::vector<Player> players;
		Colors colors;
		for (size_t p = 0; p < numberOfPlayers; p++) {
			players.push_back(definePlayer(p + 1, colors));
		}

		// Ask who will be the first player of the game
		size_t turnOfPlayerIdx = askFirstPlayerOfGame(numberOfPlayers);

		// Inicialize Players' Hand
		inicializePlayersHand(players, pool);

		// Show board and all players' hand before starting the game
		showPlayersHandBeforeGame(players, board, turnOfPlayerIdx);

		// ------- Start of the game ------------
		// Show board on its starting positon
		displayBoardStartingPosition(board);

		// Only the first player of the game can play 2 consecutive tiles in the same word
		bool isFirstPlayOfGame = true;
		unsigned int consetivePass = 0;

		// Until the game end OR until is impossible to end it
		while (!isEndOfGame(pool, players) && !isImpossibleGame(consetivePass, players)) {
			if (turnOfPlayerIdx >= numberOfPlayers) { turnOfPlayerIdx = 0; }

			Player currentPlayer = players.at(turnOfPlayerIdx);
			unsigned int playerShift = 1;
			std::string forbiddenCoordinates = "";

			// Ask each player at most 2 moves
			while (2 >= playerShift) {

				// Clear the rigth side of the screen, next to the board
				clearRightSideOfScreen(board.getColumns());

				// Display Name and Leters of the current player
				writePlayerHeader(board.getColumns(), currentPlayer, turnOfPlayerIdx);
				writeColoredText(NO_COLOR, "THIS IS YOUR " + std::to_string(playerShift) + " SHIFT");

				if (playerCanPlay(board, currentPlayer, forbiddenCoordinates, isFirstPlayOfGame)) {
					askPlayerMove(board, currentPlayer, forbiddenCoordinates);

					consetivePass = 0;
					Sleep(TIME_TO_WAIT);
				}
				// Player cannot do any move
				else {
					if (1 == playerShift) {
						if (0 != pool.getSize()) {
							// Change at most 2 tiles with pool 
							changeTilesWithPool(currentPlayer, pool, board);
							consetivePass = 0;
						}
						else {
							// Player is force to pass
							writeWarning("You can't do any move and Pool is empty! You must pass!");
							consetivePass++;
						}
					}
					else {
						writeWarning("There is no other move that you can do!");
					}

					Sleep(TIME_TO_WAIT);
					break;
				}

				// Next move of the current player
				playerShift++;
			}
			// Replace the number of tiles that player played with tiles of the pool (if possible)
			updatePlayerHand(currentPlayer, pool, playerShift, board.getColumns());

			players.at(turnOfPlayerIdx) = currentPlayer;

			// Is the turn of the next player
			turnOfPlayerIdx++;
			isFirstPlayOfGame = false;
		}
	} while (isToPlayAgain());
	// While user wants to play

	setCursorPosition(0, 12);

	return(0);
}
