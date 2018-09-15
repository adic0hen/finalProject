#include <stdlib.h>
#include <stdio.h>
#include "gurobi_c.h"
#include <string.h>
#include "InitAndTerminateModule.h"
#include "InGameCommands.h"
#include "GameDataStructs.h"

/*The Solver module contains the functions that solve the board and also the ones that are board solving related.
This module calls functions from the gurobi optimization library, and also manages the memory allocated during
the proccess of optimizing the board. The heart of generate, hint and validate functions is found here.*/

/*Declaring outer variables*/
/*This variable is used in the gurobi optimization process, the "gurobi board"*/
int** board; 

/*The results structure, will contain all the wanted info extracted from the gurobi optimization*/
typedef struct results {
	int** solBoard;
	int optimstatus;
	double objval;
}RESULTS;
RESULTS res;

/* Declaration of functions*/

int solveMain(int isGenerate);
void solve();
void initBoardSolver();
void printBoardSolver(int** board);
int** copySol(double* sol);
int** setRandom(int** board, int x);
void quit(int error, GRBenv *env);
int** allocateMemForBoardPTR();
void freeSolver();
int checkValidityGenerate(int** board, int row, int col, int num);
int checkBlockValidityGenerate(int** board, int row, int col, int num);
void copySolvedBoardToMainBoard();
void copyMainBoardToGourobiBoard();
void freeVars(int* ind, double* val, double* lb, char* vtype, double* sol);
int** deleteExcept(int** board, int y);
int hintSolve(int row, int coloumn);
int generateSolve(int x, int y);
int validateSolve();

/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/

int solveMain(int isGenerate) {
	if (!isGenerate) {
		copyMainBoardToGourobiBoard();
	}
	solve();
	
	return 1;
}

/*The gurobi solving function*/

void solve() {
	/*declaring variables*/
	GRBenv *env;
	GRBmodel *model;
	int n;
	int i;
	int j;
	int iBlocks;
	int jBlocks;
	int counter;
	int *ind; 
	double *lowerBounds;
	double *varValues; 
	char *varType; 
	int optimstatus;
	double objval;
	int error;
	double *sol;

	/*defining variables*/
	res.objval = 0.0;
	res.optimstatus = 0;
	res.solBoard = NULL;
	ind = malloc(boardSize * sizeof(int));
	varValues = malloc(boardSize * sizeof(double));
	lowerBounds = malloc(boardSize * boardSize * boardSize * sizeof(double));
	varType = malloc(boardSize * boardSize * boardSize * sizeof(char));
	sol = malloc(boardSize * boardSize*boardSize * sizeof(double));


	if (ind == NULL || varValues == NULL || lowerBounds == NULL || varType == NULL || sol == NULL) {
		memoryError();
		return;
	}

	error = 0;
	model = NULL;
	env = NULL;
	
	

	
	

	/* Defining the names of the variables and giving lower bounds*/
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (n = 0; n < boardSize; n++) {
				if (board[i][j] == n) {
					
					lowerBounds[i*boardSize*boardSize + j * boardSize + n] = 1;
				}
				else {
					lowerBounds[i*boardSize*boardSize + j * boardSize + n] = 0;
				}
			
				varType[i*boardSize*boardSize + j * boardSize + n] = GRB_BINARY;

			}
		}
	}

	/* loading new enviroment*/
	error = GRBloadenv(&env, NULL);
	if (error) {
		quit(error, env);
	}

	/*command that disables log prints when optimizing*/
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0);
	if (error) {
		printf("ERROR IN intParam: %d", error);
		quit(error, env);
	}
	

	/* creating new model*/
	error = GRBnewmodel(env, &model, "sudoku", boardSize*boardSize*boardSize, NULL, lowerBounds, NULL, varType, NULL);
	if (error) {
		quit(error, env);
	}


	/* adding constraints to each cell (need to have 1 value)*/

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (n = 0; n < boardSize; n++) {
				ind[n] = i * boardSize*boardSize + j * boardSize + n;
				varValues[n] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, varValues, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}

	/* adding constraint to coloumns- each value has to appear once in each coloumn*/
	for (n = 0; n < boardSize; n++) {
		for (i = 0; i < boardSize; i++) {
			for (j = 0; j < boardSize; j++) {
				ind[j] = i * boardSize*boardSize + j * boardSize + n;
				varValues[j] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, varValues, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}



	/* adding constraints for the rows - each value has to appear once in each row*/
	for (n = 0; n < boardSize; n++) {
		for (j = 0; j < boardSize; j++) {
			for (i = 0; i < boardSize; i++) {
				ind[i] = i * boardSize*boardSize + j * boardSize + n;
				varValues[i] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, varValues, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}




	for (n = 0; n < boardSize; n++) {
		for (iBlocks = 0; iBlocks < blockWidth; iBlocks++) {
			for (jBlocks = 0; jBlocks < blockHeight; jBlocks++) {
				counter = 0;
				for (i = iBlocks * blockHeight; i < (iBlocks + 1)*blockHeight; i++) {
					for (j = jBlocks * blockWidth; j < (jBlocks + 1)*blockWidth; j++) {
						ind[counter] = i * boardSize*boardSize + j * boardSize + n;
						varValues[counter] = 1.0;
						counter++;
					}
				}

				error = GRBaddconstr(model, boardSize, ind, varValues, GRB_EQUAL, 1.0, NULL);
				if (error) {
					quit(error, env);
				}
			}
		}
	}

	error = GRBoptimize(model);
	if (error) {
		quit(error, env);
	}
	
	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		quit(error, env);
	}

	if (optimstatus == GRB_OPTIMAL) {
		error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
		if (error) {
			quit(error, env);
		}

		error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize*boardSize*boardSize, sol);
		if (error) {
			quit(error, env);
		}

		/*assigning values to results struct*/
		res.objval = objval;
		res.optimstatus = optimstatus;
		res.solBoard = copySol(sol);
		
	}


	/*Free memory used for the optimization process*/
	GRBfreemodel(model);
	GRBfreeenv(env);
	freeVars(ind, varValues, lowerBounds, varType, sol);
}


/*Assisting Subfunctions*/

void quit(int error, GRBenv *env) {

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
}


void freeVars(int* ind, double* val, double* lb, char* vtype, double* sol) {
	free(ind);
	free(val);
	free(lb);
	free(vtype);
	free(sol);
}


int** copySol(double* sol) {
	int i;
	int j;
	int v;
	int** solBoard;
	solBoard = allocateMemForBoardPTR();
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				if (sol[i*boardSize* boardSize + j * boardSize + v] == 1) {
					solBoard[i][j] = v + 1;
				}
			}
		}
	}
	return solBoard;
}


int** allocateMemForBoardPTR() {
	int i;
	void* tempPTR;
	int** allocatedMemAddr;

	tempPTR = (malloc((sizeof(int*)) * boardSize));
	if (tempPTR == NULL) {
		memoryError();
		return 0;
	}
	else {
		allocatedMemAddr = (int**)tempPTR;
	}

	for (i = 0; i < boardSize; i++) {
		tempPTR = malloc(sizeof(int)*boardSize);
		if (tempPTR == NULL) {
			memoryError();
			return 0;
		}
		allocatedMemAddr[i] = (int*)tempPTR;
	}
	return allocatedMemAddr;
}

void copyMainBoardToGourobiBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[i][j].currentCellvalue == -1) {
				board[i][j] = -1;
			}
			else {
				board[i][j] = mainGameBoard[i][j].currentCellvalue - 1;
			}
		}
	}
}


void copySolvedBoardToMainBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			mainGameBoard[i][j].currentCellvalue = res.solBoard[i][j];
			mainGameBoard[i][j].isErroneus = 0;
			mainGameBoard[i][j].isFixed = 0;
		}
	}
}


int** setRandom(int** board,int x) {
	int row;
	int col;
	int num;
	int cnt;
	int* options;
	int index;
	int isSet;
	int k;
	k = 0;
	while(k < x){
		row = rand() % boardSize;
		col = rand() % boardSize;
		isSet = 0;
		if (board[row][col] == -1) {
			options = malloc(boardSize * sizeof(int));
			for (index = 0; index < boardSize; index++) {
				options[index] = 1;
			}
			for (cnt = 0; cnt < boardSize; cnt++) {
				do {
					num = rand() % boardSize + 1;
				} while (options[num - 1] == 0); /*Finding a number that we havent tried yet*/
				if (checkValidityGenerate(board, row, col, num)) {
					board[row][col] = num;
					k++;
					isSet = 1;
					break;
				}
				else {
					options[num - 1] = 0;
				}
			}
			free(options);
			if (!isSet) {
				return NULL;
			}
		}
	}
	return board;
}


int checkValidityGenerate(int** board, int row, int col, int num) {
	int i;
	if (!checkBlockValidityGenerate(board, row, col, num)) {
		return 0;
	}
	for (i = 0; i < boardSize; i++) {
		if (board[row][i] == num) {
			return 0;
		}
	}

	for (i = 0; i < boardSize; i++) {
		if (board[i][col] == num) {
			return 0;
		}
	}
	return 1;
}


int checkBlockValidityGenerate(int** board, int row, int col, int num) {
	int i;
	int j;
	int rowBlockStart;
	int coloumnBlockStart;

	rowBlockStart = row / blockHeight;
	coloumnBlockStart = col / blockWidth;

	for (i = (rowBlockStart*blockHeight); i < (rowBlockStart + 1)*blockHeight; i++) {
		for (j = (coloumnBlockStart*blockWidth); j < (coloumnBlockStart + 1)*blockWidth; j++) {
			if (board[i][j] == num) {
				return 0; 
			}
		}
	}
	return 1;

}


int** deleteExcept(int** board, int y) {
	
	int row;
	int col;
	int toRemove;
	int k;
	toRemove = boardSize * boardSize - y;
	k = 0;
	while (k < toRemove) {
		row = rand() % boardSize;
		col = rand() % boardSize;
		if (board[row][col] != -1) {
			board[row][col] = -1;
			k++;
		}
	}
	return board;
}


void initBoardSolver() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			board[i][j] = -1;
		}
	}
}


void printBoardSolver(int** board) {
	int i;
	int j;
	int num;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			num = board[i][j];
			if (num == -1) {
				printf("   ");
			}
			else {
				printf(" %2d ", num);
			}
		}
		printf("\n");
	}
}

/*Solver-based functions*/


int hintSolve(int row, int coloumn) { /*returns the hint value if board is solvable, 0 otherwise*/
	int num;
	board = allocateMemForBoardPTR();
	solveMain(0);
	if (res.optimstatus == GRB_OPTIMAL) {
		num = res.solBoard[row][coloumn];
		freeSolver();
		return num;
	}
	else {
		printf("Error: board is unsolvable\n");
		freeSolver();
		return 0;
	}
}


int generateSolve(int x, int y) { /*x is the cells to fill, y is the cells to keep*/
	int b;
	b = 0; /*b will contain boolean value: weather the board was successfully solved or not*/
	board = allocateMemForBoardPTR();
	initBoardSolver();
	if (setRandom(board, x) != NULL) {
			solveMain(1);
		if (res.optimstatus == GRB_OPTIMAL) {
			deleteExcept(res.solBoard, y);
			copySolvedBoardToMainBoard(); /*The board is not "solved" but it's the one we want to use*/
			/* Need to add update to Undo-Redo List (can be in the middle of the game, while URList is not empty) */
			b = 1;
			
		}
	}
	freeSolver();
	return b;
}


int validateSolve() {
	board = allocateMemForBoardPTR();
	solveMain(0);
	if (res.optimstatus == GRB_OPTIMAL) {
		freeSolver();
		return 1;
	}
	else {
		freeSolver();
		return 0;
	}
}

void freeSolver() {
	freeMat(board);
	freeMat(res.solBoard);
}










