// Player.cpp : This file contains the implementation 
//			    of the class 'Player' methods and functions
// 
// PLAY SCRABBLE JUNIOR GAME
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <iostream>
#include <string>
#include <cassert>

#include "Player.h"
#include "Auxiliar.h"

//==================================================

/**
 * Default/Empty constructor of the class 'Player'
 */
Player::Player() {
	_name = "Undefined";
	_age = 0;
	_color = "";
	_backgroundColor = "";
} // Player

//==================================================

/**
 * Complete constructor of the class 'Player'
 * @param name - name of the player
 * @param age - player's age
 * @param color - text color of the player
 * @param bgColor - background color of the text of the player
 */
Player::Player(const std::string &name, unsigned int age, const std::string &color, const std::string &bgColor) {
	_name = name;
	_age = age;
	_color = color;
	_backgroundColor = bgColor;
} // Player

//==================================================

/**
 * Change the name of the player
 * @param name - value of the new name
 */
void Player::setName(const std::string &name) {
	_name = name;
} // setName

//==================================================

/**
 * Change the age of the player
 * @param age - value of the new age
 */
void Player::setAge(const unsigned int age) {
	_age = age;
} // setAge

//==================================================

/**
 * Change the text color of the player
 * @param color - value of the new text color
 */
void Player::setColor(const std::string &color) {
	_color = color;
} // setColor

//==================================================

/**
 * Change the background color of the text of the player
 * @param bgColor - value of the new backgroung color
 */
void Player::setBackgroundColor(const std::string &backgroundColor) {
	_backgroundColor = backgroundColor;
} // setBackgroundColor

//==================================================

/**
 * Change the number of chips of the player
 * @param chips - value of the new number of chips
 */
void Player::setChips(const short chips) {
	_chips = chips;
} // setChips

//==================================================

/**
 * Add a number of chips to the current number of chips of the player
 * @param chips - value of the number of chips to add
 */
void Player::addChips(const short chips) {
	_chips += chips;
} // addChips

//==================================================

/**
 * Return the name of the player
 * @return the private atribute '_name'
 */
std::string Player::getName() const {
	return (_name);
} // getName

//==================================================

/**
 * Return the age of the player
 * @return the private atribute '_age'
 */
unsigned int Player::getAge() const {
	return (_age);
} // getAge

//==================================================

/**
 * Return the text color of the player
 * @return the private atribute '_color'
 */
std::string Player::getColor() const {
	return (_color);
} // getColor

//==================================================

/**
 * Return the background color of the text of the player
 * @return the private atribute '_backgroundColor'
 */
std::string Player::getBackgroundColor() const {
	return (_backgroundColor);
} // getColor

//==================================================

/**
 * Return the number of chips of the player
 * @return the private atribute '_chips'
 */
short Player::getChips() const {
	return (_chips);
} // getChips

//==================================================

/**
 * Return the value of a letter of the player hand in a certain index
 * @param idx - index of the letter we want to access
 * @return the value of the specific letter
 */
char Player::getLetterOnHand(int idx) const {
	assert((idx >= 0) && (idx < (int)_hand.size()));

	auto itr = _hand.begin();
	// It will move forward the passed iterator by passed value
	std::advance(itr, idx);

	// Another option: auto itr = std::next(setOfStr.begin(), idx);

	return ((char)*itr);
} // getLetter

//==================================================

/**
 * Add a specific letter to the hand of the player
 * @param letter - value of the letter to add
 */
void Player::addLetterOnHand(char letter) {
	_hand.insert(letter);
} // addLetter

//==================================================

/**
 * Remove a specific letter from the hand of the player
 * @param letter - value of the letter to remove
 */
void Player::removeLetterOfHand(char letter) {
	auto itr = _hand.find(letter);
	if (itr != _hand.end()) {
		_hand.erase(itr);
	}
} // removeLetter

//==================================================

/**
 * Check if a specific letter can be found in the hand of the player
 * @param letter - value of the letter we want to test
 * @return TRUE if the letter belongs to the player's hand; FALSE otherwise
 */
bool Player::isLetterOnHand(char letter) const {
	auto itr = _hand.find(letter);
	if (itr != _hand.end()) {
		return (true);
	}

	return (false);
} // isLetterOnHand

//==================================================

/**
 * Return the dimensions of the hand of the player
 * @return the total number of letters that are in player's hand
 */
size_t Player::getHandSize() const {
	return (_hand.size());
} // getHandSize

//==================================================

/**
 * Display the letters of the player in is color
 * @param startColumn - referenced column where to write it
 * @param stratLine - reference line where to write it
 */
void Player::showHand(const short startColumn, const short stratLine) const {
	int handSize = getHandSize();

	// [Format: ]
	// ┌───┐ ┌───┐ ┌───┐ ┌───┐ ┌───┐
	// | A | | A | | A | | A | | A | 
	// └───┘ └───┘ └───┘ └───┘ └───┘
	// Extended ASCII Characters Codes:
	// ┌ 218 | ┐ 191 | └ 192 | ┘ 217 | ─ 196 

	if (0 != getHandSize()) {
		const unsigned char chr1 = 218, chr2 = 191, chr3 = 192, chr4 = 217, chr5 = 196;

		std::string line(3, chr5);
		std::string topLine = std::string(1, chr1) + line + std::string(1, chr2);
		std::string bottomLine = std::string(1, chr3) + line + std::string(1, chr4);

		setCursorPosition(startColumn, stratLine);
		std::cout << repeatString(" " + topLine, handSize);
		setCursorPosition(startColumn, stratLine + 1);

		for (char itr : _hand) {
			std::cout << " | ";
			writeColoredText(getColor(), std::string() + itr);
			std::cout << " |";
		}
		setCursorPosition(startColumn, stratLine + 2);
		std::cout << repeatString(" " + bottomLine, handSize);
	}
	else {
		setCursorPosition(startColumn, stratLine + 1);
		writeColoredText(WARNING, "NO TLES IN YOUR HAND");
	}
}
