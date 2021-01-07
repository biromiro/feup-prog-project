// BoardBuilder.h : This file contains the definition of the class 'BoardBuilder'
//
// BOARD BUILDER
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <set>
#include <sstream>
#include <utility>
#include <random>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <conio.h>
#include "NewWord.h"

// Loading bar values and characters
constexpr const char *PBSTR = "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||";
constexpr const int PBWIDTH = 60;

// COLOR ANSI CODES
constexpr const char *CLEAR_SCREEN = "\x1b[2J";
constexpr const char *LINE_UP = "\033[A";
constexpr const char *CLEAR_LINE = "\x1b[2K";
constexpr const char *BG_BLACK = "\033[47;30m";
constexpr const char *RED = "\x1b[0;31m";
constexpr const char *GREEN = "\x1b[0;32m";
constexpr const char *YELLOW = "\x1b[0;33m";
constexpr const char *BLUE = "\x1b[0;34m";
constexpr const char *CYAN = "\x1b[0;36m";
constexpr const char *RESET = "\x1b[0m";
constexpr const char *HIDE_CURSOR = "\x1b[?25l";
constexpr const char *SHOW_CURSOR = "\x1b[?25h";

class Board
{
private:
	size_t _width;
	size_t _height;
	std::vector<std::vector<char>> _board;

	// Both this private parameters refer to a unique newMove
	// Meaning that the intersections refer to the NewMove's intersections
	// not all of them that exist in the board
	NewWord _newMove;
	std::set<std::pair<int, int>> _intersections;

	std::set<std::string> _words;
	std::set<NewWord> _placedWords;
public:
	Board();
	void setBoard(int width, int height, std::string &fileName);
	void loadDictionary(std::string &fileName);
	int placeWord(char row, char column, char orientation, std::string &word);
	void randomBuild();
	bool removeWord(const std::string &word);
	size_t getBoardWidth() const;
	size_t getBoardHeight() const;
	int getNumberOfPlacedWords() const;
	int getFilledTiles();
	int validPlacement();
	bool isInBounds() const;
	bool isInDictionary(const std::string& word) const;
	bool wasAlreadyUsed(const std::string& word) const;
	bool isValidIntersect();
	bool isInValidSurroundings();
	bool isInValidSurroundingsVertical();
	bool isInValidSurroundingsHorizontal();
	void outputBoardToFile(const std::string &fileName) const;
	void showBoard(std::ostream &out) const;
	void updateBoard();
	void progressBar(double percentage) const;
};

#endif // !BOARD_H
