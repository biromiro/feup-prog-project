# feup_prog_project
1st Year, 2nd Semester PROG Project
T1G06, Elements:
 -> (201906272) Nuno Ricardo Teixeira da Costa 
 -> (201905427) Patricia do Carmo Nunes Oliveira

Project Distribution:
 -> Nuno Costa : 50%
 -> Patricia Oliveira : 50%

[Nuno - responsible for the program "BoardBuilder";
 Patricia - responsible for the program "ScrabbleJunior";
NOTE: Although we splat the developement of the project in two separate parts, during the 
execution of all the work we have collaborated, not only to test the programs, but to correct 
errors, standardize code styles and displays, compare solutions and data structures, thinking 
about the best strategies to face problems related to both programs, for example; We also 
reviewed each other's code and proposed improvements, discussed problem interpretations and 
exceptional cases that could happen. (With the support of a gitlab repository)]

-----------------------------------------------------------------------------------------------
Using Visual Studio's tool "Code analysis" no warnings were detected (in both programs).
-----------------------------------------------------------------------------------------------

1. ALL THE OBJETIVES OF THE PROJECT WERE ACCOMPLISHED:

(BOARD BUILDER)
 • Validation of all inputs and anticipation of errors;
 • Implementation of "ASCII Escapes Codes" to control console output;
 • Allows to select the board size, up to 20x20;
 • Allows to create the game board;
 • Allows to position a word in the board, in a user given position and orientation., if: it is 
   the given dictionary, it is not repeated, it is in bounds, it intersects correctly with other 
   words and doesn't create invalid new words;
 • Saves the contents of the board to a text file, according to the specifications, and prints
   the state of the board at the end of the file;
 • Implementation of the required and necessary class.

(SCRABBLE JUNIOR)
 • Successful implementation of all the rules of the game;
 • Validation of all inputs and anticipation of errors;
 • Implementation of "ASCII Escapes Codes" to control cursor movement
   (change cursor position; erase line; erase display and set graphics mode);
 • Distinguish the move of each player with colored letters;
 • Implementation of the required and necessary classes;
 • Separation between the definition and implementation of the methods in all classes.

-----------------------------------------------------------------------------------------------

2. OBJETIVES THAT WERE NOT ACHIEVED:
 • All objectives were achieved.

-----------------------------------------------------------------------------------------------

3. IMPROVEMENTS THAT WERE MADE:

(BOARD BUILDER)
 • Implementation of a random method to build the board(*);
 • Added progress bar for random builder (due to possible longer runtime);
 • Allows to remove a word (**);
 • Tackles situations where words in consecutive lines, which form new and valid vertical or
   horizontal words, are allowed if such condition is met.

(*) If the board is has less than 4 tiles, the random method is not available.
   
(**) Removing some words may create invalid ones (words that aren't in the dictionary). As such,
our approach did not add words, neither valid or invalid, after a removal. It must be at the 
user's criteria what 'removals' he finds correct, as they might create unsolvable situations in
Scrabble Junior (playing in a word that does not quite exist, for example).

Ex.: Consider this example:

      P	L A Y A             There are 5 words : PLAYA, EXTRA, CARS, YES and AX. By removing YES,
	    E X T R A  ->   for example, the letters Y, E and S cannot be removed, as it would
      C A R S               invalidate 3 words. It's the player's responsability to remove them.
                            Also, removing EXTRA would not remove it's 'E' nor 'X', but it also
			    would not create the word EX. It's the player's responsability to add
                            that word, if intended, to that position.

(SCRABBLE JUNIOR)
 • Saves the results of all matches in a file.

-----------------------------------------------------------------------------------------------

4. FUNCTIONING DETAILS OF THE "BOARD BUILDER"

(A) COMMANDS USED TO FACILITATE THE INTERACTION WITH THE USER
   • Created a "consoleCode" mode, where the changes made to the board by the user are made 
     using correct consoleCodes for each action (using !help displays all codes);
   • Allows the player to choose the dictionary to load the words from, as well as the file to
     which the board is to be saved.

(B) DESCRIPTION OF THE FLOW OF THE PROGRAM
  1- Ask the name of the dictionary file, as well as the board dimensions (20x20 max);
  2- Ask what build mode the user wants (random or manual):
     -> If manual:
         • Prompts to main menu and allows next move.
     -> If random:
         • Does a random build of the board according to its specifications;
         • Prompts to main menu and allows next move.
  3- While the console code input isn't "!finish":
     -> If "!rebuild":
         • Promps the user for the build mode (as shown above, same flow).
     -> If "!add":
         • Tries to add a new word to the board according to the given word, row, column and
          orientation.
     -> If "!remove":
         • Tries to remove a word from the board.
     -> If "!help":
         • Prompts the console Codes to the user.
     -> If "!finish":
         • Asks the user for the output file name;
         • Asks the user if he/she wants to build another board;
         • If so, finishes program, else, !rebuild. 

-----------------------------------------------------------------------------------------------

5. FUNCTIONING DETAILS OF THE "SCRABBLE JUNIOR GAME"

[The user is responsible for placing the file generated by the BoardBuilder in the correct 
 location of the Project "ScrabbleJunior"]
[The file generated by the BoardBuilder has 3 parts: 
    number_of_lines x number_of_columns
    list of all the words (eg.: Ak H WORD)
    ....
    2D view of the board]

(A) STRATEGIES USED TO VALIDATE THE MOVE OF EACH PLAYER
   • Letter must be in the hand of the player;
   • Coordinate must be part of the board;
   • The letter in that coordinate must match the one player wants to play;
   • The coordinate can't correspond to a tile that was already played;
   • Letter and coordinate must be the first or the next letter of any word of the board;
     (The player's second move can't be in the same word as the previous one
      [forbidden coordinates], except the first player of the game, even if there is an
      intersection between the two).

(B) DESCRIPTION OF HOW THE GAME WORKS:
  1- Ask the name of the board file and the number of players (2 to 4 allowed);
       [RULE 1: The board must have enough letters so that all players get at least one in 
        their hand.
	   • If this does not happen, the user has the opportunity to try again or leave.]
  2- Defines the players and inicialize their hand;
       [RULE 2: The tiles in the pool are evenly distributed among all players, being 7 the 
        maxium of letters that they can have in their hand.]
  3- Ask who will be the first player (randomly OR choose a specific one)
  4- Show the board on his starting position and all players' hand;
  5- Waits for the user to "say" that he is ready to start the game;
  6- Ask each player for their move (at most two if possible)
       [RULE 3: Only the first player of the game can play on the first and second letters of 
        the same word.]
       [RULE 4: If no move is possible, and player's hand is not empty
	-> Two possible outcomes:
	   • If pool is empty the player is force to pass;
	   • Otherwise, the player must replace at most two tiles with the pool.] 
  8- Player receives the number of chips correspondent to the number of words that were 
     finished with that move;
  9- Player receives the same amount of tiles he played, from the pool 
     (unless the pool is empty);
 10- After each move, detect if the game ended or has no ending and announce the winner(s);
       [RULE 4: Game ends when all the board is complete (pool and players' hand are empty)];
 11- Save the result of the game in an external file;
 12- Asks the player if he would like to play again.

[NOTE: Visual Studio required the use of the function "localtime_s" instead of "localtime" to 
 get the current time and date. But with other compilers the function "localtime_s" was not 
 recognized, saying that it should be "localtime". In order for Visual Studio to compile the 
 program with the function "localtime", it would be necessary to have the instruction 
 "#pragma warning(disable : 4996)". In order to avoid it, according to the system where it
 is being compiled in, we use the corresponding function.]
