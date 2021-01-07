// Board.cpp : This file contains the implementation 
//			    of the class 'Board' methods and functions
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <fstream> 
#include <cassert>

#include "Board.h"
#include "Auxiliar.h"

//==================================================

/**
 * PRIVATE METHOD: Calcules the number of the line that correspond to a certain coordinate
 * @param coordinate - the specific coordinates [Format: 'Aa' <=> 'LINEcolumn']
 * @return the number that result from the calculation
 */
int Board::getCoordinateLine(std::string coordinate) const {
	// 'A' code = 65
	return ((int)(toupper(coordinate[0])) - 65 + 1);
} // getCoordinateLine

//==================================================

/**
 * PRIVATE METHOD: Calcules the number of the column that correspond to a certain coordinate
 * @param coordinate - the specific coordinates [Format: 'Aa' <=> 'LINEcolumn']
 * @return the number that result from the calculation
 */
int Board::getCoordinateColumn(std::string coordinate) const {
	// 'a' code = 97
	return ((int)(tolower(coordinate[1])) - 97 + 1);
} // getCoordinateColumn

//==================================================

/**
 * PRIVATE METHOD: Remove a specific word (word was finished)
 * @param idx - index of the element to remove
 */
void Board::removeWordFinished(size_t idx) {
	_words.erase(_words.begin() + idx);
} // removeWordFinished

//==================================================

/**
 * Default/Empty constructor of the class 'Board'
 */
Board::Board() {
	_lines = 0;
	_columns = 0;
}

//==================================================

/**
 * Complete constructor of the class 'Board'
 * @param boardFile - file name of the board
 */
Board::Board(const std::string &boardFile) {
	// Read the content from the board file
	std::ifstream boardFileContent(boardFile);
	std::string fileLine;

	// The first line of the file contains the board dimensions
	std::getline(boardFileContent, fileLine);
	std::istringstream issFileLine(fileLine);
	std::string separator;
	issFileLine >> _lines >> separator >> _columns;

	// ----- Construct the board WITHOUT depending on the 2D view at the end of the file -----
	// Resize and inicialize '_tiles' according the board dimensions
	Tile emptyTile;
	_tiles.resize(_lines, std::vector<Tile>(_columns, emptyTile));

	// Till the end of the file or a empty line, save the list of words of the board
	// [Format: LINEcolumn orientation(H/V) WORD]
	std::getline(boardFileContent, fileLine);

	while (!boardFileContent.fail() && !fileLine.empty()) {
		// List of the words needed to complete the board
		_words.push_back(fileLine);
		
		// Save each line on '_tiles' (matrix that represent the board)
		std::istringstream issWordLine(fileLine);
		std::string coordinate, word;
		char orientation;

		issWordLine >> coordinate >> orientation >> word;

		int lineIdx = getCoordinateLine(coordinate) - 1;
		assert((lineIdx >= 0) && (lineIdx < (int)_lines));

		int columnIdx = getCoordinateColumn(coordinate) - 1;
		assert((columnIdx >= 0) && (columnIdx < (int)_columns));

		// Save in the corresponding coordinates the words, according to its orientation
		Tile wordFirstTile(word.at(0), lineIdx, columnIdx);
		_tiles.at(lineIdx).at(columnIdx) = wordFirstTile;

		for (size_t c = 1; c < word.size(); c++) {
			if ('H' == orientation) {
				columnIdx++;
			}
			else {
				lineIdx++;
			}

			Tile newTile(word.at(c), lineIdx, columnIdx);
			_tiles.at(lineIdx).at(columnIdx) = newTile;
		}

		std::getline(boardFileContent, fileLine);
	}

	// Construct the board lines 
	// (makes easier to draw the board and get all the necessary letters to play it)
	std::string newBoardLine;

	// First line correspond to the legend of the columns
	newBoardLine = " ";
	for (size_t i = 0; i < _columns; i++) {
		newBoardLine += "  " + std::string() + (char)('a' + i);
	}
	_boardLines.push_back(newBoardLine);

	// On the next lines, the first character corresponds to the legend of the line
	for (size_t i = 0; i < _lines; i++) {
		newBoardLine = std::string() + (char)('A' + i) + " ";
		for (size_t j = 0; j < _columns; j++) {
			if ('\0' == _tiles.at(i).at(j).getLetter()) {
				newBoardLine += "   ";
			}
			else {
				newBoardLine += " " + std::string() + _tiles.at(i).at(j).getLetter() + " ";
			}
		}
		_boardLines.push_back(newBoardLine);
	}

	// ----- Construct the board BASED ON the 2D view at the end of the file -----
	/*
	// Till a empty line, save the list of words of the board
	// [Format: LINEcolumn orientation(H/V) WORD]
	std::getline(boardFileContent, fileLine);
	while (!fileLine.empty()) {
		_words.push_back(fileLine);
		std::getline(boardFileContent, fileLine);
	}

	// In the end of the file is a 2D view of the board on his starting position
	std::getline(boardFileContent, fileLine);

	// The first line of the 2D view is the legend, that does not have tiles
	int lineIdx = -1; unsigned int columnIndx;
	std::vector<Tile> tilesInLine;

	while (!boardFileContent.fail()) {
		// Ignore the space before the legend of the line 
		// (according to the format of the 2D view of the file generated by the board builder)
		fileLine = fileLine.substr(1);
		_boardLines.push_back(fileLine);

		if (-1 != lineIdx) {
			tilesInLine.resize(0);
			// Save all board tiles in their respective position
			// 3 is the distance between the letters on the 2D view
			for (size_t c = 3; c < fileLine.size(); c = c + 3) {
				if (' ' != fileLine[c]) {
					columnIndx = (c / 3) - 1;
					Tile boardTile(fileLine[c], lineIdx, columnIndx);
					tilesInLine.push_back(boardTile);
				}
				else {
					Tile emplyTile;
					tilesInLine.push_back(emplyTile);
				}
			}
			_tiles.push_back(tilesInLine);
		}

		lineIdx++;
		std::getline(boardFileContent, fileLine);
	} 
	*/

	boardFileContent.close();

} // Board

//==================================================

/**
 * Change the number of columns of the board
 * @param columns - value of the new number of columns
 */
void Board::setColumns(unsigned int columns) {
	_columns = columns;
} // setColums

//==================================================

/**
 * Change the number of line of the board
 * @param columns - value of the new number of line
 */
void Board::setLines(unsigned int lines) {
	_lines = lines;
} // setLines

//==================================================

/**
 * Return the total number of columns of the board
 * @return the private atribute '_columns'
 */
unsigned int Board::getColumns() const {
	return (_columns);
} // getColums

//==================================================

/**
 * Return the total number of lines of the board
 * @return the private atribute '_lines'
 */
unsigned int Board::getLines()  const {
	return (_lines);
} // getLines

//==================================================

/**
 * Get all letters of the board from the lines of his 2D view
 * @return all the letters needed to complete the board
 */
std::vector<char> Board::getAllBoardLetters() const {
	std::string s = "";
	for (size_t l = 1; l < _boardLines.size(); l++) {
		std::string line = _boardLines.at(l);

		// Remove spaces from the line
		line.erase(remove_if(line.begin(), line.end(), isspace), line.end());

		// Select from the second caracter to the end of the string (not include the line legend)
		s += line.substr(1);
	}
	// Create a vector with all of the letters of the board from a string
	std::vector<char> allLetters(s.begin(), s.end());

	return (allLetters);
} // getAllBoardLetters

//==================================================

/**
 * Check if a specific coordinate is part of the board
 * @param coordinate - coordinates to test
 * @return TRUE if the coordinates is part of the board; FALSE otherwise
 */
bool Board::isCoordinatesInBoardRange(const std::string &coordinate) const {
	int line = getCoordinateLine(coordinate);
	int column = getCoordinateColumn(coordinate);

	return (1 <= line && line <= (int)_lines && 1 <= column && column <= (int)_columns);

} // isCoordinatesInBoardRange

//==================================================

/**
 * Check if the value of the letter in a specific coordinate equals the value of a certain letter
 * @param coordinate - coordinates to test
 * @param letter - value of the certain letter
 * @return TRUE if they match; FALSE otherwise
 */
bool Board::coordinatesMatchLetter(const std::string &coordinate, char letter) const {
	int lineIdx = getCoordinateLine(coordinate) - 1;
	assert((lineIdx >= 0) && (lineIdx < (int)_lines));

	int columnIdx = getCoordinateColumn(coordinate) - 1;
	assert((columnIdx >= 0) && (columnIdx < (int)_columns));

	Tile tile = _tiles.at(lineIdx).at(columnIdx);

	return (tile.getLetter() == letter);

} // coordinatesMatchLetter

//==================================================

/**
 * Return the state of a specific tile of the board
 * @param coordinate - coordinates of the tile
 * @return TRUE if the tile was already played; FALSE otherwise
 */
bool Board::isLetterAlreadyPlayed(const std::string &coordinate) const {
	int lineIdx = getCoordinateLine(coordinate) - 1;
	assert((lineIdx >= 0) && (lineIdx < (int)_lines));

	int columnIdx = getCoordinateColumn(coordinate) - 1;
	assert((columnIdx >= 0) && (columnIdx < (int)_columns));

	Tile tile = _tiles.at(lineIdx).at(columnIdx);

	return (tile.getAlreadyPlayed());
} // letterAlreadyPlayed

//==================================================

/**
 * Check if a specific coordinate corresponds to the fist letter of any word
 * @param coordinate - coordinates to test
 * @return TRUE if there is any word that starts in that coordinate; FALSE otherwise
 */
bool Board::isFirstLetterOfWord(const std::string &coordinate) const {
	std::string firstLettersCoordinates;

	for (const auto & _word : _words) {
		/*std::istringstream wordStrStream(_words.at(w));
		wordStrStream >> firstLettersCoordinates;*/
		firstLettersCoordinates = _word.substr(0, 2);

		if (firstLettersCoordinates == coordinate) {
			return (true);
		}
	}

	return (false);
} // isFirstLetterOfWord

//==================================================

/**
 * Check if a specific letter corresponds to the fist letter of any word
 * @param letter - letter to test
 * @return TRUE if there is any word that starts with that letter; FALSE otherwise
 */
bool Board::isFirstLetterOfWord(char letter) const {
	std::string firstLettersCoordinates;
	char orientation;
	std::string word;

	for (const auto & _word : _words) {
		std::istringstream wordStrStream(_word);
		wordStrStream >> firstLettersCoordinates >> orientation >> word;

		if (word.at(0) == letter) {
			return (true);
		}
	}

	return (false);
} // isFirstLetterOfWord

//==================================================

/**
 * Check if all valid coordinates to play of a certain letter can be found in "forbidden coordinates"
 * @param letter - value of the letter to test
 * @param forbiddenCoords - coordinates that cannot be played
 * @return FALSE if at least one coordinate cannot be found in the forbidden ones; TRUE otherwise
 */
bool Board::isLetterAllValidCoordsForbidden(char letter, const std::string &forbiddenCoords) const {
	std::string allValidCoords = allCoordinatesWhereIsFirstLetter(letter);

	for (size_t c = 0; c < allValidCoords.size(); c = c + 3) {
		std::string coord = allValidCoords.substr(c, 2);
		// Check if there is a valid coordinate that is not a forbidden one
		if (std::string::npos == forbiddenCoords.find(coord)) {
			return (false);
			// There is at least one move that can be done with this letter
			// Not all are forbidden
		}
	}

	return (true);
} // isLetterAllValidCoordsForbidden

//==================================================

/**
 * Return all the coordinates where a certain letter is a first letter of a word
 * @param letter - value of the letter to test
 * @return the resulting string of the concatenation of all valid coordinates separeted by a space
 */
std::string Board::allCoordinatesWhereIsFirstLetter(char letter) const {
	std::string firstLettersCoordinates;
	char orientation;
	std::string word;

	std::string coordinates = "";
	for (const auto & _word : _words) {
		std::istringstream wordStrStream(_word);
		wordStrStream >> firstLettersCoordinates >> orientation >> word;

		if (word.at(0) == letter) {
			coordinates += firstLettersCoordinates + " ";
		}
	}

	return (coordinates);
} // allCoordinatesWhereIsFirstLetter

//==================================================

/**
 * Update the considered first letter of the words that were afected by a specific move
 * @param coordinate - coordinates of the move
 * @param letter - letter of the move
 * @return a pair with the number of words finished and the forbidden coordinates for the next move
 */
std::pair <int, std::string> Board::updateWords(const std::string &coordinate) {

	// When the move is valid we need to update the considered first letter of the words
	std::string firstLetterCoordinates;
	char orientation;
	std::string word;
	int numberWordsFinished = 0;
	std::string forbiddenCoordinates = "";

	for (size_t w = 0; w < _words.size(); w++) {
		std::istringstream issWords(_words.at(w));
		issWords >> firstLetterCoordinates >> orientation >> word;

		if (firstLetterCoordinates == coordinate) {
			int lineIdx = getCoordinateLine(firstLetterCoordinates) - 1;
			int columnIdx = getCoordinateColumn(firstLetterCoordinates) - 1;

			size_t letterIdxInWord = 1;
			bool isNextFirstLetter = false;
			while (!isNextFirstLetter) {
				if ('V' == orientation) {
					lineIdx++;
				}
				else {
					columnIdx++;
				}

				// If the next letter if out of the boarders of the board
				// Is the last letter of a vertical word OR is the last letter of a horizontal word
				if ((lineIdx >= (int)_lines && 'V' == orientation) || (columnIdx >= (int)_columns && 'H' == orientation)) {
					numberWordsFinished++;
					removeWordFinished(w);
					w--;
					// To not give out of range of vector
					break;
				}

				Tile nextWordTile = _tiles.at(lineIdx).at(columnIdx);

				// Is the end of a word
				//if (NULL == nextWordTile.getLetter()) {
				if ('\0' == nextWordTile.getLetter()) {
					numberWordsFinished++;
					removeWordFinished(w);
					w--;
					isNextFirstLetter = true;
				}
				// Tile was not played yet
				else if (!nextWordTile.getAlreadyPlayed()) {
					// Save this new coordinates and the "New word" (substring of the original) 
					firstLetterCoordinates = std::string() + (char)(lineIdx + 65) + (char)(columnIdx + 97);
					word = word.substr(letterIdxInWord);
					_words.at(w) = std::string() + firstLetterCoordinates + " " + orientation + " " + word;

					// The new coordinates are now forbidden coordinates (cannot choose the next letter of the same word)
					forbiddenCoordinates += firstLetterCoordinates + " ";
					isNextFirstLetter = true;
				}

				letterIdxInWord++;
			}
		}
	}

	std::pair <int, std::string> result;
	result.first = numberWordsFinished;
	result.second = forbiddenCoordinates;

	return (result);

} // updateWords

//==================================================

/**
 * Calcules the number of the line and column that correspond to a certain coordinate
 * @param coordinate - the specific coordinates [Format: 'Aa' <=> 'LINEcolumn']
 * @return a pair with the number of the line and the number of the column
 */
std::pair <int, int> Board::getIntCoordinates(const std::string &coordinate) const {
	std::pair <int, int> intCoords;
	intCoords.first = getCoordinateLine(coordinate);
	intCoords.second = getCoordinateColumn(coordinate);

	return (intCoords);
} // getIntCoordinates

//==================================================

/**
 * Change the state of a certain tile of the board
 * @param coordinate - in the specific coordinates [Format: 'Aa' <=> 'LINEcolumn']
 * @param played - indicates whether the tile was already played or not
 */
void Board::changeCoordinatesPlayability(const std::string &coordinate, bool alreadyPlayed) {
	int lineIdx = getCoordinateLine(coordinate) - 1;
	assert((lineIdx >= 0) && (lineIdx < (int)_lines));

	int columnIdx = getCoordinateColumn(coordinate) - 1;
	assert((columnIdx >= 0) && (columnIdx < (int)_columns));

	_tiles.at(lineIdx).at(columnIdx).setAlreadyPlayed(alreadyPlayed);
} // setAlreadyPlayedOfTile

//==================================================

/**
 * Display the board on his starting position, in a given line and column of the screen
 * @param startLine - line where is to start drawing
 * @param startColumn - column where is to start drawing
 */
void Board::drawBoard(short startLine, short startColumn) const {
	setTextColor(NO_COLOR);

	// PLAYABLE AREA OF THE BOARD: BLACK CHARACTERS AND WHITE BACKGROUND
	for (size_t l = 0; l < _boardLines.size(); l++) {
		setCursorPosition(startColumn, startLine + (short)l);
		if (0 == l) {
			std::cout << _boardLines.at(l);
		}
		else {
			std::cout << _boardLines.at(l).substr(0, 2);
			setTextColor(BG_BLACK);
			std::cout << _boardLines.at(l).substr(2);
		}
		setTextColor(NO_COLOR);
	}
} // drawBoardStartingPosition
