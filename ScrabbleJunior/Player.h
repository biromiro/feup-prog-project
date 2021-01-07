// Player.h : This file contains the definition of the class 'Player'
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
#include <vector>
#include <set>

class Player
{
private:
	std::string _name;
	unsigned int _age;
	std::string _color;
	std::string _backgroundColor;
	short _chips = 0;
	std::multiset <char> _hand;
public:
	Player();
	Player(const std::string &name, unsigned int age, const std::string &color, const std::string &bgColor);
	void setName(const std::string &name);
	void setAge(const unsigned int age);
	void setColor(const std::string &color);
	void setBackgroundColor(const std::string &backgroundColor);
	void setChips(const short chips);
	void addChips(const short chips);
	std::string getName() const;
	unsigned int getAge() const;
	std::string getColor() const;
	std::string getBackgroundColor() const;
	short getChips() const;
	char getLetterOnHand(int idx) const;
	void addLetterOnHand(char letter);
	void removeLetterOfHand(char letter);
	bool isLetterOnHand(char letter) const;
	size_t getHandSize() const;
	void showHand(const short startColumn, const short stratLine) const;
};

#endif // !PLAYER_H
