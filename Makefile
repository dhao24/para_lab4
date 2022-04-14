sudoku_para: Sudoku.o Solver_para.o
	mpiCC -o sudoku_para Sudoku.o Solver_para.o

Solver_para.o: Solver.h Solver_para.cpp
	g++ -std=c++11 -g -O0 -c -o Solver_para.o Solver_para.cpp

sudoku: Sudoku.o Solver.o
	g++ -o sudoku Sudoku.o Solver.o

Sudoku.o: Solver.h Sudoku_para.cpp
	mpiCC -std=c++11 -g -O0 -c -o Sudoku.o Sudoku_para.cpp

Solver.o: Solver.h Solver.cpp
	g++ -std=c++11 -g -O0 -c -o Solver.o Solver.cpp

clean:
	rm -f sudoku
	rm -f sudoku_para
	rm -f Sudoku.o
	rm -f Solver.o
	rm -f Solver_para.o


