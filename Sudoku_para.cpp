// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>

#include "Solver.h"
#include <mpi.h>

#define RootRank 0

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
    int size;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Minimal Soduku tables with 17 elements: http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
	Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
	if (my_rank==RootRank)
	{
		std::cout << "Problem:" << std::endl << std::endl;
		solver.print(std::cout);
		std::cout << std::endl << "-----------------------------------------" << std::endl;
	// }
	
	std::cout << "Solution:" << std::endl << std::endl;;
	int sum=0;
	solver.solveBackTrack(&sum);
	// solver.print(std::cout);
	std::cout<<"Number of solutions are "<<sum<<std::endl;
	}
	MPI_Finalize();
    return 0;
}

