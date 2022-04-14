#pragma once

#include <iostream>
#include <vector>
#include <stdlib.h>

#define sudoku_N 9
#define sudoku_n 3
#define serial_level 3

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
	
	int getNumOfZero(int* x, int* y);

	void getAllData(char* pdata);

	bool findNextValid(char* ndata, int* nNum);
	void set(char val, int x, int y);

private:
	char data[sudoku_N][sudoku_N];
};

bool findNextTables(char* pdata, int pNum, char* ndata, int* nNum);
bool addValidSolutions(char* data, int* n);

