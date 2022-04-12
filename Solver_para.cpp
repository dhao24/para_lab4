#include "Solver.h"

void clearBitmap(bool *map, int size) {
    for (int i = 0; i < size; i++) {
        map[i] = false;
    }
}

Solver::Solver()
{
	for (int y = 0; y < sudoku_N; ++y)
	{
		for (int x = 0; x < sudoku_N; ++x)
		{
			data[y][x] = 0;
		}
	}
}

Solver::Solver(const char * init)
{
	for (int i = 0; i < sudoku_N*sudoku_N; ++i)
	{
		int x = i % sudoku_N;
		int y = i / sudoku_N;
		data[y][x] = init[i] - '0';
	}
}

Solver::Solver(const Solver * init)
{
	for (int y = 0; y < sudoku_N; ++y)
	{
		for (int x = 0; x < sudoku_N; ++x)
		{
			data[y][x] = init->data[y][x];
		}
	}
}


Solver::~Solver()
{
}

void Solver::print(std::ostream & s)
{
	for (int y = 0; y < sudoku_N; ++y)
	{
		for (int x = 0; x < sudoku_N; ++x)
		{
			s << (char)(data[y][x] + '0') << " ";
		}
		s << std::endl;
	}
}

bool Solver::isSolved()
{
	// Check whether every cell is filled in the table?
	for (int y = 0; y < sudoku_N; ++y)
	{
		for (int x = 0; x < sudoku_N; ++x)
		{
			if (data[y][x] == 0) return false;
		}
	}
	return true;
}

bool Solver::isAllowed(char val, int x, int y)
{
	bool allowed = true;

	// Only one 'val' is allowed in same row and column
	for (int i = 0; i < sudoku_N; ++i)
	{
		if (data[y][i] == val) allowed = false;
		if (data[i][x] == val) allowed = false;
	}

	// Only one 'val' is allowed in a 3x3 cell
	int cellBaseX = sudoku_n * (int)(x / sudoku_n);
	int cellBaseY = sudoku_n * (int)(y / sudoku_n);
	for (int y = cellBaseY; y < cellBaseY + sudoku_n; ++y)
	{
		for (int x = cellBaseX; x < cellBaseX + sudoku_n; ++x)
		{
			if (data[y][x] == val) allowed = false;
		}
	}

	return allowed;
}

bool Solver::isValid()
{
	bool seen[sudoku_N];
    clearBitmap(seen, sudoku_N);

    // check if rows are valid
    for (int i = 0; i < sudoku_N; i++) {
        clearBitmap(seen, sudoku_N);

        for (int j = 0; j < sudoku_N; j++) {
            char val = data[i][j];

            if (val != 0) {
                if (seen[val - 1]) {
                    return false;
                } else {
                    seen[val - 1] = true;
                }
            }
        }
    }

    // check if columns are valid
    for (int j = 0; j < sudoku_N; j++) {
        clearBitmap(seen, sudoku_N);

        for (int i = 0; i < sudoku_N; i++) {
            char val = data[i][j];

            if (val != 0) {
                if (seen[val - 1]) {
                    return false;
                } else {
                    seen[val - 1] = true;
                }
            }
        }
    }

    // finally check if the sub-boards are valid
    for (int ridx = 0; ridx < sudoku_n; ridx++) {
        for (int cidx = 0; cidx < sudoku_n; cidx++) {
            clearBitmap(seen, sudoku_N);

            for (int i = 0; i < sudoku_n; i++) {
                for (int j = 0; j < sudoku_n; j++) {
                    char val = data[ridx * sudoku_n + i][cidx * sudoku_n + j];

                    if (val != 0) {
                        if (seen[val - 1]) {
                            return false;
                        } else {
                            seen[val-1] = true;
                        }
                    }
                }
            }
        }
    }

    // if we get here, then the board is valid
    return true;
}

bool Solver::solveBackTrack(int* sum)
{
	// Are we there yet?
	if (isSolved())
	{
		*sum+=1;
		std::cout<<"["<<*sum<<"]"<<std::endl;
		this->print(std::cout);
		return true;
	}

	// Find an empty cell
	for (int y = 0; y < sudoku_N; ++y)
	{
		for (int x = 0; x < sudoku_N; ++x)
		{
			// Is it empty?
			if (data[y][x] == 0)
			{
				// Find an appropriate 'val'
				int success_flag=0;
				for (int n = 1; n <= sudoku_N; ++n)
				{
					// Is 'val' allowed in this cell?
					if (isAllowed(n, x, y))
					{
						// Copy the table
						Solver tmpSolver(this);
						// Put 'val' into the cell
						tmpSolver.set(n, x, y);
						// Try to solve the new table
						if (tmpSolver.solveBackTrack(sum))
						{
							// Solution
							// *this = tmpSolver;
							success_flag+=1;
						}
					}
				}
				if (success_flag)
				{
					return true;
				}
				
			}
			// Cannot solve this table, back track to a previous state
			if (data[y][x] == 0) return false;
		}
	}

	return false;
}

void Solver::set(char val, int x, int y)
{
	data[y][x] = val;
}

