// Tile.h : This file contains the definition of the class 'Tile'
//			(The Tile represents a house of the board)
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef TILE_H
#define TILE_H

#include <iostream>
#include <string>

class Tile
{
private:
	char _letter;
	unsigned int _line;
	unsigned int _column;
	bool _playedBefore = false;
public:
	Tile();
	Tile(char letter, unsigned int line, unsigned int column);
	// Doesn't have set of the letter, line and column because 
	// the tile value and position on the board cannot change
	char getLetter() const;
	unsigned int getLine() const;
	unsigned int getColumn() const;
	bool getAlreadyPlayed() const;
	void setAlreadyPlayed(bool played = true);
};

#endif // !TILE_H
