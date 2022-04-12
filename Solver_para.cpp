#include "Solver.h"

Solver::Solver()
{
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			data[y][x] = 0;
		}
	}
}

Solver::Solver(const char * init)
{
	for (int i = 0; i < 81; ++i)
	{
		int x = i % 9;
		int y = i / 9;
		data[y][x] = init[i] - '0';
	}
}

Solver::Solver(const Solver * init)
{
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
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
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			s << (char)(data[y][x] + '0') << " ";
		}
		s << std::endl;
	}
}

bool Solver::isSolved()
{
	// Check whether every cell is filled in the table?
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
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
	for (int i = 0; i < 9; ++i)
	{
		if (data[y][i] == val) allowed = false;
		if (data[i][x] == val) allowed = false;
	}

	// Only one 'val' is allowed in a 3x3 cell
	int cellBaseX = 3 * (int)(x / 3);
	int cellBaseY = 3 * (int)(y / 3);
	for (int y = cellBaseY; y < cellBaseY + 3; ++y)
	{
		for (int x = cellBaseX; x < cellBaseX + 3; ++x)
		{
			if (data[y][x] == val) allowed = false;
		}
	}

	return allowed;
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
	for (int y = 0; y < 9; ++y)
	{
		for (int x = 0; x < 9; ++x)
		{
			// Is it empty?
			if (data[y][x] == 0)
			{
				// Find an appropriate 'val'
				int success_flag=0;
				for (int n = 1; n <= 9; ++n)
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

