#include "Connect4.h"
#include <string>

Connect4::Connect4()
{
	reset();
}

void Connect4::click(int pos)
{
	if (pos <= 6)
	{
		for (int y = 0; y < 6; y++)
		{
			if (dotPos[pos][y] == 0)
			{
				if (yellow)
				{
					dotPos[pos][y] = 2;
					fourInRow = findInARow(2, 4);
					yellow = !yellow;
					break;
				}
				else
				{
					dotPos[pos][y] = 1;
					fourInRow = findInARow(1, 4);
					yellow = !yellow;
					if (vsAi)
					{
						if (fourInRow.empty())
						{
							computer();
						}
					}
					break;
				}
			}
		}
	}
}

void Connect4::reset()
{
	fourInRow.clear();
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			dotPos[x][y] = 0;
		}
	}
	yellow = false;
}

void Connect4::computer()
{
	// bättre än julius
	for (int i = 0; i < 7; i++) {
		if (isWinningMove(i, 2)) {
			click(i);
			return;
		}
	}

	for (int i = 0; i < 7; i++) {
		if (isWinningMove(i, 1)) {
			click(i);
			return;
		}
	}
	inRow.clear();
	findInARow(2, 4);
	if (!inRow.empty())
	{
		click(inRow.front().row);
	}
	else if (dotPos[3][0] == 0)
	{
		click(3);
	}
	else
	{
		click(2);
	}
}

bool Connect4::isWinningMove(int row, int player)
{
	int col;
	for (col = 0; col < 6; col++) {
		if (dotPos[row][col] == 0) {
			break;
		}
	}

	if (col == 6) {
		return false;
	}
	dotPos[row][col] = player;
	bool isWin = !findInARow(player, 4).empty();
	dotPos[row][col] = 0;
	return isWin;
}

vector<Connect4::Coord> Connect4::checkDirection(int startRow, int startCol, int dirX, int dirY, int value, int rowCount)
{
	vector<Connect4::Coord> coords;
	for (int i = 0; i < rowCount; ++i) {
		int r = startRow + i * dirX;
		int c = startCol + i * dirY;
		if (r < 0 || r >= 7 || c < 0 || c >= 6) {
			return {};
		}
		if (dotPos[r][c] == 0 && (c == 0 || dotPos[r][c - 1] != 0))
		{
			inRow.push_back({ r,c });
		}
		if (dotPos[r][c] != value)
		{
			return {};
		}
		coords.push_back({ r,c });
	}
	return coords;
}

vector<Connect4::Coord> Connect4::findInARow(int value, int rowCount)
{
	const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
	const int dy[8] = { 1, 0, 1, -1, -1, 0, -1, 1 };

	for (int i = 0; i < 7; ++i) {
		for (int j = 0; j < 6; ++j) {
			if (dotPos[i][j] == value) {
				for (int d = 0; d < 8; ++d) {
					vector<Connect4::Coord> coords = checkDirection(i, j, dx[d], dy[d], value, rowCount);
					if (!coords.empty()) {
						return coords;
					}
				}
			}
		}
	}
	return {};
}

void Connect4::save()
{
ofstream file;
	file.open("save.txt");
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			file << dotPos[x][y];
		}
	}
	file << yellow;
	file << vsAi;
	file << " ";
	file << r;
	file <<" ";
	file << y;
	file.close();
}
void Connect4::load()
{
	ifstream file;
	file.open("save.txt");
	string line;
	getline(file, line);
	int i = 0;
	for (int x = 0; x < 7; x++)
	{
		for (int y = 0; y < 6; y++)
		{
			dotPos[x][y] = line[i] - '0';
			i++;
		}
	}
	yellow = line[i] - '0';
	i++;
	vsAi = line[i] - '0';
	i++;
	r = stoi(line.substr(line.find(" "), line.find_last_of(" ")));
	i++;
	y = stoi(line.substr(line.find_last_of(" "), line.size()));
	file.close();
}
