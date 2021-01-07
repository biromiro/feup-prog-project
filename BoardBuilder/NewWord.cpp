// NewWord.cpp : This file contains the implementation
//			     of the class 'NewWord' methods and functions
//
// BOARD BUILDER
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <algorithm>

#include "NewWord.h"

//==================================================

/**
 * Default/Empty constructor of class NewWord
 */
NewWord::NewWord() {
	_row = 0;
	_column = 0;
	_orientation = ' ';
	_word = "UNDEFINED";
} // NewWord

//==================================================

/**
 * Complete constructor of class NewWord
 * @param row - Defines NewWord's row
 * @param column - Defines NewWord's columns
 * @param orientation - Defines NewWord's orientation
 * @param word Defines NewWord's actual word
 */
NewWord::NewWord(char row, char column, char orientation, std::string &word) {
	_row = (int)row - 'A';                     // Converting row and column
	_column = (int)column - 'a';               // From ascii letter to a numerical value
	_orientation = orientation;

	// Converting word to uppercase
	transform(word.begin(), word.end(), word.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); } );

	_word = word;
} // NewWord

//==================================================

/**
 * Defines NewWord's 'less than' operator
 * @param newW - NewWord object which is to be compared with NewWord
 * @return TRUE if newW's word is smaller than NewWord's, FALSE otherwise
 */
bool NewWord::operator<(const NewWord &newW) const {
	return (_word < newW._word);
} // operator<

//==================================================

/**
 * Defines the placement of the NewWord
 * @param row - Defines NewWord's row
 * @param column - Defines NewWord's columns
 * @param orientation - Defines NewWord's orientation
 * @param word - Defines NewWord's actual word
 */
void NewWord::setPlacement(char row, char column, char orientation, std::string &word) {
	_row = (int)row - 'A';                     // Converting row and column
	_column = (int)column - 'a';               // From ascii letter to a numerical value
	_orientation = orientation;

	// Converting a word to uppercase
	transform(word.begin(), word.end(), word.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });

	_word = word;
} // setPlacement

//==================================================

/**
 * Method to get NewWord's word
 * @return NewWord's word
 */
std::string NewWord::getWord() const {
	return (_word);
} // getWord

//==================================================

/**
 * Method to get NewWord's row in the board
 * @return NewWord's board row
 */
size_t NewWord::getRow() const {
	return (_row);
} // getRow

//==================================================

/**
 * Method to get NewWord's column in the board
 * @return NewWord's board column
 */
size_t NewWord::getColumn() const {
	return (_column);
} // getColumn

//==================================================

/**
 * Method to get NewWord's orientation in the board
 * @return NewWord's board orientation
 */
char NewWord::getOrientation() const {
	return (_orientation);
} // getOrientation
