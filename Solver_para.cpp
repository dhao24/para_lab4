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

/* return the number of zeros left in the table
	(x,y) get the location of the first remaining zero of the table
	*/
int Solver::getNumOfZero(int* x, int* y){
	int num=0;
	for (int i = 0; i < sudoku_N; i++)
	{
		for (int j = 0; j < sudoku_N; j++)
		{
			if (data[i][j]==0)
			{
				if (num==0)
				{
					*x=i;
					*y=j;
				}
				
				num+=1;
			}
		}
	}
	return num;
}

/*this function used to check if the current table is valid*/ 
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

bool Solver::solveBackTrack(int* sum, char** localdata)
{
	// Are we there yet?
	if (isSolved())
	{
		this->addToResult(localdata, sum);
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
						if (tmpSolver.solveBackTrack(sum,localdata))
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

/* this function will find all valid tables of the next zero element
return true if there is a next zero
pdata: store the valid tables
totalNum: return the number of valid tables, always no bigger than 9 if 9*9 table
*/
bool Solver::findNextValid(char* ndata, int* totalNum){
	int x=0,y=0,num=0;
	if(getNumOfZero(&x,&y)){
		for (int val = 1; val <= sudoku_N; val++)
		{
			if (isAllowed(val,y,x))
			{
				getAllData(ndata+num*sudoku_N*sudoku_N);
				*(ndata+num*sudoku_N*sudoku_N+x*sudoku_N+y)=val+'0';
				num+=1;
			}
		}
	}else{
		return false;
	}

	*totalNum=num;
	return true;
}

void Solver::set(char val, int x, int y)
{
	data[y][x] = val;
}

void Solver::getAllData(char* pdata)
{
	for (int i = 0; i < sudoku_N*sudoku_N; i++)
	{
		int x = i % sudoku_N;
		int y = i / sudoku_N;
		*(pdata+i)=data[y][x]+'0';
	}
}

bool Solver::addToResult(char** data, int* n){
	addMemoryForSolutions(data,*n);
	this->getAllData(*data+(*n)*sudoku_N*sudoku_N);
	*n+=1;
	return true;
}

/* from the pdata status find the next valid
return false if no valid elements can be found
pdata: previous array of tables
pNum: number of previous tables
ndata: next array of tables
pNum: number of next tables
*/
bool findNextTables(char* pdata, int pNum, char* ndata, int* nNum){
	int total_num=0;
	bool rtflag=false;
	for (int i = 0; i < pNum; i++)
	{
		int num=0;
		Solver solver(pdata+i*sudoku_N*sudoku_N);
		if (solver.findNextValid(ndata+total_num*sudoku_N*sudoku_N, &num)){
			total_num+=num;
			rtflag=true;
		}
	}
	*nNum=total_num;
	return rtflag;
}

bool addMemoryForSolutions(char** data, int n){
	*data=(char*)realloc(*data,(n+1)*sudoku_N*sudoku_N*sizeof(char));
	return true;
}