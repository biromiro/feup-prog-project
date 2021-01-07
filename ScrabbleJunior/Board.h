// Board.h : This file contains the definition of the class 'Board'
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <string>
#include <vector>

#include "Tile.h"

class Board
{
private:
	unsigned int _columns;
	unsigned int _lines;
	std::vector<std::vector<Tile>> _tiles;
	std::vector<std::string> _words;
	std::vector<std::string> _boardLines;
	int getCoordinateLine(std::string coordinate) const;
	int getCoordinateColumn(std::string coordinate) const;
	void removeWordFinished(size_t idx);
public:
	Board();
	Board(const std::string &boardFile);
	void setColumns(unsigned int columns);
	void setLines(unsigned int lines);
	unsigned int getColumns() const;
	unsigned int getLines() const;
	std::vector<char> getAllBoardLetters() const;
	bool isCoordinatesInBoardRange(const std::string &coordinate) const;
	bool coordinatesMatchLetter(const std::string &coordinate, char letter) const;
	bool isLetterAlreadyPlayed(const std::string &coordinate) const;
	bool isFirstLetterOfWord(const std::string &coordinate) const;
	bool isFirstLetterOfWord(char letter) const;
	bool isLetterAllValidCoordsForbidden(char letter, const std::string &forbiddenCoords) const;
	std::string allCoordinatesWhereIsFirstLetter(char letter) const;
	std::pair <int, std::string> updateWords(const std::string &coordinate);
	std::pair <int, int> getIntCoordinates(const std::string &coordinate) const;
	void changeCoordinatesPlayability(const std::string &coordinate, bool alreadyPlayed);
	void drawBoard(short startLine, short startColumn) const;
};

#endif // !BOARD_H
