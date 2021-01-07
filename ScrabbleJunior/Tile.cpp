// Tile.cpp : This file contains the implementation 
//			  of the class 'Tile' methods and functions
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include "Tile.h"

//==================================================

/**
 * Default/Empty constructor of the class 'Tile'
 */
Tile::Tile() {
	_letter = '\0';
	_line = 0;
	_column = 0;
} // Tile

//==================================================

/**
 * Complete constructor of the class 'Tile'
 * @param letter - value of the line
 * @param line - index of the line of the tile in a board
 * @param column - index of the column of the tile in a board
 */
Tile::Tile(char letter, unsigned int line, unsigned int column) {
	_letter = letter;
	_line = line;
	_column = column;
} // Tile

//==================================================

/**
 * Return the value of the tile
 * @return private atribute '_letter'
 */
char Tile::getLetter() const {
	return (_letter);
} // getLetter

//==================================================

/**
 * Return the index of the line of the tile in a board
 * @return private atribute '_line'
 */
unsigned int Tile::getLine() const {
	return (_line);
} // getLine

//==================================================

/**
 * Return the index of the column of the tile in a board
 * @return private atribute '_column'
 */
unsigned int Tile::getColumn() const {
	return (_column);
} // getColumn

//==================================================

/**
 * Return the state of the tile
 * @return TRUE if the tile was already played; FALSE otherwise
 */
bool Tile::getAlreadyPlayed() const {
	return (_playedBefore);
} // getAlreadyPlayed

//==================================================

/**
 * Change the state of the tile
 * @param played - indicates whether the tile was already played or not
 */
void Tile::setAlreadyPlayed(bool played) {
	_playedBefore = played;
} // setAlreadyPlayed
