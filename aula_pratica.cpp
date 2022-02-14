#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <random>       
#include <chrono>      
#include <set>
#include <iterator>

using namespace std;
unsigned seed = chrono::system_clock::now().time_since_epoch().count(); //random seed
default_random_engine e(seed); // defining the seed to generate random numbers later on

//player struct to save features of each player and have easy acess to get their values and manipulate them
struct playerStruct {
	string name; //player name - only one word
	int number; //number associated (ex: player 1)
	int points; //points of the player
	int rank; //relative position given by the number of points
};
//---------------------------------------------------------------------------------

//board struct to save features of each element of the board and have easy acess to get their values and manipulate them
struct boardElement {
	char letter; //letter of the element
	int color; //color of the letter of the element
};
//---------------------------------------------------------------------------------

//bag struct to save all letters available in the game and have easy acess to get their values and delete them as they are passed to the rack
struct bagStruct {
	vector <char> letter; //letters in the bag 
	vector <int> letterQuantity; //quantity available of each letter 
};
//---------------------------------------------------------------------------------

//The dictionary class contains all words considered valid and verifies if a word is in it
class Dictionary {

public:
	Dictionary(const string wordsFileName); //all words are saved in a set of strings
	bool isWordInDictionary(string& word); //checks if the word is in the dictionary

private:
	set <string> dictionaryWords; //contains all valid words
};
//---------------------------------------------------------------------------------

//The constructor acesses an external file with all the possible words and saves them in a set of strings
Dictionary::Dictionary(const string wordsFileName) {

	string word;
	ifstream wordFile;
	wordFile.open(wordsFileName); //open the external file

	//testing the file opening	
	if (!wordFile) {
		cout << "Error opening dictionary file" << endl;
		exit(0);
	}

	// fill the set of strings with the words in upper case
	while (wordFile >> word) {
		transform(word.begin(), word.end(), word.begin(), toupper); //now i push the upper case string
		dictionaryWords.insert(word);
	}
	wordFile.close(); //close the extern file after the data being saved
}
//---------------------------------------------------------------------------------

//Function that receives a word and checks if it is valid (present in the chosen dictionary)
bool Dictionary::isWordInDictionary(string& word) {

	//pass the word to upper case to compare with the words in the dictionary and see if there is an equal one
	transform(word.begin(), word.end(), word.begin(), toupper);
	if (find(dictionaryWords.begin(), dictionaryWords.end(), word) != dictionaryWords.end()) { //word is in the pack
		return true;
	}
	else {
		cout << "The word: " << word << " doesn't belong to the dictionary" << endl;
		return false;
	}
}
//---------------------------------------------------------------------------------

// The board class is responsible for setting and displaying the board
class Board {

public:
	Board(int numRow, int numCol); //resizes the board
	vector<vector<boardElement>> getBoard();
	void addBoard(string& word, char& row_char, char& col_char, char& dir, int& playerColor); //adds the words to the board previously developed
	void displayBoard(); //displays the board

private:
	vector<vector<boardElement>> board;
	const char defaultChar = ' '; //default char of the elements of the board
	string colorWord(char& word, const int player); //sets the color of the words to the associated to the player whose word belongs
};
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//The constructor receives the number o rows and columns defined and resizes its structure 
Board::Board(int numRow, int numCol) {

	//resizing the number of elements of the board
	board.resize(numRow);
	for (int i = 0; i < numRow; i++) board.at(i).resize(numCol);

	// defining the elements of the board element with a default char (' ') and the color associated to when the space is empty
	for (int i = 0; i < board.size(); i++) {
		for (int j = 0; j < board.at(i).size(); j++) {
			board[i][j].letter = defaultChar; //set to the default char (' ') 
			board[i][j].color = 0; //set the player color to 0 as default value 
		}
	}
}
//---------------------------------------------------------------------------------

vector<vector<boardElement>> Board::getBoard() {
	return board;
}
//---------------------------------------------------------------------------------

//Function that adds a word to the board. For that it receives the word, its position and direction and the color in which it should be presented
void Board::addBoard(string& word, char& row_char, char& col_char, char& dir, int& playerColor) {

	//converting the char with the letters to int numbers
	int col = toupper(col_char) - 'A';
	int row = toupper(row_char) - 'A';

	//passing the word received to upper case
	transform(word.begin(), word.end(), word.begin(), toupper);

	// looping the col or row, depending on the direction and replacing the default value (" ") with the letter that is already in upper case
	if (toupper(dir) == 'V') //vertical direction, loop the row
		for (int i = 0; i < word.size(); i++) { //i will do n step, being n the size of the word to add
			board[i + row][col].letter = word[i]; //change the char to the letter to add
			board[i + row][col].color = playerColor;  //change the color to the player's
		}

	if (toupper(dir) == 'H') //vertical direction, loop the col
		for (int i = 0; i < word.size(); i++) { //i will do n step, being n the size of the word to add
			board[row][i + col].letter = word[i]; //change the char to the letter to add
			board[row][i + col].color = playerColor;  //change the color to the player's
		}
}
//---------------------------------------------------------------------------------

//Function to display the board (already updated in the previous function)
void Board::displayBoard() {

	//defining the first row and column labels
	char row = 'A';	char col = 'a';
	cout << "   ";

	//the first row is the column labels
	for (int i = 0; i < board.at(0).size(); i++) {
		cout << col++ << " ";
	}
	cout << endl;

	//the next rows start with their label and have the size defined before and saved in the board struct
	for (int i = 0; i < board.size(); i++) {
		cout << " " << row++ << " "; //the first column is the row labels
		for (int j = 0; j < board.at(i).size(); j++) {
			//displaying the rest of the elements with the letters (or default char ' ') defined and the color of the player whose letter belongs
			cout << colorWord(board[i][j].letter, board[i][j].color);
		}
		cout << endl;
	}
	cout << endl;
}
//---------------------------------------------------------------------------------

//Function to set the color of the word presented depending on whose player they belong
string Board::colorWord(char& word, const int player) {

	// colors of the player 1 - 4 in order: red, blue, green and magenta	
	// vector encoding the color codes: default color, and each player's in order
	vector <string> colours = { "97m", "31m", "34m", "32m", "95m" };

	// vector to fill with the full code of each player's color
	vector <string> coloredWord = {};
	for (int i = 0; i < 5; i++) {
		coloredWord.push_back("\033[;47;" + colours.at(i) + word + " \033[0m");
	}
	switch (player) {
	case 0:
		return coloredWord[0];
	case 1:
		return coloredWord[1];
	case 2:
		return coloredWord[2];
	case 3:
		return coloredWord[3];
	case 4:
		return coloredWord[4];
	}
}
//-----------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//The bag class has all leters available to play and is updated as the letters are used
class Bag {

public:
	Bag(); // the bag is automaticaly created by the Game class
	vector <char> getBag();  
	void setBag(char letter); //fills the bag with the letters available
	void shuffleBag(); //shuffle the bag after it is created
	void updateBag(size_t rack_size); //update the bag as its letters are passed to the rack

private:
	vector <char> lettersInBag; // letters in the bag (all available)
};
//---------------------------------------------------------------------------------------

//The bag constructor is set as a default constructor as it is created along side the Game activity when the CONFIG file is readed. 
Bag::Bag() {
	this->lettersInBag.resize(0);
}
//---------------------------------------------------------------------------------

vector <char> Bag::getBag() {
	return this->lettersInBag;
}
//-------------------------------------------------------------------------------------------

//Function to automatically fill the bag by adding the letters and the respective quantity
void Bag::setBag(char letter) {
	lettersInBag.push_back(letter);
}
//------------------------------------------------------------------------------------------

//Function to shuffle the bag so that after the letters can be passed to the rack in order
void Bag::shuffleBag() {
	shuffle(lettersInBag.begin(), lettersInBag.end(), e); //e corresponds to the seed created in line 13
}
//---------------------------------------------------------------------------------

//Function to update the bag by removing the letters that were added to the rack
void Bag::updateBag(size_t rack_size) {
	lettersInBag.erase(lettersInBag.begin(), lettersInBag.begin() + rack_size);
}
//--------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//The rack class has the letters available to play in each move
class Rack {

public:
	Rack(size_t rackSize, Bag bag); //constructing the rack with letters from the bag
	vector<char> getRack();
	void displayRack(); // displaying the rack for a certain move
	void updateRack(Bag& bag, vector<char>& usedLetters); // updating the rack (removing the used letters and adding new ones)

private:
	vector<char>rack;
};
//---------------------------------------------------------------------------------

//The constructor fills the rack with the defined number of letters (which is received as a parameter) from the bag
Rack::Rack(size_t rack_size, Bag bag) {

	//since the bag is mixed up, my first rag will be the first n ( n = rack_size) chars of the bag
	for (int i = 0; i < rack_size; i++) {
		rack.push_back(bag.getBag()[i]); // adding the first n letters of the bag to the rack
	}
	bag.updateBag(rack_size); //deleting the first n letters from the bag
}
//---------------------------------------------------------------------------------

vector<char> Rack::getRack() {
	return this->rack;
}
//---------------------------------------------------------------------------------

//Function to display the rack to the user with the letter that he can use in his move
void Rack::displayRack() {

	//starting by sorting the rack alphabetically
	sort(rack.begin(), rack.end());

	//printing the rack
	cout << "RACK LETTERS -> | ";
	for (int l = 0; l < rack.size(); l++) {
		cout << rack.at(l) << " | ";
	}
	cout << endl;
}
//---------------------------------------------------------------------------------

//Function to update the rack by deleting the used letters (received as a paramenter organized in a vector) and adding new ones from the bag
void Rack::updateRack(Bag& bag, vector<char>& usedLetters) {

	for (int i = 0; i < usedLetters.size(); i++) { //go through the used letters vector
		for (int j = 0; j < rack.size(); j++) { //loop through the rack
			// if a letter used in a word was from the bag, that letter is replaced by a new one from the bag
			if (usedLetters.at(i) == rack.at(j)) {
				rack.at(j) = bag.getBag()[i];
				break;
			}
		}
	}
	//bag update: n letters were added to the rack from the bag because n letters were applied to the board
	//so, that n letters need to be deleted from the bag
	bag.updateBag(usedLetters.size());
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//The class player creates the players and updates their features
class Players {

public:
	Players(int numPlayers); //creating the players
	vector <playerStruct> getPlayers();
	void displayClassification(); // display the classification and points of each player
	void deletePlayer(int& playerNumber); // deletes the player if quits
	void updatePlayerPoints(int& playerNumber); // updates the points 
	void resetPlayerPoints(int& playerNumber); // reset the player's points before updating

private:
	vector <playerStruct> players;
	const unsigned int numMinPlayers = 2; // minimum of 2 players required
	const unsigned int numMaxPlayers = 4; // maximum of 4 players allowed
	int numPlayers;
	bool numPlayersValid(int& numPlayers); // checks if the number of players introduced is valid
};
//---------------------------------------------------------------------------------

//The constructor creates the players
Players::Players(int numPlayers) {

	//first we ask the user the number of players and check if the input is valid
	if (numPlayersValid(numPlayers)) {
		// the number of players has been introduced and validated
		for (int i = 1; i < numPlayers + 1; i++) { // create each player
			playerStruct p;
			cout << "Player " << i << " name: "; // only one word is accepted and can be composed by letters and/or numbers
			cin >> p.name;
			cin.ignore(1000, '\n'); // in the case that more than one word is written, the first is saved and the rest deleted from the buffer
			p.number = i;
			p.points = 0; //players start with 0 points
			this->players.push_back(p); //introduces the elements of the structure all for every player p in the vector called players.
		}
		cout << endl;
	}
}
//---------------------------------------------------------------------------------

//Function to receive and validate the number of players
bool Players::numPlayersValid(int& numPlayers) {

	cout << "How many players will be playing? ";
	cin >> numPlayers;
	//if the lecture was correct there is no need to do anything, the value of the int numPlayers was updated in the reading

	while (cin.fail() || numPlayers < numMinPlayers || numPlayers > numMaxPlayers) {
		// the lecture of an int from the buffer failed or is not in the range of players allowed
		if (cin.eof()) { // the file was correctly closed
			cin.clear();
			cerr << "Ended with CTRL+Z\n";
			exit(0); // if the user chose to close, we want to exit the game
		}
		else { // invalid input (not an int or not in the range allowed)
			cerr << "The number of players must be in the range" << numMinPlayers << "-" << numMaxPlayers << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "How many players will be playing? ";
			cin >> numPlayers;
		} //while the user doesn't input the correct message the program will continue to ask for a new input
	}
}
//---------------------------------------------------------------------------------

vector <playerStruct> Players::getPlayers() {
	return this->players;
}
//---------------------------------------------------------------------------------

//Function to rank and display the classification
void Players::displayClassification() {

	//creating a vector with a copy of the points of the players to sort it in descending order
	vector<int> points_copy;
	int ranking = 1;

	for (int p = 0; p < this->players.size(); p++) {
		points_copy.push_back(this->players[p].points); // fill the copy vector of points
		this->players[p].rank = 0; // set all ranks to zero so that they get a new rank only if the rank is 0 
		// this ensures that no players is ranked twice (important in case of tie)
	}
	sort(points_copy.begin(), points_copy.end()); // order the vector of points ascending

	for (int i = points_copy.size() - 1; i >= 0; i--) { //start with the max value
		for (int p = 0; p < this->players.size(); p++) {
			if (this->players[p].points == points_copy[i] && this->players[p].rank == 0) { // finds the player with that number of players and no ranking
				this->players[p].rank = ranking; // that player is the next in the rank
				cout << this->players[p].rank << " -> " << "Player " << this->players[p].number << " -> " << this->players[p].name
					<< " -> " << this->players[p].points << " points" << endl; //sends the information of the player rank to the screen.
				ranking++; // passes to the next position in the rank
			}
		}
	}
	cout << endl;
}
//---------------------------------------------------------------------------------

//Function to delete a players (in case he quits the game)
void Players::deletePlayer(int& playerNumber) {
	players.erase(players.begin() + playerNumber - 1);
}
//---------------------------------------------------------------------------------

//Function to update the points of the player
void Players::updatePlayerPoints(int& playerNumber) {
	players[playerNumber].points = players[playerNumber].points + 1;
}
//---------------------------------------------------------------------------------

//Function to reset the points of the player
void Players::resetPlayerPoints(int& playerNumber) {
	players[playerNumber].points = 0;
}
//---------------------------------------------------------------------------------
//---------------------------------------------------------------------------------

//The class game is responsible for setting the definitions and ensuring all rules of the game are being obeyed
class Game {

public:
	Game(string ConfigFile); //sets the main configurations
	Bag getInitialBag(); //creates the bag
	int getWinPoints() const; //gets the number of points needed to win the game
	string getDictonaryFile() const; //creates the dictionary file
	int getSumPassMax() const; //max number of times allowed to pass before updating the rack
	void initialDisplay() const; //displys the introduction message
	void readInput(int& playColor, Players& Players, string& word, char& play, char& row_char, char& col_char, char& dir); // receives the input
	bool isPlayValid(Dictionary& Dict, Rack& Rack, Bag& bag, string& word, Board& Board, char rowChar, char colChar, char dirChar, int& turnWordWritten, vector <char>& usedLetters);
	bool areRowAndColumnValid(Board& Board, char& rowChar, char& colChar); // checks if the column and row are valid
	bool isDirectionValid(char& dir); // checks the direction introduced
	void pointCounter(Players& player, Board& board); // counts each player points
	void play(Dictionary& Dict, Rack& Rack, Bag& bag, Players& Players, Board& board); //works as the main loop for each game round and turn 
	void allPlayersPassed(Players& Players, Rack& Rack, Bag& bag, int& passCounter, int& sumPassCounter, vector<char>& usedLetters); //checks if all players passed
	void saveWinnerPoints(Players& Players, int pointsOfThePlayerWinning); //save the points of the player that is winning at the moment
	int getRackSize() const;
	int getBoardNumberRows() const;
	int getBoardNumberCols() const;


private:
	int winPoints; //value read in the external file with the main rules
	string wordsFileName = "WORD_EN.txt"; //by default, it will go for this dictionary
	Bag bag;
	string word;
	const int sumPassMax = 3; //last case cenario the players can pass 3 times before the rack is updated
	const char defaultChar = ' ';
	int rackSize = 7;
	int boardNumberRows = 13;
	int boardNumberCols = 13;
};
//---------------------------------------------------------------------------------

//The constructor initializes the game by setting the main configurations
Game::Game(string ConfigFile) {

	ifstream config(ConfigFile);

	//testing the file opening
	if (!config) {
		cout << "Error opening the configurations file" << endl;
		exit(0); //if the file doesn't open the programm is closed
	}

	//reading the file and extracting the words
	else {
		int sizeSpace = 2;
		string s1, s2;
		int temp;
		string line;
		getline(config, line);
		size_t  pos = line.find(":");
		line.erase(0, pos + sizeSpace);//after finding the ":" and saving its position in the line the code erases a segment of the line strating from the begining to to positions after the:.
		this->winPoints = stoi(line); // converts the string line to an integrer and equals the variable winPoints to that value

		getline(config, line);
		size_t  pos2 = line.find(":");
		line.erase(0, pos2 + sizeSpace);//after finding the ":" and saving its position in the line the code erases a segment of the line strating from the begining to to positions after the:.
		this->wordsFileName = line; //the variable wordsFileName is changed to the same as the string line.

		getline(config, line);
		char letter;
		int qt;
		while (config >> letter >> qt) {
			for (int j = 0; j < qt; j++)this->bag.setBag(letter);
		}//fills the bag with the letters that came from the words in the file config
	}
}
//---------------------------------------------------------------------------------

Bag Game::getInitialBag() {
	return this->bag;
}
//---------------------------------------------------------------------------------

int Game::getRackSize() const {
	return this->rackSize;
}
//---------------------------------------------------------------------------------

int Game::getBoardNumberRows() const {
	return this-> boardNumberRows;
}
//---------------------------------------------------------------------------------

int Game::getBoardNumberCols() const {
	return this->boardNumberCols;
}
//---------------------------------------------------------------------------------

int Game::getWinPoints() const {
	return this->winPoints;
}
//---------------------------------------------------------------------------------

string Game::getDictonaryFile() const {
	return this->wordsFileName;
}
//---------------------------------------------------------------------------------

int Game::getSumPassMax() const {
	return this->sumPassMax;
}
//---------------------------------------------------------------------------------

//Function to display an inital message with the name of the game and a brief introduction
void Game::initialDisplay() const {
	// Initial message to display
	cout << "						Fundox	\n\n";
	cout << "The objective of the game is to construct words in the board reusing words previously constructed by other players.\n";
	cout << "Each letter used in a word correspond to 1 point.\n\n";
}
//---------------------------------------------------------------------------------

//Function that receives the move from the player
void Game::readInput(int& playColor, Players& Players, string& word, char& play, char& row_char, char& col_char, char& dir) {

	vector <playerStruct> players = Players.getPlayers();
	cout << endl << players[playColor - 1].name << ", what word do you want to play? ((x) to quit the game and (p) to pass the turn): ";
	cin >> play;
	//first the input is saved in a char so, in the case of being actually a char there is no need to do anything else

	//checking if it had more letters and for that reason we are reading a word
	if (cin.peek() != '\n') { // a word was introduced
		cin >> word;
		word = play + word; //concatenation of play (where the first letter was saved) with the rest of the word to have the complete word
		play = ' '; //the play char is changed to ' ' because the player didn't wan't to pass or quit
		cin.ignore(1000, '\n');
		cout << "Word position (row and column respectively): ";
		cin >> row_char >> col_char;//saves the coordenates indicated by the player to the first letter
		cin.ignore(1000, '\n');
		cout << "Word direction - horizontal (h) or vertical (v): ";
		cin >> dir; //saves the direction of the word
		cin.ignore(1000, '\n');
	}
	cout << endl;
}
//---------------------------------------------------------------------------------

//This method is where we verify if the word is a valid word for a certain position (col and row) and for a certain direction. We have already created function that verify if 
//the direction is valid, if the col and row are valids and if the word is from the dictionary, so here we check if the word doesn't get out off the board, doesn't occupied spaces that
//are already ocupied and also if the word can be formed using the available letters
bool Game::isPlayValid(Dictionary& Dict, Rack& Rack, Bag& bag, string& word, Board& board1, char rowChar, char colChar, char dirChar, int& turnWordWritten, vector <char>& usedLetter)
{
	if (!areRowAndColumnValid(board1, rowChar, colChar))return false;
	else if (!isDirectionValid(dirChar)) return false;
	else if (!Dict.isWordInDictionary(word)) return false;

	vector<vector<boardElement>>board = board1.getBoard();

	int col = toupper(colChar) - 'A'; //conveting char A, B.. to 1 ,2 so that i can iterate the board
	int row = toupper(rowChar) - 'A';

	if (toupper(dirChar) == 'V') {
		//check if the word doesn't get out of the board
		if (board.size() - row < word.size()) { 
			cout << "Word too big for that space" << endl;
			return false;
		}
		for (int i = 0; i < word.size(); i++) {
			//checking for occupied space
			if (board[i + row][col].letter == defaultChar) { //if the space is empty 
				usedLetter.push_back(toupper(word[i])); //if the space is empty the i char of the word is added to the lose letters, so that letter is to be moved from the rack later
			}
			else { // space not empty
				// if it has already a different letter, the word cannot be written
				if (board[i + row][col].letter != toupper(word[i])) {
					cout << "Cannot add word there, space occupied by other words" << endl;
					return false;
				}
			};
		}
	}
	else if (toupper(dirChar) == 'H') {

		//check if the word doesn't get out of the board
		if (board.size() - col < word.size()) {
			cout << "Word too big for that space" << endl;
			return false;
		}
		for (unsigned int i = 0; i < word.size(); i++) {
			//checking for occupied space 			
			if (board[row][col].letter == defaultChar) { //if the space is empty 
				usedLetter.push_back(toupper(word[i])); //if the space is empty the i char of the word is added to the lose letters, so that letter is to be moved from the rack later

			}
			else { // space not empty
				// if it has already a different letter, the word cannot be written
				if (board[row][col].letter != toupper(word[i])) {
					cout << "Cannot add word there, space occupied by other words" << endl;
					return false;
				}

			};
		}
	}
	vector <char> rack_copy = Rack.getRack();
	for (int j = 0; j < usedLetter.size(); j++) {
		auto it = find(rack_copy.begin(), rack_copy.end(), usedLetter[j]);
		if (it != rack_copy.end()) {
			rack_copy.erase(it);
		}
		else {
			cout << "The letters needed are not available in the rack\n\n";
			return false;
		}
	}

	// checking if a board letter was used or a paralel word was formed
	if (turnWordWritten >= 1 && (word.size() == usedLetter.size())) {
		cerr << "No letter already displayed in the board is being used." << endl;
		return false;
	}
	return true;
}

//Function that checks if the column and row introduced are valid
bool Game::areRowAndColumnValid(Board& Board, char& rowChar, char& colChar) {

	if(isdigit(rowChar) || isdigit(colChar)){
		cout << "Column and Rows can't be numeric values, check the board labels " << endl;
		return false;
	}
	vector<vector<boardElement>>board = Board.getBoard();
	bool colValid = false; bool rowValid = false;
	vector <char> colLabel;	vector <char> rowLabel;

	for (char i = 'A'; i < 'A' + board.size(); i++) { //char vector to get the label
		rowLabel.push_back(i); //this way we will get the first label [A,B,C,D...] and this we can check if the column and row introduces are any of this
	}

	for (char i = 'A'; i < 'A' + board[0].size(); i++) { //char vector to get the label
		colLabel.push_back(i); //this way we will get the second  [a,b,c,d...] and this we can check if the column and row introduces are any of this
	}

	for (int i = 0; i < rowLabel.size(); i++) {
		if (rowLabel.at(i) == toupper(rowChar)) {
			rowValid = true;
		}
	} //compares the elements from the vector rowLabel to the one introduced by the user.

	for (int i = 0; i < colLabel.size(); i++) {
		if (colLabel.at(i) == toupper(colChar)) {
			colValid = true;
		}
	} //compares the elements from the vector colLabel to the one introduced by the user.

	if (!colValid) {
		cout << "Column invalid, check the board labels" << endl;
		return false;
	}
	else if (!rowValid) {
		cout << "Row invalid, check the board labels" << endl;
		return false;
	}
	else return true;
}
//---------------------------------------------------------------------------------

//Function to verify the input in the direction
bool Game::isDirectionValid(char& dirChar) {
	if (isdigit(dirChar)) {
		cout << "Direction can only be vertical (v) or horizontal (h) - not numeric values " << endl;
		return false;
	}
	else if (toupper(dirChar) != 'H' && toupper(dirChar) != 'V') {
		cout << "Direction can only be vertical (v) or horizontal (h) " << endl;
		return false;
	}
	else return true;
}
//---------------------------------------------------------------------------------

//Function to control the players moves and the game
void Game::play(Dictionary& Dict, Rack& Rack, Bag& Bag, Players& Players, Board& Board) {

	vector<char> usedLetters; //vector to save the letters used from the rack 
	int turnWordWritten = 0; // counts the turns where words were added to the board to separate the first where there is no need to reuse board letters
	int sumPassCounter = 0; // times passed by all players
	int pointsOfThePlayerWinning = 0; // points of the winning player
	vector <int> playersQuit; //player that have quited in a round
	int passCounter = 0; // how many players have passed the turn sequentialy

	//Each player will play in turns until one reaches the points defined needed to win the game or until they all pass a defined maximum number of times
	while (pointsOfThePlayerWinning < winPoints && sumPassCounter < getSumPassMax()) {
		for (int p = 1; p < Players.getPlayers().size() + 1; p++) { //each players turn		
			bool validTurn = false;
			while (!validTurn) { //each turn
				Board.displayBoard(); //show the board to the player before he makes the play
				Rack.displayRack(); //show the rack to the player
				//defining or cleaning the values from the previous turn of the variables for the play column, row, direction, play itself, word and player color
				char col, row, dir;
				char play = ' ';
				string word = "";
				int playerColor = p;
				readInput(playerColor, Players, word, play, row, col, dir); //ask the player what does he wants to do or what word to input
				if (toupper(play) == 'X') { //The player quited the game 
					if (p == Players.getPlayers().size()) { cout << "here" << endl;  break; }
					validTurn = true; //it is a valid move
					playersQuit.push_back(p);
					passCounter = 0; //didn't pass, so the counter of times passed sequentialy is reseted
				}
				else if (toupper(play) == 'P') { //The player passed the round
					passCounter++; //add one to the counter of times passed sequentialy 
					validTurn = true; //it is a valid move
				}
				else { //The player will try to add the word	
					passCounter = 0; //didn't pass, so the counter of times passed sequentialy is reseted
					usedLetters.clear(); //clear the used letters from the previous turn to save the ones used in this turn
					//if the play is valid, the word will be added to the board
					if (isPlayValid(Dict, Rack, Bag, word, Board, row, col, dir, turnWordWritten, usedLetters)) {
						Board.addBoard(word, row, col, dir, p);
						Rack.updateRack(Bag, usedLetters); // update the rack
						pointCounter(Players, Board);//count the player points
						turnWordWritten++; //a word was added to the board in a turn
						validTurn = true; //it is a valid move
					}
					else { validTurn = false; } //a valid move was not introduced so the user will be asked to try to play again
				}
			}
			//check if all players have passed
			allPlayersPassed(Players, Rack, Bag, passCounter, sumPassCounter, usedLetters);
		}
		for (int i = 0; i < playersQuit.size(); i++) {
			Players.deletePlayer(playersQuit.at(i)); //delete the player
		}
		playersQuit.clear(); //so that the players that have quited deleted from the vector so that they dont get deleted twice
		//display the current classifications
		cout << "TURN CLASSIFICATIONS \n";
		Players.displayClassification();

		// save the winner points
		saveWinnerPoints(Players, pointsOfThePlayerWinning);

		//check if all players but one have quited
		if (Players.getPlayers().size() == 1) break;
	}
	//display the final classifications
	cout << "FINAL CLASSIFICATIONS \n";
	Players.displayClassification();
}
//---------------------------------------------------------------------------------

//Function to count the points of each player at the moment
void Game::pointCounter(Players& Player, Board& Board)
{
	vector<vector<boardElement>> board = Board.getBoard();
	for (int p = 0; p < Player.getPlayers().size(); p++) {
		Player.resetPlayerPoints(p);
	} //in each count it counts every point so i have to first set it back to 0

	for (int i = 0; i < board.size(); i++) { //start in one cuz i dont want to count the margins 
		for (int j = 0; j < board[i].size(); j++) {
			for (int p = 0; p < Player.getPlayers().size(); p++) {
				if (board[i][j].color == p + 1) {
					Player.updatePlayerPoints(p);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------------

//Function to check if all players have passed sequentialy
void Game::allPlayersPassed(Players& Players, Rack& Rack, Bag& bag, int& passCounter, int& sumPassCounter, vector<char>& usedLetters) {

	//if the all players passed the rack is updated
	if (passCounter == Players.getPlayers().size()) {
		cout << "All players have passed, the rack will be updated.\n" << endl;
		usedLetters = Rack.getRack();
		Rack.updateRack(bag, usedLetters); //update all rack
		sumPassCounter++;
		passCounter = 0;
		cout << "Times passed by all players: " << sumPassCounter << " \n\n";
	}
}
//---------------------------------------------------------------------------------

//Function to save the points of the player that is winning to see if he can be anounced winner
void Game::saveWinnerPoints(Players& Players, int pointsOfThePlayerWinning) {

	for (int p = 0; p < Players.getPlayers().size(); p++) {
		if (Players.getPlayers()[p].rank == 1)
			pointsOfThePlayerWinning = Players.getPlayers()[p].points;
	}
}
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
int main() {

	Game game("CONFIG.txt"); // sets the main configurations
	game.initialDisplay(); // displays introduction message
	string DictonaryFile = game.getDictonaryFile();	// creates the dictionary file
	Dictionary dictionary(DictonaryFile);
	Players players(0); // creates the players
	Bag bag = game.getInitialBag();	// creates the bag
	bag.shuffleBag(); // shuffles the bag
	Rack rack(game.getRackSize(), bag); // creates the rack
	bag.updateBag(rack.getRack().size()); // updates the bag after rack_size elements having been removed
	Board board(game.getBoardNumberRows() , game.getBoardNumberCols() ) ; // defines the board dimensions and creates it
	game.play(dictionary, rack, bag, players, board); //main playing method where each round and turn are processed 

	return 0;
}