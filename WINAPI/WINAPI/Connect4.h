#pragma once
#include <vector>
#include <fstream>
using namespace std;
class Connect4
{
public:
	Connect4();// Constructor
	void click(int);// Handles click 
	void reset();// Resets the game
	int dotPos[7][6];// 2D array representing the game board
	bool yellow;// yellow is true when it's yellow's turn
	struct Coord { // Struct for coordinates
		int row, col;
	};
	vector<Coord> fourInRow;// Vector containing the coordinates of the four in a row
	vector<Coord> inRow;// Vector containing posible moves
	bool vsAi = false;// true if playing against AI
	int r = 0, y = 0;// r = red wins, y = yellow wins
	void save();//save
	void load();//load
private:
	void computer();// AI
	bool isWinningMove(int col, int player);// Checks if a move is a winning move
	vector<Coord> checkDirection(int, int, int, int, int, int);// Checks a direction for four in a row
	vector<Coord> findInARow(int, int);// Finds a sequence of pieces in a row and returns their coordinates.
};
