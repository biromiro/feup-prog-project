// NewWord.h : This file contains the definition of the class 'NewWord'
//
// BOARD BUILDER
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef NEWWORD_H
#define NEWWORD_H

#include <iostream>
#include <string>

//==================================================

class NewWord
{
private:
	size_t _row;
	size_t _column;
	char _orientation;
	std::string _word;
public:
	friend class Board;
	NewWord();
	NewWord(char row, char column, char orientation, std::string &word);
	bool operator<(const NewWord &newWord) const;
	void setPlacement(char row, char column, char orientation, std::string &word);
	std::string getWord() const;
	size_t getRow() const;
	size_t getColumn() const;
	char getOrientation() const;
};

#endif // !NEWWORD_H
