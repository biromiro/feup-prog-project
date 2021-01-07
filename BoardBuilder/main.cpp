// main.cpp : This file contains the 'main' function. Program execution begins and ends here.
//
// BOARD BUILDER
// Authors :	Nuno Costa, T1_201906272
//				Patrícia Oliveira, T1_201905427
//
//==================================================

#include <cctype>
#include "Board.h"

//==================================================

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

/**
   Sets up the console in order to accept ANSI codes. Code from Microsoft Docs.
   https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences#example-of-enabling-virtual-terminal-processing

   @return True if it set up the ANSI codes correctly, false otherwise.
 */
bool setUpConsole() {
	// Set output mode to handle virtual terminal sequences
	if (!SetConsoleCP(CP_UTF8) || !SetConsoleOutputCP(CP_UTF8)) {
		return (false);
	}
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
		// Failed to set VT input mode, can't do anything here.
		return (false);
	}
	DWORD dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
	if (!SetConsoleMode(hOut, dwOutMode)) {
		// we failed to set both modes, try to step down mode gracefully.
		dwOutMode = dwOriginalOutMode | ENABLE_VIRTUAL_TERMINAL_PROCESSING;
		if (!SetConsoleMode(hOut, dwOutMode)) {
			// Failed to set any VT mode, can't do anything here.
			return (false);
		}
	}
	return (true);
}

#endif //_WIN32

//==================================================

/**
 * Sets the console cursor to a given positions
 * @param x - The x axis coordinate
 * @param y - The y axis coordinate
 */
void goToPosition(const unsigned int x, const unsigned int y) {
	std::cout << "\x1b[" << y << ';' << x << 'H';
} // goToPosition

//==================================================

/**
 * Shows the program title to the screen
 */
void showTitle() {
	std::cout << "      ---------------------\n"
		"      Board Builder Program\n\n";
} // showTitle

//==================================================

/**
 * Shows the start message to the screen
 */
void promptStartMessage() {
	showTitle();
	std::cout << HIDE_CURSOR;

	std::cout << " Hello and welcome to the " << YELLOW << "Board Builder program" << RESET << ".\n\n Here you can, "
		<< RED << "manually" << RESET << " or " << RED << "randomly" << RESET << ", "
		"generate a custom board for the Scrabble Junior Game.\n All you have to do is, firstly, "
		"input the board size.\n From then on, it's your choice if you'd like to build it by "
		"yourself or randomly.\n\n -If you choose to build it manually, write "
		<< YELLOW << "!help" << RESET << " to get to know "
		"the commands!\n -If you choose to build it randomly, just hold on to your seat and let "
		"the magic be done!\n -If you're not happy with the board at any point, you can always "
		"restart by using the command " << YELLOW << "!rebuild" << RESET
		<< " to start over.\n\n Now it's up to you!\n\n"
		<< RED << " PRESS ANY KEY TO CONTINUE" << RESET;

	if (char pressedKey = (char)_getch()) {};
	std::cout << SHOW_CURSOR;
} // promptStartMessage

//==================================================

/**
 * Reads a board attribute (row or column) from user input
 * @param attribute - The number of rows/columns
 * @param name - Defines what is to be defined (either rows or columns)
 * @return The number of the chosen parameter
 */
int getBoardAttribute(size_t attribute, const std::string &name) {

	// clean - true if the input is invalid and needs to be cleaned up
	// first - true if it is the first iteration (for error messages)
	bool clean = false, first = true;

	std::cout << " Please input the number of " << name << " you'd like your board to have (1-20): ";

	while (true) {

		// Tries to get input
		if ((std::cin >> attribute)) {
			// Char to check if there are chars remaining in the buffer
			char c;

			clean = false;

			// Cleans up the buffer
			// even if it is an input success, if there are still remaining characters
			// in the buffer, it is considered an error
			while (std::cin.get(c) && '\n' != c) {
				if (!std::isspace(c)) {
					clean = true;
				}
			}

			// Checks if the main condition of the board is true
			if ((attribute < 1 || attribute > 20) && !clean) {
				clean = true;

			}

			// Displays error message
			if (clean) {
				if (!first) { std::cout << LINE_UP << CLEAR_LINE << "\r"; }
				std::cerr << RED << " It has to be a number from 1 to 20! " << RESET << "Please try again : ";
			}
			else {
				break;
			}

		}
		else {
			// Clears if cin failed
			std::cin.clear();
			std::cin.ignore(1000, '\n');

			// Error message on the same line if it is the first time
			if (!first) {
				std::cout << LINE_UP << CLEAR_LINE << "\r";
			}
			std::cerr << RED << " It has to be a number from 1 to 20! " << RESET << "Please try again : ";
		}

		first = false;
	}
	return (attribute);
} // getBoardAttribute

//==================================================

/**
 * Reads a console action
 * @param consoleAction - The console code of the action
 * @param word - If !add or !remove, defines the word of the corresponding action
 * @param row - If !add, defines the word's row
 * @param col - If !add, defines the word's col
 * @param orientation - If !add, defines the word's orientation
 */
void readAction(std::string &consoleCode, std::string &word, char &row, char &col, char &orientation, bool &syntaxError) {

	// command - corresponds to the full code wrote to the console
	// action - conversion of command to a stringstream
	std::string command = "";
	std::stringstream action;

	// Gets user input
	std::cout << "\n Please input an action (" << YELLOW << "!help" << RESET << " if you don't know what to do): ";
	getline(std::cin, command);

	action << command;
	action >> consoleCode >> word >> row >> col >> orientation;

	// Transform the code attributes to the according size (lowerCase or upperCase)
	transform(consoleCode.begin(), consoleCode.end(), consoleCode.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });
	transform(word.begin(), word.end(), word.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });
	col = (char)tolower((int)col);
	row = (char)toupper((int)row);
	orientation = (char)toupper((int)orientation);

	// size - counts how many chars were missing / how many were in excess
	// actionSpaces - number of spaces according to each action
	// operatorSpaces - number of spaces filled with operators for each action
	int size = 0, actionSpaces = 0, operatorSpaces;

	// Detection of syntax errors
	if ("!REBUILD" == consoleCode || "!HELP" == consoleCode || "!FINISH" == consoleCode) {
		actionSpaces = 0;
		operatorSpaces = 0;
		size = command.size() - consoleCode.size();
	}
	else if ("!ADD" == consoleCode) {
		actionSpaces = 4;
		operatorSpaces = 3;
		size = command.size() - consoleCode.size() - word.size() - operatorSpaces - actionSpaces;
	}
	else if ("!REMOVE" == consoleCode) {
		actionSpaces = 1;
		operatorSpaces = 0;
		size = command.size() - consoleCode.size() - word.size() - actionSpaces;
	}

	// If size isn't 0, then there must be a syntax 0 (if it's negative there are missing
	// parameters, if its positive (>0) there are excessive parameters)
	if (0 != size) {
		consoleCode = "!incorrectSyntax";
		syntaxError = true;
	}

	std::cout << CLEAR_SCREEN;
	goToPosition(0, 0);
} // readAction

//==================================================

/**
 * Outputs the console codes for the !help code
 */
void getConsoleCodes() {
	std::cout << CLEAR_SCREEN;
	goToPosition(0, 0);
	showTitle();
	std::cout << HIDE_CURSOR << " These are the actions you can perform:\n\n"
		<< YELLOW << " !rebuild" << RESET << " -  restart the building process\n"
		<< YELLOW << " !add" << GREEN << " WORD " << CYAN << "ROW COL" << BLUE << " ORIENTATION" << RESET
		<< " - add a new word\n"
		<< YELLOW << " !remove" << GREEN << " WORD " << RESET << "- remove a existing word\n"
		<< YELLOW << " !finish " << RESET << "- close the build and save to text file\n\n"
		<< RED << " PRESS ANY KEY TO CONTINUE" << RESET;
	if (char pressedKey = (char)_getch()) {};
	std::cout << CLEAR_SCREEN;
	goToPosition(0, 0);
	std::cout << SHOW_CURSOR;
} // getConsoleCodes

//==================================================

/**
 * Defines which build the user pretends to use (either manual or random)
 * @param board - The board to be applied the option
 */
void buildOption(Board &board) {

	// option - defines user's option to build board
	char option = ' ';

	if ((board.getBoardHeight()*board.getBoardWidth() <= 4)) { return; }

	std::cout << " In what way would you like to build your board?\n\n"
		<< RED << " PRESS M FOR MANNUALLY OR R FOR RANDOMLY" << RESET;

	while (true) {
		// Gets user choice
		option = (char)toupper(_getch());

		// If random, executes a random build
		if ('R' == option) {
			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);
			std::cout << HIDE_CURSOR;
			showTitle();
			board.randomBuild();
			std::cout << SHOW_CURSOR;
			break;
		}

		// If mannually, leaves to normal board builder
		if ('M' == option) {
			break;
		}
	}
} // buildOption

//==================================================

/**
 * Reads the chosen dictionary's name from input
 * @return The dictionary's file name
 */
std::string getDictionaryName() {

	// fileName - the name of the file
	// fin - the input stream to test the availability of the file
	std::string fileName;
	std::ifstream fin;

	std::cout << " Please input the dictonary's name (without '.txt'): ";
	getline(std::cin, fileName);
	fileName = "../" + fileName + ".txt";

	// Tries to open file
	fin.open(fileName);
	// If unsuccessful prompts user until an input is ok
	while (fin.fail()) {
		std::cout << LINE_UP << CLEAR_LINE << "\r";
		std::cerr << RED << " File not found. " << RESET << "Try again: ";
		getline(std::cin, fileName);
		fileName += ".txt";
		fin.open(fileName);
	}

	fin.close();
	return (fileName);
} // getDictionaryName

//==================================================

/**
 * Reads the chosen output file's name from input
 * @return The output file's name
 */
std::string getOutputFileName() {
	// fileName - the output's file name
	std::string fileName;

	std::cout << "\n Please input the output file's name (without '.txt'): ";
	getline(std::cin, fileName);
	fileName += ".txt";

	return (fileName);
} // getOutputFileName

//==================================================

int main() {
	// Sets up Windows Console to accept Ansi Escape Codes
	#ifdef MY_UNICODE 
	if (!setUpConsole()) {
		std::cerr << "\n Something went wrong setting up the console. The display is compromised, please check the code." << std::endl;
		std::cout << "\n Press any key to leave.";
		if (char pressedKey = (char)_getch()) {
			return (-1);
		}
	}
	#endif

	// Sets up new random seed everytime the board builder runs
	srand((unsigned)time(0));

	// rows - the number of rows of the board
	// columns - the number of columns of the board
	// result - the code of validPlacement() --> 0 if fine, other number for specific error message
	// canBeRemoved - true if a word can be removed
	// syntaxError - true if there was a syntax error
	// consoleCode - the action code
	// word - word to add/remove
	// row, col, orientation - attributes of the add code
	size_t rows = 0, columns = 0;
	int result = 0;
	bool canBeRemoved = false, syntaxError = false;
	std::string consoleCode = "!REBUILD", word = "", filename = "words.txt";
	char row = ' ', col = ' ', orientation = ' ';

	// Start board builder
	promptStartMessage();

	Board board;

	while (true) {
		std::cout << CLEAR_SCREEN;
		goToPosition(0, 0);

		if ("!REBUILD" == consoleCode) {
			showTitle();
			std::string dictionaryName = getDictionaryName();
			board.setBoard(getBoardAttribute(columns, "columns"), getBoardAttribute(rows, "rows"), dictionaryName);

			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			showTitle();
			buildOption(board);

			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			showTitle();
			board.showBoard(std::cout);

			readAction(consoleCode, word, row, col, orientation, syntaxError);

		}
		else if ("!HELP" == consoleCode) {
			getConsoleCodes();

			showTitle();
			board.showBoard(std::cout);

			readAction(consoleCode, word, row, col, orientation, syntaxError);
		}
		else if ("!ADD" == consoleCode) {
			result = board.placeWord(row, col, orientation, word);

			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			showTitle();
			board.showBoard(std::cout);

			if (!('H' == toupper(orientation) || 'V' == toupper(orientation))) {
				std::cerr << RED << " The orientation is invalid." << RESET;
			}
			else if (1 == result) {
				std::cerr << RED << " The word wasn't found in the dictionary." << RESET;

			}
			else if (2 == result) {
				std::cerr << RED << " The word seems to already be in the board." << RESET;

			}
			else if (3 == result) {
				std::cerr << RED << " The word does not fit within the board bounds." << RESET;

			}
			else if (4 == result) {
				std::cerr << RED << " The word invalidly intersects another." << RESET;

			}
			else if (5 == result) {
				std::cerr << RED << " The word creates another invalid words." << RESET;

			}

			readAction(consoleCode, word, row, col, orientation, syntaxError);
		}
		else if ("!REMOVE" == consoleCode) {
			canBeRemoved = board.removeWord(word);

			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			showTitle();
			board.showBoard(std::cout);

			if (!canBeRemoved) {
				std::cerr << RED << " The word wasn't found." << RESET;
			}

			readAction(consoleCode, word, row, col, orientation, syntaxError);
		}
		else if ("!FINISH" == consoleCode) {
			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			board.updateBoard();
			showTitle();
			board.showBoard(std::cout);

			char answer = ' ';
			if (board.getNumberOfPlacedWords() < 1) {
				std::cerr << RED << " You need to place at least one word." << RESET;
				readAction(consoleCode, word, row, col, orientation, syntaxError);
				continue;
			}
			else if (board.getFilledTiles() < 14) {
				std::cout << YELLOW << " This board might lack letters to play Scrabble Junior." << RESET << " You sure you want to finish (y/n)?" << HIDE_CURSOR;
				while ('Y' != answer && 'N' != answer) {
					answer = (char)toupper(_getch());
				}
				std::cout << SHOW_CURSOR;
				if ('N' == answer) {
					readAction(consoleCode, word, row, col, orientation, syntaxError);
					continue;
				}
			}
			
			board.outputBoardToFile(getOutputFileName());

			std::cout << CLEAR_SCREEN;
			goToPosition(0, 0);

			char finish = ' ';

			showTitle();
			std::cout << " Do you wish to build another board (y/n)?" << HIDE_CURSOR;
			while ('Y' != finish && 'N' != finish) {
				finish = (char)toupper(_getch());
			}

			if ('N' == finish) {
				std::cout << CLEAR_SCREEN;
				goToPosition(0, 0);

				std::cout << "\n Thank you for using Board Builder!\n\n"
					<< RED << " PRESS ANY KEY TO LEAVE" << RESET;
				if (char pressedKey = (char)_getch()) {};
				break;
			}
			consoleCode = "!REBUILD";
		}
		else {
			while ("!REBUILD" != consoleCode && "!HELP" != consoleCode && "!ADD" != consoleCode &&
				"!REMOVE" != consoleCode && "!FINISH" != consoleCode) {
				std::cout << CLEAR_SCREEN;
				goToPosition(0, 0);

				showTitle();
				board.showBoard(std::cout);

				if (syntaxError) {
					std::cerr << RED << " Invalid syntax. Try again." << RESET;
					syntaxError = false;

				}
				else {
					std::cerr << RED << " Unrecognised command. Try again." << RESET;

				}

				readAction(consoleCode, word, row, col, orientation, syntaxError);
			}
		}
		std::cout << CLEAR_SCREEN;
	}

	return (0);
}
