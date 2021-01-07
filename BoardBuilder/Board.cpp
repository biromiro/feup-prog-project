// BoardBuilder.cpp : This file contains the implementation
//			     of the class 'BoardBuilder' methods and functions
//
// BOARD BUILDER
// Authors :	Nuno Costa, T1_201906272
//				Patricia Oliveira, T1_201905427
//
//==================================================

#include <iterator>

#include "Board.h"

//==================================================

/**
 * Default/Empty constructor of class BoardBuilder
 */
Board::Board() {
	_width = 20;
	_height = 20;
	_board.resize(_height, std::vector<char>(_width, ' '));
} // BoardBuilder

//==================================================

/**
 * Complete constructor of class BoardBuilder
 * @param width - The number of columns of the board
 * @param height - The number of rows of the board
 * @param fileName - The name of the dictionary file
 */
void Board::setBoard(int width, int height, std::string &fileName) {
	_width = width;
	_height = height;

	// Making sure all board characteristics are empty if it is a rebuild
	_board.clear();
	_words.clear();
	_placedWords.clear();
	_intersections.clear();

	// Resizing board according to user input
	_board.resize(_height, std::vector<char>(_width, ' '));
	loadDictionary(fileName);
} // setBoard

//==================================================

/**
 * Loads the dictionary according to the board size
 * @param fileName - The name of the dictionary file
 */
void Board::loadDictionary(std::string &fileName) {
	std::ifstream fin;
	std::string line;
	fin.open(fileName);

	if (fin.fail()) {
		std::cerr << "Failed to open words file";
		exit(1);
	}

	// Gets dictionary's words
	while (getline(fin, line)) {
		// Uppercase conversion
		transform(line.begin(), line.end(), line.begin(), [](int c) -> char { return static_cast<char>(::toupper(c)); });

		// Making sure there's no words with 1 letter and that no words
		// are bigger than the board itself in
		if ((2 > line.size()) || (line.size() > _width && line.size() > _height)) {
			continue;
		}

		_words.insert(line);
	}

	fin.close();
} // loadDictionary

//==================================================

/**
 * Tries to place a word in the board
 * @param row - The row of the new word to place
 * @param column - The column of the new word to place
 * @param orientation - The orientation of the new word to place
 * @param word - The new word to place
 * @return 0 if is valid, 1 if the word is not in the dict, 2 if it was already used, 3 if not in bounds, 4
 * if invalidly intersecting or 5 if its surroundings aren't valid
 */
int Board::placeWord(char row, char column, char orientation, std::string &word) {

	// Updates the current newMove object
	_newMove.setPlacement(row, column, orientation, word);

	// Result - the value of validPlacement - 0 if valid, another code if not
	int result = validPlacement();

	if (result == 0) {  // If the placement is valid
		std::istringstream inp(_newMove._word);

		// Letter by letter, every word is put in board according to its characteristics
		char letter;
		int newWCol = _newMove._column;
		int newWRow = _newMove._row;

		for (size_t j = 0; j < _newMove._word.size(); j++) {
			inp >> letter;
			_board[newWRow][newWCol] = letter;

			if ('H' == _newMove._orientation) {
				newWCol++;
			}
			else {
				newWRow++;
			}
		}
	}

	// Returns the value of validPlacement
	return (result);
} // placeWord

//==================================================

/**
 * Determines the number of tiles of the board that are filled in
 * @return The number of filled in tiles
 */
int Board::getFilledTiles() {
	int filledInTiles = 0;

	for (size_t i = 0; i < _height; i++) {
		for (size_t j = 0; j < _width; j++) {

			//if the board hasn't got a space in a specified position, it is
			//"not empty", by definition
			if (' ' != _board[i][j]) {
				filledInTiles++;
			}
		}
	}
	return (filledInTiles);
} // getFilledTiles

//==================================================

/**
 * Performs a random build of the board
 */
void Board::randomBuild() {
	// Percentage attributes for the loading bar
	double percentage, savedPercentage = 0;

	// Runs while the filled in tiles don't ammount to 40% of the board
	while ((int)((float)0.4 * (_width * _height)) > getFilledTiles()) {
		percentage = (getFilledTiles()) / ((float)0.4 * (_width * _height));

		// Load the bar only if there's change on it's progress or
		// If it is the first time running
		if ((0 < percentage - savedPercentage) || 0 == percentage) {
			progressBar(percentage);
			savedPercentage = percentage;
		}

		// Attributes of the randomMove
		char row, column, orientation;
		std::string word = "";

		// pos - random number to choose random word
		// int_row and int_col are the conversion to integer of the
		// randomMov attributes row and col
		// dif - how many spaces between the random position and the board there are
		// iter - dictionary iterator for random word choice
		int pos, int_row, int_col;
		size_t dif;
		std::set<std::string>::iterator iter;

		// Random calculation for the row and column
		row = (char)(rand() % (_height)+'A');
		column = (char)(rand() % (_width)+'a');

		int_row = (int)row % 'A';
		int_col = (int)column % 'a';

		// Random calculation for the orientation
		if (rand() % 2) {
			orientation = 'H';
			dif = _width - int_col;
		}
		else {
			orientation = 'V';
			dif = _height - int_row;
		}

		// If there are only one space or none between the board limit,
		// Skip this iteration
		if (dif < 3) { continue; }

		// Due to random limitations, it only produces numbers up to 32767, and there
		// are 108574 words with 2 or more characters in the dictionary
		// So bigRand will correspond to a combination of 2 random calls
		size_t size = _words.size(), bigRand = 0;


		// For better random performance, the words chosen were limited to 7 characters
		while (2 > word.size() || word.size() > dif || 7 < word.size()) {
			// Chooses a random word until the while condition isn't met
			bigRand = rand() << 15 | rand();
			iter = _words.begin();
			pos = bigRand % (size);
			std::advance(iter, pos);
			word = *iter;
		}
		// If all conditions were met, tries to place the word
		// The remaining code will determine if the word can or cannot be placed
		placeWord(row, column, orientation, word);
	}
} // randomBuild

//==================================================

/**
 * Removes a word from the board
 * @param word - The word to be removed
 * @return TRUE if the word can be and was removed, FALSE otherwise
 */
bool Board::removeWord(const std::string &word) {
	// Defines if the word was found or not
	bool wordFound = false;

	// NewWord object in case that the word is found
	NewWord found;

	for (const auto &elem : _placedWords) {
		if (elem._word == word) {
			wordFound = true;
			found = elem;
			break;
		}
	}

	if (wordFound) {
		// If the word is found, checks for surroundings
		size_t foundColumn = found.getColumn();
		size_t foundRow = found.getRow();
		size_t size = found._word.size();
		if ('H' == found.getOrientation()) {
			for (size_t i = foundColumn; i < foundColumn + size; i++) {

				if ((0 == foundRow || ' ' == _board[foundRow - 1][i]) &&
					(_height - 1 == foundRow || ' ' == _board[foundRow + 1][i])) {

					// Only removes the character from the board in case it doesn't
					// Disrupt other words already placed
					_board[foundRow][i] = ' ';
				}
			}
		}
		else if ('V' == found.getOrientation()) {
			for (size_t i = foundRow; i < foundRow + size; i++) {

				if ((0 == foundColumn || ' ' == _board[i][foundColumn - 1]) &&
					(_width - 1 == foundColumn || ' ' == _board[i][foundColumn + 1])) {

					// Same for the opposite orientation
					_board[i][foundColumn] = ' ';
				}
			}
		}

		_placedWords.erase(found);
		updateBoard();
		return (true);
	}
	else {
		return (false);
	}
} // removeWord

//==================================================

/**
 * Returns the board's width
 * @return The width of the board
 */
size_t Board::getBoardWidth() const {
	return (_width);
}// getBoardWidth

//==================================================

/**
 * Returns the board's height
 * @return The height of the board
 */
size_t Board::getBoardHeight() const {
	return (_height);
}// getBoardHeight

//==================================================

/**
 * Returns the number of words currently int the board
 * @return the number os elements of the atribute '_placedWords'
 */
int Board::getNumberOfPlacedWords() const {
	return (_placedWords.size());
} // getNumberOfPlacedWords

//==================================================

/**
 * Checks if a placement is valid and puts it in _placedWords if it is
 * @return 0 if is valid, 1 if the word is not in the dict, 2 if it was already used, 3 if not in bounds, 4
 * if invalidly intersecting or 5 if its surroundings aren't valid
 */
int Board::validPlacement() {
	if (!isInDictionary(_newMove._word)) {
		return (1);
	}
	else if (wasAlreadyUsed(_newMove._word)) {
		return (2);
	}
	else if (!isInBounds()) {
		return (3);
	}
	else if (!isValidIntersect()) {
		return (4);
	}
	else if (!isInValidSurroundings()) {
		return (5);
	}
	else {
		// If all conditions are met, the placement is valid
		NewWord newW;
		newW.setPlacement((char)(_newMove._row + 'A'), (char)(_newMove._column + 'a'), _newMove._orientation, _newMove._word);

		// Remove words that might be contained in the newly placed word
		std::set<NewWord> copyPlaced = _placedWords;

		for (const auto &elem : copyPlaced) {
			if (newW._word.find(elem._word) != std::string::npos) {
				if (('H' == newW._orientation) && ('H' == elem._orientation)) {
					if (elem._row == newW._row && elem._column >= newW._column
						&& (elem._column + elem._word.size()) <= (newW._column + newW._word.size())) {
						_placedWords.erase(elem);
					}
				}
				else if (('V' == newW._orientation) && ('V' == elem._orientation)) {
					if (elem._column == newW._column && elem._row >= newW._row
						&& (elem._row + elem._word.size()) <= (newW._row + newW._word.size())) {
						_placedWords.erase(elem);
					}
				}
			}
		}
		// Checks if there's a word in the board contained inside the new one

		_placedWords.insert(newW);
		
		return (0);
	}
} // validPlacement

//==================================================

/**
 * Checks if the word to place is within the board limits
 * @return TRUE if it is, FALSE otherwise
 */
bool Board::isInBounds() const {
	// Firstly checks if the position chosen is not within the board limits
	if (_newMove._column >= _width || _newMove._row >= _height) {
		return (false);

		// Then, according to is orientation, checks if the word fits
	}
	else if ('H' == _newMove._orientation) {
		return (_newMove._column + _newMove._word.size() <= _width);
	}
	else if ('V' == _newMove._orientation) {
		return (_newMove._row + _newMove._word.size() <= _height);
	}
	else {
		return (false);
	}
} // isInBounds

//==================================================

/**
 * Checks if a specific word is in the given dictionary
 * @param word - The word to check
 * @return TRUE if it is, FALSE otherwise
 */
bool Board::isInDictionary(const std::string &word) const {
	return (1 == _words.count(word));
} // isInDictionary

//==================================================

/**
 * Checks if a given word is already in the board
 * @param word - The word to check
 * @return TRUE if it is, FALSE otherwise
 */
bool Board::wasAlreadyUsed(const std::string& word) const {
	// Iterates the already placed words to check if there's already
	// an occurrence of the given word
	for (const auto &elem : _placedWords) {
		if (word == elem.getWord()) {
			return (true);
		}
	}
	return (false);
} // wasAlreadyUsed

//==================================================

/**
 * Checks if the word to place can intersect with the remaining words already placed
 * @return TRUE if it can, FALSE otherwise
 */
bool Board::isValidIntersect() {
	// index - respective word index according to iteration
	int index = 0, size = _newMove._word.size(), intersectCount = 0;

	// Cleans previous word intersections
	_intersections.clear();

	if ('H' == _newMove._orientation) {
		for (size_t i = _newMove._column; i < _newMove._column + _newMove._word.size(); i++) {

			if (' ' != _board[_newMove._row][i]) {

				// While iterating the word's board places, if there's a place that isn't a space
				// checks if the character on that place is the same as the word's character of
				// that index
				if (_board[_newMove._row][i] != _newMove._word[index]) {
					return (false);
				}

				// If that's true, that's a valid intersection point
				std::pair<int, int> coords(_newMove._row, i);
				_intersections.insert(coords);
				intersectCount++;
			}
			index++;
		}
	}
	else if ('V' == _newMove._orientation) {
		for (size_t i = _newMove._row; i < _newMove._row + _newMove._word.size(); i++) {

			if (' ' != _board[i][_newMove._column]) {

				// Same goes for the opposite orientation
				if (_board[i][_newMove._column] != _newMove._word[index]) {
					return (false);
				}

				std::pair<int, int> coords(i, _newMove._column);
				_intersections.insert(coords);
				intersectCount++;
			}
			index++;
		}
	}

	if (intersectCount == size) {
		return (false);
	}

	return (true);
} // isValidIntersect

//==================================================

/**
 * Checks if the surroundings of the word are valid (top and bottom if horizontal, left and right if vertical)
 * @return TRUE if it is, FALSE otherwise
 */
bool Board::isInValidSurroundings() {
	// Cheks the surroundings according to the word's orientation

	if ('H' == _newMove._orientation) {
		return (isInValidSurroundingsHorizontal());

	}
	else if ('V' == _newMove._orientation) {
		return (isInValidSurroundingsVertical());

	}
	else {
		return (false);
	}
} // isInValidSurroundings

//==================================================

/**
 * Checks if a horizontal word can be placed according to its surroundings
 * @return TRUE if it can, FALSE otherwise
 */
bool Board::isInValidSurroundingsHorizontal() {
	// initial - the NewWord's initial column
	// wordSize - the size of the NewWord
	// end - the final column of the NewWord
	size_t initial = _newMove._column;
	size_t wordSize = _newMove._word.size();
	size_t end = _newMove._column + wordSize;

	// Copy of end in case
	size_t endCopy = end;

	// Possible words behind and ahead of the NewWord
	std::string wordBehind = "", wordAhead = "";

	// While it doesn't reach the board limit, checks if there are words behind the
	// newly placed word
	while (0 < _newMove._column) {
		if (' ' != _board[_newMove._row][_newMove._column - 1]) {
			_newMove._word = _board[_newMove._row][_newMove._column - 1] + _newMove._word;
			wordBehind = _board[_newMove._row][_newMove._column - 1] + wordBehind;
			_newMove._column--;
		}
		else {
			break;
		}
	}

	// Same here, but for words ahead of the newly placed word
	while (end < (_width)) {
		if (' ' != _board[_newMove._row][end]) {
			_newMove._word += _board[_newMove._row][end];
			wordAhead += _board[_newMove._row][_newMove._column + 1];
			end++;
		}
		else {
			break;
		}
	}

	// Checks if the newly formed word isn't on dict, which means that the placement
	// formed a invalid word
	if (!isInDictionary(_newMove._word)) {
		return (false);
	}

	// If the word is valid, it's is mandatory to remove the previously formed words from
	// the board, because they belong to a bigger word now
	else {

		if (wordBehind.size()) {
			NewWord newW((char)(_newMove._row + 'A'), (char)(_newMove._column + 'a'), 'H', wordBehind);
			_placedWords.erase(newW);
		}

		if (wordAhead.size()) {
			NewWord newW((char)(_newMove._row + 'A'), (char)(endCopy + 'a'), 'H', wordAhead);
			_placedWords.erase(newW);
		}
	}

	// This part checks if it forms legal/illegal words vertically when placing a horizontal word
	// create a set of possible NewWords
	// create a set of words to destroy if needed
	// create a result set
	std::set<NewWord> NewWords;
	std::set<NewWord> toDestroy;
	std::set<NewWord> result;

	// Iterates from the beggining of the NewWord (the placed one, not the the possibly newly formed)
	// because the word behind and ahead were already previously checked
	for (size_t i = initial; i < initial + wordSize; i++) {

		// Checks if it is an intersection (if it is, that was previously checked, so skip)
		std::pair<int, int> coords(_newMove._row, i);
		if (_intersections.count(coords)) {
			continue;
		}

		// row - initial NewWord row
		// possibleNewWord - the possible new vertical word formed
		// passedRef - true if it has already reached the starting row
		// newWAhead - possible new vertical word starting in the initial row
		// newWBehind - possible new vertical word starting behind the initial row
		int row = _newMove._row;
		std::string possibleNewWord = "";
		bool passedRef = false;
		std::string str = " ";
		NewWord newWAhead(' ', ' ', ' ', str);
		NewWord newWBehind(' ', ' ', ' ', str);

		// Searches upwards for new vertical words until it reaches a space
		while (0 <= (row - 1) && ' ' != _board[row - 1][i]) {
			row--;
		}

		// endRow - variable to unwind to until it reaches the new endRow
		size_t endRow = row;

		// Unwinds until it finds a new space, meaning there are no more new possible word chars

		//    abcdefgh      --> adding "abapa", all new words formed are viable, so, when in letter
		//  A     ALLEZ     --> P from NewWord "abapa", it searches upwards until it reaches "allez"'s row,
		//  B  ABAPA        --> and unwinds until it reaches "epitome"'s row, creating the word "ape"
		//  C     EPITOME   --> so "endRow" starts on 'A' and ends on 'C', which is "ape"'s last
		//                  --> row. "row" saves it's initial row, to save the newly formed word
		//
		while ((endRow) < (_height) && (' ' != _board[endRow][i] || endRow == _newMove._row)) {
			// If it reaches the NewWord row, it has reached the reference and formed the
			// "wordBehind" already
			if (endRow == _newMove._row) {
				passedRef = true;
				wordBehind = possibleNewWord;
				newWBehind.setPlacement((char)(row + 'A'), (char)(i + 'a'), 'V', wordBehind);
				possibleNewWord += _newMove._word[i - _newMove._column];
			}
			else {
				if (passedRef) {
					wordAhead += _board[endRow][i];
				}
				possibleNewWord += _board[endRow][i];
			}
			endRow++;
		}
		// After this, it forms the "wordAhead"
		newWAhead.setPlacement((char)(_newMove._row + 'A'), (char)(i + 'a'), 'V', wordAhead);

		// If the "possibleNewWord" is the same as the char in that column of the word, then
		// there are no newly formed words
		if (possibleNewWord == std::string(1, _newMove._word[i - _newMove._column])) {
			continue;
		}

		// Else, if the "possibleNewWord" is in the dictionary, it is needed to delete possible
		// words that are destroyed (case of plurals, for example)
		//    a b c d e f
		//  A           A
		//  B T O W E R S -> adding the word "as" removes the word "tower", and creates "towers"
		//  C
		else if (isInDictionary(possibleNewWord) && !wasAlreadyUsed(possibleNewWord)) {
			if (_placedWords.count(newWBehind)) {
				toDestroy.insert(newWBehind);
			}
			if (_placedWords.count(newWAhead)) {
				toDestroy.insert(newWAhead);
			}
			// Adds the newly formed word to the "NewWords" set
			NewWord newW((char)(row + 'A'), (char)(i + 'a'), 'V', possibleNewWord);
			NewWords.insert(newW);
		}
		else {
			return (false);
		}
	}

	// The words are only added if all NewWords formed are viable
	std::set_difference(_placedWords.begin(), _placedWords.end(), toDestroy.begin(), toDestroy.end(), std::inserter(result, result.begin()));
	_placedWords.clear();
	_placedWords.insert(result.begin(), result.end());
	_placedWords.insert(NewWords.begin(), NewWords.end());
	return (true);
} // isInValidSurroundingsHorizontal

//==================================================

/**
 * Checks if a vertical word can be placed according to its surroundings
 * @return TRUE if it can, FALSE otherwise
 */
bool Board::isInValidSurroundingsVertical() {
	// initial - the NewWord's initial row
	// wordSize - the size of the NewWord
	// end - the final row of the NewWord
	size_t initial = _newMove._row;
	size_t wordSize = _newMove._word.size();
	size_t end = _newMove._row + wordSize;

	// Copy of end in case
	size_t endCopy = end;

	// Possible words behind and ahead of the NewWord
	std::string wordBehind = "", wordAhead = "";

	// While it doesn't reach the board limit, checks if there are words behind the
	// newly placed word
	while (0 < _newMove._row) {
		if (' ' != _board[_newMove._row - 1][_newMove._column]) {
			_newMove._word = _board[_newMove._row - 1][_newMove._column] + _newMove._word;
			wordBehind = _board[_newMove._row - 1][_newMove._column] + wordBehind;
			_newMove._row--;
		}
		else {
			break;
		}
	}

	// Same here, but for words ahead of the newly placed word
	while (end < (_height)) {
		if (' ' != _board[end][_newMove._column]) {
			_newMove._word += _board[end][_newMove._column];
			wordAhead += _board[_newMove._row + 1][_newMove._column];
			end++;
		}
		else {
			break;
		}
	}

	// Checks if the newly formed word isn't on dict, which means that the placement
	// formed a invalid word
	if (!isInDictionary(_newMove._word)) {
		return (false);
	}

	// If the word is valid, it's is mandatory to remove the previously formed words from
	// the board, because they belong to a bigger word now
	else {

		if (!wordBehind.empty()) {
			NewWord newW((char)(_newMove._row + 'A'), (char)(_newMove._column + 'a'), 'V', wordBehind);
			_placedWords.erase(newW);
		}
		if (!wordAhead.empty()) {
			NewWord newW((char)(endCopy + 'A'), (char)(_newMove._column + 'a'), 'V', wordAhead);
			_placedWords.erase(newW);
		}
	}

	// This part checks if it forms legal/illegal words horizontally when placing a vertical word
	// create a set of possible NewWords
	// create a set of words to destroy if needed
	// create a result set
	std::set<NewWord> newWords;
	std::set<NewWord> toDestroy;
	std::set<NewWord> result;

	for (size_t i = initial; i < initial + wordSize; i++) {

		// Checks if it is an intersection (if it is, that was previously checked, so skip)
		std::pair<int, int> coords(i, _newMove._column);
		if (_intersections.count(coords)) {
			continue;
		}

		// col - initial NewWord col
		// possibleNewWord - the possible new horizontal word formed
		// passedRef - true if it has already reached the starting col
		// newWAhead - possible new horizontal word starting in the initial column
		// newWBehind - possible new horizontal word starting behind the initial column
		int col = _newMove._column;
		std::string possibleNewWord = "";
		bool passedRef = false;
		std::string str = " ";
		NewWord newWAhead(' ', ' ', ' ', str);
		NewWord newWBehind(' ', ' ', ' ', str);

		while (0 <= (col - 1) && ' ' != _board[i][col - 1]) {
			col--;
		}

		// endCol - variable to unwind to until it reaches the new endCol
		size_t endCol = col;

		// Unwinds until it finds a new space, meaning there are no more new possible word chars
		//
		//    a b c d e f   --> adding "AR", all new words formed are viable, so, when in letter
		//  A     A         --> R from NewWord "AR", it searches leftwards until it reaches "ape"'s col,
		//  B   A R T       --> and unwinds until it reaches "tin"'s col, creating the word "art"
		//  C   P   I       --> so "endCol" starts on 'A' and ends on 'T', which is "art"'s last
		//  D   E   N       --> col. "col" saves it's initial col, to save the newly formed word
		//
		while ((endCol) < (_width) && (' ' != _board[i][endCol] || endCol == _newMove._column)) {
			// If it reaches the NewWord col, it has reached the reference and formed the
			// "wordBehind" already
			if (endCol == _newMove._column) {
				passedRef = true;
				wordBehind = possibleNewWord;
				newWBehind.setPlacement((char)(i + 'A'), (char)(col + 'a'), 'H', wordBehind);
				possibleNewWord += _newMove._word[i - _newMove._row];
			}
			else {
				if (passedRef) {
					wordAhead += _board[i][endCol];
				}
				possibleNewWord += _board[i][endCol];
			}
			endCol++;
		}
		// After this, it forms the "wordAhead"
		newWAhead.setPlacement((char)(i + 'A'), (char)(_newMove._column + 'a'), 'H', wordBehind);

		// If the "possibleNewWord" is the same as the char in that row of the word, then
		// there are no newly formed words
		if (possibleNewWord == std::string(1, _newMove._word[i - _newMove._row])) {
			continue;
		}
		// Else, if the "possibleNewWord" is in the dictionary, it is needed to delete possible
		// words that are destroyed (case of plurals, for example)
		//    a b c d e f
		//  A   A
		//  B   C           -> adding the word "ace" removes the word "vans", and creates "evans"
		//  C   E V A N S
		else if (isInDictionary(possibleNewWord) && !wasAlreadyUsed(possibleNewWord)) {
			if (_placedWords.count(newWBehind)) {
				toDestroy.insert(newWBehind);
			}
			if (_placedWords.count(newWAhead)) {
				toDestroy.insert(newWAhead);
			}
			//adds the newly formed word to the "NewWords" set
			NewWord newW((char)(i + 'A'), (char)(col + 'a'), 'H', possibleNewWord);
			newWords.insert(newW);
		}
		else {
			return (false);
		}
	}

	// The words are only added if all NewWords formed are viable
	std::set_difference(_placedWords.begin(), _placedWords.end(), toDestroy.begin(), toDestroy.end(), std::inserter(result, result.begin()));
	_placedWords.clear();
	_placedWords.insert(result.begin(), result.end());
	_placedWords.insert(newWords.begin(), newWords.end());
	return (true);
} // isInValidSurroundingsVertical

//==================================================

/**
 * Outputs the board status to a text file (its dimensions and containing words)
 * @param fileName - The name of the file to output
 */
void Board::outputBoardToFile(const std::string &fileName) const {

	std::ofstream fout(fileName);
	if (fout.fail()) {
		std::cerr << "Failed to create output file";
		exit(2);
	}
	fout << _height << " x " << _width << "\n";
	for (auto elem : _placedWords) {
		fout << (char)(elem.getRow() + 'A') << (char)(elem.getColumn() + 'a') << " " << elem.getOrientation() << " "
			<< elem.getWord() << "\n";
	}
	fout << "\n";
	showBoard(fout);

	fout.close();
} // outputBoardToFile

//==================================================

/**
 * Outputs the board to a defined ostream
 * @param out - The stream which the board is to be output
 */
void Board::showBoard(std::ostream &out) const {
	out << "  ";
	for (size_t i = 'a'; i < _width + 'a'; i++) {
		out << "  " << (char)i;
	}
	out << "\n";
	for (size_t i = 0; i < _height; i++) {
		out << " " << (char)(i + 'A') << " ";
		if (&out == &std::cout) {
			std::cout << BG_BLACK;
		}
		for (size_t j = 0; j < _width; j++) {
			out << " " << _board[i][j] << " ";
		}
		if (&out == &std::cout) {
			std::cout << RESET;
		}
		out << "\n";
	}
} // showBoard

//==================================================

/**
 * Updates the display of the board (important when removing words)
 */
void Board::updateBoard() {
	_board = std::vector<std::vector<char> >(_height, std::vector<char>(_width, ' '));

	for (const auto &elem : _placedWords) {
		std::istringstream inp(elem._word);

		// Letter by letter, every word is put in board according to its characteristics
		char letter;
		int wCol = elem._column;
		int wRow = elem._row;

		for (size_t j = 0; j < elem._word.size(); j++) {
			inp >> letter;
			_board[wRow][wCol] = letter;

			if ('H' == elem._orientation) {
				wCol++;
			}
			else {
				wRow++;
			}
		}
	}
} // updateBoard

//==================================================

/**
 * Shows the progress of the random build
 * @param percentage - The status of evolution of the random build
 */
void Board::progressBar(double percentage) const {
	std::cout << "\r";
	int val = (int)(percentage * 100);
	int lpad = (int)(percentage * PBWIDTH);
	int rpad = PBWIDTH - lpad;
	printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
} // progressBar
