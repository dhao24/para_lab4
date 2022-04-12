// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>

#include "Solver.h"

int main()
{
	// Minimal Soduku tables with 17 elements: http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
	Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
	// Sukudo case with 2 solutions
	// Solver solver("906070403000400200070023010500000100040208060003000005030700050007005000405010708");
	// Sukudo case with 3 solutions
	// Solver solver("906070403000400200070023010500000100040208060003000005030700050007005000405010700");
	// Sukudo case with NO solutions
	// Solver solver("906070403000400200070023010500000100040208060003000005030700050007005000405010728");
	std::cout << "Problem:" << std::endl << std::endl;
	solver.print(std::cout);
	std::cout << std::endl << "-----------------------------------------" << std::endl;
	std::cout << "Solution:" << std::endl << std::endl;;
	int sum=0;
	solver.solveBackTrack(&sum);
	// solver.print(std::cout);
	std::cout<<"Number of solutions are "<<sum<<std::endl;
    return 0;
}

