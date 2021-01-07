// Pool.cpp : This file contains the implementation 
//			  of the class 'Pool' methods and functions
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <cassert>

#include "Pool.h"
#include "Auxiliar.h"

//==================================================

/**
 * Default/Empty constructor of the class 'Pool'
 */
Pool::Pool() {}

//==================================================

/**
 * Complete constructor of the class 'Pool'
 * @param allBoardLetters - all letters needed to construct the board
 */
Pool::Pool(const std::vector<char> &allBoardLetters) {
	_boardLetters = allBoardLetters;
} // Pool

//==================================================

/**
 * Change the value of a letter in a certain index
 * @param letter - new value
 * @param idx - index where to change it
 */
void Pool::setLetter(char letter, int idx) {
	assert((idx >= 0) && (idx < (int)_boardLetters.size()));
	_boardLetters.at(idx) = letter;
} // setLetter

//==================================================

/**
 * Return the value of a letter in a certain index
 * @param idx - index of the letter we want to access
 * @return the value of the specific letter
 */
char Pool::getLetter(int idx) const {
	assert((idx >= 0) && (idx < (int)_boardLetters.size()));
	return (_boardLetters.at(idx));
} // getLetter

//==================================================

/**
 * Generate a index between 0 and the dimensions of the pool
 * @return the number generated
 */
int Pool::randomLetterIdx() const {
	return (generateRandomNumberBetween(0, _boardLetters.size() - 1));
} // randomLetterIdx

//==================================================

/**
 * Remove the letter of a specific index
 * @param idx - index of the letter to remove
 */
void Pool::removeLetter(int idx) {
	assert((idx >= 0) && (idx < (int)_boardLetters.size()));
	_boardLetters.erase(_boardLetters.begin() + idx);
} // removeLetter

//==================================================

/**
 * Remove a specific letter
 * @param letter - value of the letter to remove
 */
void Pool::removeLetter(char letter) {
	for (size_t l = 0; l < _boardLetters.size(); l++) {
		if (_boardLetters.at(l) == letter) {
			_boardLetters.erase(_boardLetters.begin() + l);
			break;
		}
	}
} // removeLetter

//==================================================

/**
 * Add a specific letter
 * @param letter - value of the letter to add
 */
void Pool::addLetter(char letter) {
	_boardLetters.push_back(letter);
} // addLetter

//==================================================

/**
 * Return the dimensions of the pool
 * @return the total number of letters left in the pool
 */
size_t Pool::getSize() const {
	return (_boardLetters.size());
} // getSize
