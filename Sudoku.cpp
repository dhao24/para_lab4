// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>

#include "Solver.h"

int main()
{
	// Minimal Soduku tables with 17 elements: http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
	Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
	std::cout << "Problem:" << std::endl << std::endl;
	solver.print(std::cout);
	std::cout << std::endl << "-----------------------------------------" << std::endl;
	std::cout << "Solution:" << std::endl << std::endl;;
	solver.solveBackTrack();
	solver.print(std::cout);
    return 0;
}

