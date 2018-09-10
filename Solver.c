#include <stdlib.h>
#include <stdio.h>

#include "gurobi_c.h"
#include <string.h>
#include "InitAndTerminateModule.h"
#include "InGameCommands.h"
#include "GameDataStructs.h"

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

/*Function declarations*/
void solve();
void initBoardSolver();
void printBoardSolver(int** board);
int** transpose(int** board);
int** copySol(double* sol);
void quit(int error, GRBenv *env);
int** allocateMemForBoardPTR();
void freeSolver();
int checkValidityGenerate(int** board, int row, int col, int num);
int checkBlockValidityGenerate(int** board, int row, int col, int num);
void copySolvedBoardToMainBoard();
void copyMainBoardToGourobiBoard();
void freeVars(int* ind, double* val, double* lb, char* vtype, double* sol, char** names, char*namestorage);

int solveMain(int isGenerate) {
	int temp;
	int toTranspose;
	if (!isGenerate) {
		copyMainBoardToGourobiBoard();
	}
	if (blockHeight > blockWidth) {
		toTranspose = 1;
	}
	else {
		toTranspose = 0;
	}
	if (toTranspose) { /*Solver only works if blockHeight>=blockWidth*/
		board = transpose(board);
		temp = blockHeight;
		blockHeight = blockWidth;
		blockWidth = temp;
	}
	solve();
	if (toTranspose) {
		res.solBoard = transpose(res.solBoard);
		temp = blockHeight;
		blockHeight = blockWidth;
		blockWidth = temp;
	}
	return 1;
}

/*The gurobi solving function*/

void solve() {
	/*declaring variables*/
	GRBenv *env;
	GRBmodel *model;
	int v;
	int i;
	int j;
	int ig;
	int jg;
	int count;
	int *ind; /**/
	double *val; /**/
	double *lb; /**/
	char *vtype; /**/
	char **names; /**/
	char *namestorage; /**/
	char *cursor;
	int optimstatus;
	double objval;
	int error;
	double *sol;

	/*defining variables*/
	res.objval = 0.0;
	res.optimstatus = 0;
	res.solBoard = NULL;
	ind = malloc(boardSize * sizeof(int));
	if (!ind) {
		printf("error in ind");
	}
	val = malloc(boardSize * sizeof(double));
	if (!val) {
		printf("error in val");
	}
	lb = malloc(boardSize * boardSize * boardSize * sizeof(double));
	if (!lb) {
		printf("error in lb");
	}
	vtype = malloc(boardSize * boardSize * boardSize * sizeof(char));
	if (!vtype) {
		printf("error in vtype");
	}
	names = malloc(boardSize * boardSize * boardSize * sizeof(char*));
	if (!names) {
		printf("error in names");
	}
	namestorage = malloc(20 * boardSize * boardSize * boardSize * sizeof(char));
	if (!namestorage) {
		printf("error in namestorage");
	}
	sol = malloc(boardSize * boardSize*boardSize * sizeof(double));
	if (!sol) {
		printf("error in sol");
	}
	error = 0;
	model = NULL;
	env = NULL;
	
	

	/*
	error = GRBsetintparam(env, GRB_INT_PAR_LOGTOCONSOLE, 0); 
	if (error) {
		printf("ERROR IN intParam: %d", error);
		quit(error, env);
	}
	*/
	

	cursor = namestorage;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				if (board[i][j] == v) {
					

					lb[i*boardSize*boardSize + j * boardSize + v] = 1;
				}
				else {
					

					lb[i*boardSize*boardSize + j * boardSize + v] = 0;
				}
				

				vtype[i*boardSize*boardSize + j * boardSize + v] = GRB_BINARY;
				

				names[i*boardSize*boardSize + j * boardSize + v] = cursor;
				

				sprintf(names[i*boardSize*boardSize + j * boardSize + v], "x[%d,%d,%d]", i, j, v + 1);
				

				cursor += strlen(names[i*boardSize*boardSize + j * boardSize + v]) + 1;
			}
		}
	}

	
		error = GRBloadenv(&env, NULL);
		if (error) {
			printf("ERROR IN loadenv: %d", error);
			quit(error, env);
		}
	

	
	error = GRBnewmodel(env, &model, "sudoku", boardSize*boardSize*boardSize, NULL, lb, NULL, vtype, names);
	if (error) {
		printf("ERROR IN newmodel: %d", error);
		quit(error, env);
	}



	/*~~adding constraints~~*/

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				ind[v] = i * boardSize*boardSize + j * boardSize + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR IN addconstr: %d", error);
				quit(error, env);
			}
		}
	}


	for (v = 0; v < boardSize; v++) {
		for (j = 0; j < boardSize; j++) {
			for (i = 0; i < boardSize; i++) {
				ind[i] = i * boardSize*boardSize + j * boardSize + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				printf("ERROR IN addconstr1: %d", error);
				quit(error, env);
			}
		}
	}


	for (v = 0; v < boardSize; v++) {
		for (i = 0; i < boardSize; i++) {
			for (j = 0; j < boardSize; j++) {
				ind[j] = i * boardSize*boardSize + j * boardSize + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}

	for (v = 0; v < boardSize; v++) {
		for (ig = 0; ig < blockHeight; ig++) {
			for (jg = 0; jg < blockWidth; jg++) {
				count = 0;
				for (i = ig * blockHeight; i < (ig + 1)*blockHeight; i++) {
					for (j = jg * blockWidth; j < (jg + 1)*blockWidth; j++) {
						ind[count] = i * boardSize*boardSize + j * boardSize + v;
						val[count] = 1.0;
						count++;
					}
				}

				error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) {
					quit(error, env);
				}
			}
		}
	}

	error = GRBoptimize(model);
	if (error) {
		printf("ERROR IN OPTIMIZATION: %d", error);
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
	freeVars(ind, val, lb, vtype, sol,names,namestorage);
}


/*Assisting Subfunctions*/

void quit(int error, GRBenv *env) {

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
}

void freeVars(int* ind, double* val, double* lb, char* vtype, double* sol,char** names, char*namestorage) {
	free(ind);
	free(val);
	free(lb);
	free(vtype);
	free(sol);
	free(namestorage);
	free(names);
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

int** transpose(int** board) {
	int i;
	int j;
	int** transposed;
	transposed = allocateMemForBoardPTR();
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			transposed[i][j] = board[j][i];
		}
	}
	freeMat(board);/*The current board is freed because the board pointer is going to be set to the new transposed bord*/
	return transposed;
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
	if (setRandom(board, x) == NULL) {
		return b;
	}
	solveMain(1);
	if (res.optimstatus == GRB_OPTIMAL) {
		deleteExcept(res.solBoard, y);
		copySolvedBoardToMainBoard(); /*The board is not "solved" but it's the one we want to use*/
		/* Need to add update to Undo-Redo List (can be in the middle of the game, while URList is not empty) */
		b = 1;
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










