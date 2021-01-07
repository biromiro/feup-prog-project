// Auxiliar.h : This file contains the definition of auxiliar
//				methods and auxiliar class 'Colors'
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef AUXILIAR_H
#define AUXILIAR_H

// DEFINE TEXT COLOR CODES 
#define NO_COLOR "\033[0m"
#define BLACK "\033[0;30m"
#define WARNING "\033[31m"
// #define RED "\033[31m"
#define GREEN "\033[32m"
#define BLUE "\033[1;34m"
#define PURPLE "\033[35m"
#define PINK "\033[1;31m"
#define ORANGE "\033[33m"

// DEFINE WHITE BACKGROUND COLOR CODE WITH 
// DIFERENT TEXT COLOR CODES 
#define BG_BLACK "\033[47;30m"
// #define BG_RED "\033[47;31m"
#define BG_GREEN "\033[47;32m"
#define BG_BLUE "\033[47;1;34m"
#define BG_PURPLE "\033[47;35m"
#define BG_PINK "\033[47;1;31m"
#define BG_ORANGE "\033[47;33m"

#include <iostream>
#include <string>
#include <vector> 
#include <sstream> 

// AUXILIAR METHODS AND FUNCTIONS
void saveCursorPosition();
void restoreCursorPosition();
void setCursorPosition(short column, short line);
void clearLine(short column, short line);
void clearScreen();
std::string repeatString(const std::string &str, const int n);
std::string trimString(std::string &str);
bool isInputCorrect(std::string &inputStr);
bool isBoardFileNameCorrect(std::string &inputStr);
bool tryConvertToNumber(const std::string &input, int &intInput);
int generateRandomNumberBetween(int lower, int upper);
bool canFileBeOpen(const std::string &fileName);
// The next two functions are related with the color of the text displayed in the screen
void setTextColor(const std::string &color);
void writeColoredText(const std::string &color, const std::string &text);

// AUXILIAR CLASS 'COLORS'
// methods needed to show the user what colors can he choose from when defining a player
class Colors
{
private:
	int const _arrayLength = 5;
	std::string _colorsAvailable[5] = { GREEN , BLUE, PURPLE, PINK, ORANGE };
	std::string _backgrounds[5] = { BG_GREEN , BG_BLUE, BG_PURPLE, BG_PINK, BG_ORANGE };
public:
	void setColorAvailable(const std::string &text, int idx);
	std::string getColorAvailable(int idx) const;
	std::string getBackgroundgColorAvailable(int idx) const;
	int getArrayLength() const;
	std::vector<int> writeAvailableColors() const;
};

#endif // !AUXILIAR_H
