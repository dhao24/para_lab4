#pragma once

#include <iostream>

#define sudoku_N 9
#define sudoku_n 3

/*force the bool value to zero
map: bool array
size: number of values from begin to be set
*/
void clearBitmap(bool *map, int size);

class Solver
{
public:
	Solver();
	Solver(const char* init);
	Solver(const Solver* init);

	~Solver();

	void print(std::ostream &s);

	bool isSolved();
	bool isAllowed(char val, int x, int y);
	bool solveBackTrack(int* sum);
	bool isValid();
	int solveBackTrack_all();
	int getNumOfZero();

	void set(char val, int x, int y);

private:
	char data[9][9];
};

