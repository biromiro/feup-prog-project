// Pool.h : This file contains the definition of the class 'Pool'
//			(set of all letters needed to construct the board)
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef POOL_H
#define POOL_H

#include <iostream>
#include <string>
#include <vector>

class Pool
{
private:
	std::vector<char> _boardLetters;
public:
	Pool();
	Pool(const std::vector<char> &allBoardLetters);
	void setLetter(char letter, int idx);
	char getLetter(int idx) const;
	int randomLetterIdx() const;
	void removeLetter(int idx);
	void removeLetter(char letter);
	void addLetter(char letter);
	size_t getSize() const;
};

#endif // !POOL_H
