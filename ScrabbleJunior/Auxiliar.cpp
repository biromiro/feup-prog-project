// Auxiliar.cpp : This file contains the implementation of the auxiliar
//				  methods and auxiliar class 'Colors' functions
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <iostream>
#include <string>
#include <sstream> 
#include <vector>
#include <fstream> 

#include "Auxiliar.h"

//==================================================

/**
 * Save the current position of the cursor on the screen
 */
void saveCursorPosition() {
	std::cout << "\033[s";
} // saveCursorPosition

//==================================================

/**
 * Retstore the cursor to the saved position
 */
void restoreCursorPosition() {
	std::cout << "\033[u";
} // restoreCursorPosition

//==================================================

/**
 * Moves the cursor to a specified position
 * @param column - number of the column (x axis)
 * @param line - number of the line (y axis)
 */
void setCursorPosition(short column, short line) {
	std::ostringstream oss;
	oss << "\033[" << line << ";" << column << "H";
	std::cout << oss.str();
} // setCursorPosition

//==================================================

/**
 * Erase from a certain cursor position to the end of the line
 * @param column - number of the column (x axis)
 * @param line - number of the line (y axis) to erase
 */
void clearLine(short column, short line) {
	saveCursorPosition();
	setCursorPosition(column, line);
	std::cout << "\033[K"; // Erase line
	restoreCursorPosition();
} // clearLine

//==================================================

/**
 * Erase display
 */
void clearScreen() {
	std::cout << "\033[2J";
	saveCursorPosition();
	std::cout.flush();
} // clearScreen

//==================================================

/**
 * Change the color of the text displayed
 * @param color - foreground color to change to
 */
void setTextColor(const std::string &color)
{
	std::cout << color;
} // setTextColor

//==================================================

/**
 * Display text in a specific color
 * @param color - foreground color to write the text
 * @param text - will be written on the console
 */
void writeColoredText(const std::string &color, const std::string &text) {
	setTextColor(color);
	if (WARNING == color) {
		std::cerr << text;
	}
	else {
		std::cout << text;
	}
	setTextColor(NO_COLOR);
} // writeColoredText

//==================================================

/**
 * Repeat a string a certain number of times
 * @param str - string to repeat
 * @param n - number of times to repeat it
 * @return the string result
 */
std::string repeatString(const std::string &str, const int n) {
	std::string repeatedStr = str;

	for (int i = 1; i < n; i++) {
		repeatedStr += str;
	}

	return repeatedStr;
} // repeatStr

//==================================================

/**
 * Erase the spaces surrounding a string
 * @param str - string to be modified
 * @return the string without spaces
 */
std::string trimString(std::string &str)
{
	str.erase(0, str.find_first_not_of(' '));   // Erase prefixing spaces
	str.erase(str.find_last_not_of(' ') + 1);   // Erase surfixing spaces
	return (str);
} // trimString

//==================================================

/**
 * Reads a line and checks if that input respects specific conditions
 * (one and only one word must be read)
 * @param inputStr - the result of the line read
 * @return FALSE when the input is considered incorrect; TRUE otherwise
 */
bool isInputCorrect(std::string &inputStr) {
	getline(std::cin, inputStr, '\n');

	if (std::cin.fail() || std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return (false);
	}

	//Empty inputs are not valid, such as \n or spaces
	if (trimString(inputStr).empty()) {
		return (false);
	}

	// Only one word if valid: check if there are any spaces in the middle of the string
	return (std::string::npos == inputStr.find(' '));

} // isInputCorrect

//==================================================

/**
 * Reads a line and checks if the input for the fileName of the board is correct
 * @param inputStr - the string with the fileName
 * @return TRUE if the filename is correct, FALSE otherwise.
 */
bool isBoardFileNameCorrect(std::string &inputStr) {
	getline(std::cin, inputStr, '\n');

	if (std::cin.fail() || std::cin.eof()) {
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		return (false);
	}

	//Empty inputs are not valid, such as \n or spaces
	if (trimString(inputStr).empty()) {
		return (false);
	}

	inputStr += ".txt";
	return (true);
} // isBoardFileNameCorrect

//==================================================

/**
 * Try to convert a string to an integer
 * @param input - string to convert
 * @param intInput - result of the conversion
 * @return FALSE when it's not a valid string to convert; TRUE otherwise
 */
bool tryConvertToNumber(const std::string &input, int &intInput) {
	// String must only contain digits
	if (std::string::npos != input.find_first_not_of("0123456789")) {
		return (false);
	}
	try
	{
		intInput = stoi(input);
		return (true);
	}
	catch (std::exception)
	{
		return (false);
	}
} // tryConvertToNumber

//==================================================

/**
 * Generate random number between two limits
 * @param lower - lower limit
 * @param upper - upper limit
 * @return integer generated
 */
int generateRandomNumberBetween(int lower, int upper) {
	return (rand() % (upper - lower + 1) + lower);
} // generateRandomNumberBetween

//==================================================

/**
 * Checks if a file can be open
 * @param fileName - name of the file to try to open
 * @return FALSE if file doesn't exists; TRUE otherwise
 */
bool canFileBeOpen(const std::string &fileName) {
	bool valid = true;

	std::ifstream file;
	file.open(fileName);

	if (file.fail() || !file.is_open()) {
		valid = false;
	}

	file.close();

	return (valid);
} // fileCanBeOpen

//==================================================
//==================================================
// IMPLEMENTATIONS OF THE PUBLIC METHODS AND 
// FUNCTIONS OF THE CLASS COLORS
//==================================================

/**
 * Set a new color to a certain position of the vector 'colorsAvailable'
 * @param text - change the element to this text
 * @param idx - index of the element to change
 */
void Colors::setColorAvailable(const std::string &text, int idx) {
	_colorsAvailable[idx] = text;
} // setColorAvailable

//==================================================

/**
 * Return the color in a certain position
 * @param idx - index of the element to get
 * @return text in that element
 */
std::string Colors::getColorAvailable(int idx) const {
	return (_colorsAvailable[idx]);
} // getColorAvailable

//==================================================

/**
 * Return the background color in a certain position
 * @param idx - index of the element to get
 * @return text in that element
 */
std::string Colors::getBackgroundgColorAvailable(int idx) const {
	return (_backgrounds[idx]);
} // getBackgroundgColorAvailable

//==================================================

/**
 * Return the number of colors/background colors available
 * @return the value of the private variable 'arrayLength'
 */
int Colors::getArrayLength() const {
	return (_arrayLength);
} // getArrayLength

//==================================================

/**
 * Write the available colors for the user to choose from
 * @return indexes of the colors already chosen by others users
 */
std::vector<int> Colors::writeAvailableColors() const {
	std::cout << " COLORS : | ";
	std::vector<int> emptyColorIdx;

	for (int i = 0; i < _arrayLength; i++) {
		if (!_colorsAvailable[i].empty()) {
			writeColoredText(_colorsAvailable[i], std::to_string(i + 1) + " - TEXT");
			std::cout << " | ";
		}
		else {
			emptyColorIdx.push_back(i);
		}
	}

	return (emptyColorIdx);
} // writeAvailableColors
