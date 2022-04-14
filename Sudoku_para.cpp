// Sudoku.cpp : Defines the entry point for the console application.

#include <iostream>
#include <cmath>

#include "Solver.h"
#include <mpi.h>

#define RootRank 0

void exScan_array(int* arr_out, int* arr_in, int n){
	arr_out[0]=arr_in[0];
	for (int i = 1; i < n; i++)
	{
		arr_out[i]=arr_out[i-1]+arr_in[i-1];
	}
}

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
    int size;
    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	// Minimal Soduku tables with 17 elements: http://staffhome.ecm.uwa.edu.au/~00013890/sudokumin.php
	Solver solver("000801000000000043500000000000070800020030000000000100600000075003400000000200600");
	
	const int sizedata=pow(sudoku_N,serial_level);
	char* preData=(char*)calloc(sudoku_N*sudoku_N*sizedata,sizeof(char));
	char* nextData=(char*)calloc(sudoku_N*sudoku_N*sizedata,sizeof(char));
	char* resultData=NULL;
	char* localResultData=NULL;
	solver.getAllData(preData);
	int pNum=1;
	int nNum=0;
	int resultNum=0;

	for (int i = 0; i < serial_level; i++)
	{
		if (i%2==0)
		{
			findNextTables(preData,pNum,nextData,&nNum);
		}else{
			findNextTables(nextData,nNum,preData,&pNum);
		}
	}

	if (serial_level%2==1)
	{
		resultData=nextData;
		resultNum=nNum;
	}else{
		resultData=preData;
		resultNum=pNum;
	}

	int *globe_num=(int*)calloc(size, sizeof(int));
	int *globe_offset=(int*)calloc(size, sizeof(int));
	char* gatherData=NULL;
	int global_sum=0;
	int local_sum=0;
	int total_sum=0;
	int index=my_rank;
	while (index<resultNum)
	{
		//todo
		Solver tempSolver(resultData+index*sudoku_N*sudoku_N);
		// tempSolver.solveBackTrack(&caseSum);
		tempSolver.solveBackTrack(&local_sum, &localResultData);

		index+=size;
	}

	MPI_Gather(&local_sum,1,MPI_INT,globe_num,1,MPI_INT,RootRank,MPI_COMM_WORLD);
	if (my_rank==RootRank)
	{
		exScan_array(globe_offset,globe_num,size);
		total_sum=globe_num[size-1]+globe_offset[size-1];
		gatherData=(char*)calloc(total_sum*sudoku_N*sudoku_N,sizeof(char));
		MPI_Gatherv(localResultData,local_sum*sudoku_N*sudoku_N,MPI_CHAR,gatherData,globe_num,globe_offset,MPI_CHAR,RootRank,MPI_COMM_WORLD);
	}else
	{
		MPI_Gatherv(localResultData,local_sum*sudoku_N*sudoku_N,MPI_CHAR,NULL,NULL,NULL,MPI_CHAR,RootRank,MPI_COMM_WORLD);
	}
	
	
	if (my_rank==RootRank)
	{
		std::cout << "Problem:" << std::endl << std::endl;
		solver.print(std::cout);
		std::cout << std::endl << "-----------------------------------------" << std::endl;
	
		std::cout << "Solution:" << std::endl << std::endl;;
		
		std::cout<<"Number of Tables: "<<resultNum<<std::endl;

		for (int i = 0; i < total_sum; i++)
		{
			Solver resultSolver(gatherData+i*sudoku_N*sudoku_N);
			std::cout<<"["<<i<<"]"<<std::endl;
			resultSolver.print(std::cout);
		}
	}
	// std::cout<<"thread:"<<my_rank<<", Number of Solutions: "<<local_sum<<std::endl;
	// for (int k = 0; k < local_sum; k++){
	// 	for (int i = k*sudoku_N*sudoku_N; i < k*sudoku_N*sudoku_N+sudoku_N; i++){
	// 		std::cout<<*(localResultData+i)<<" ";
	// 	}
	// 	std::cout<<std::endl;
	// }

	free(preData);
	free(nextData);
	free(localResultData);
	free(globe_num);
	free(globe_offset);
	free(gatherData);
	MPI_Finalize();
    return 0;
}

