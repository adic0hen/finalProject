#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"
#include "gurobi_c.h"
#include <string.h>
#include "InGameCommands.h"
#include <time.h>;

/*Outer variable declarations*/
int boardSize;
int blockHeight;
int blockWidth;
int** board; /*is used in the gurobi optimization process, "gurobi board"*/


/*The results structure, will contain all the wanted info extracted from the gurobi optimization*/
typedef struct results {
	int** solBoard;
	int optimstatus;
	double objval;
}RESULTS;

/*Function declarations*/
void solve();

int** copySol(double* sol);
void quit(int error, GRBenv *env);
int** allocateMemForBoardPTR();
void test_MAIN();
int test_solverTest();
RESULTS res;



int main() {
	srand(time(NULL));
	printf("hello\n");
	boardSize = 9;
	blockHeight = 3;
	blockWidth = 3;
	test_MAIN();
	return 0;
}

int solveMain() {
	board = allocateMemForBoardPTR();
	test_initBoard();
	copyMainBoardToGourobiBoard();
	if (blockHeight > blockWidth) {
		transpose(board);
	}
	solve();
	if (blockHeight > blockWidth) {
		transpose(res.solBoard);
	}
	copySolvedBoardToMainBoard();
	free(board);
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
	ind = (int*)malloc(boardSize * sizeof(int));
	val = (double*)malloc(boardSize * sizeof(double));
	lb = (double*)malloc(boardSize * boardSize * boardSize * sizeof(double));
	vtype = (char*)malloc(boardSize * boardSize * boardSize * sizeof(char));
	names = (char**)malloc(boardSize * boardSize * boardSize * sizeof(char));
	namestorage = (char*)malloc(10 * boardSize * boardSize * boardSize * sizeof(char));
	error = 0;
	model = NULL;
	env = NULL;
	sol = (double*)malloc(boardSize * boardSize*boardSize * sizeof(double));



	/* Create an empty model */

	cursor = namestorage;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				if (board[i][j] == v)
					lb[i*boardSize*boardSize + j * boardSize + v] = 1;
				else
					lb[i*boardSize*boardSize + j * boardSize + v] = 0;
				vtype[i*boardSize*boardSize + j * boardSize + v] = GRB_BINARY;

				names[i*boardSize*boardSize + j * boardSize + v] = cursor;
				sprintf(names[i*boardSize*boardSize + j * boardSize + v], "x[%d,%d,%d]", i, j, v + 1);
				cursor += strlen(names[i*boardSize*boardSize + j * boardSize + v]) + 1;
			}
		}
	}

	/* Create environment */

	error = GRBloadenv(&env, "sudoku.log");
	if (error) {
		quit(error, env);
	}

	/* Create new model */

	error = GRBnewmodel(env, &model, "sudoku", boardSize*boardSize*boardSize, NULL, lb, NULL,
		vtype, names);
	if (error) {
		quit(error, env);
	}

	/*~~adding constraints~~*/

	/* Each cell gets a value */

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				ind[v] = i * boardSize*boardSize + j * boardSize + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}

	/* Each value must appear once in each row */

	for (v = 0; v < boardSize; v++) {
		for (j = 0; j < boardSize; j++) {
			for (i = 0; i < boardSize; i++) {
				ind[i] = i * boardSize*boardSize + j * boardSize + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, boardSize, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) {
				quit(error, env);
			}
		}
	}

	/* Each value must appear once in each column */

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

	/* Each value must appear once in each subgrid */

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


	/* Optimize model */

	error = GRBoptimize(model);
	if (error) {
		quit(error, env);
	}
	
	/* Write model to 'sudoku.lp' ~~~~~~~~~CAN BE DELETED~~~~~*/

	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		quit(error, env);
	}

	/* Capture solution information ~~~~~~~~~CAN BE DELETED~~~~~*/

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) {
		quit(error, env);
	}

	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) {
		quit(error, env);
	}

	error = GRBgetdblattrarray(model, GRB_DBL_ATTR_X, 0, boardSize*boardSize*boardSize, sol);
	if (error) {
		quit(error, env);
	}

	/*print test ~~~~~~~~~CAN BE DELETED~~~~~*/
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			for (v = 0; v < boardSize; v++) {
				if (sol[i*boardSize* boardSize + j * boardSize + v] == 1) {
					printf("%d ", v + 1);
				}
			}
			printf("  ");
		}
		printf("\n\n");
	}

	/*more printing ~~~~~~~~~CAN BE DELETED~~~~~*/
	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL)
		printf("Optimal objective: %.4e\n", objval);
	else if (optimstatus == GRB_INF_OR_UNBD)
		printf("Model is infeasible or unbounded\n");
	else
		printf("Optimization was stopped early\n");
	printf("\n");


	/*assigning values to results struct*/
	res.objval = objval;
	res.optimstatus = optimstatus;
	res.solBoard = copySol(sol);

	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

}


/*assisting subfunctions*/

void quit(int error, GRBenv *env) {

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
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
					solBoard[i][j] = v;
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
	int size;

	tempPTR = (malloc((sizeof(int*)) * size));
	if (tempPTR == NULL) {
		return 0;
	}
	else {
		allocatedMemAddr = (int**)tempPTR;
	}

	for (i = 0; i < size; i++) {
		tempPTR = malloc(sizeof(int)*size);
		if (tempPTR == NULL) {
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
	return transposed;
}

void copyMainBoardToGourobiBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[i][j].currentCellvalue == -1) {
				board[i][j] = 0;
			}
			else {
				board[i][j] = mainGameBoard[i][j].currentCellvalue;
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
	int k;
	k = 0;
	while(k < x){
		row = rand() % boardSize;
		col = rand() % boardSize;
		num = rand() % boardSize;
		if (checkValidity(board, row, col, num)) {
			board[row][col] = num;
			k++;
		}
	}
	return board;
}

int checkValidityGenerate(int** board, int row, int col, int num) {
	int i;
	int j;
	if (!checkBlockValidity(board, row, col, num)) {
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
				return 0; /*this means invalid*/
			}
		}
	}

	return 1; /*valid*/

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


/*everything that starts with 'test'*/

void test_initBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			board[i][j] = -1;
		}
	}
}

void test_printBoard(int** board) {
	int i;
	int j;



	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			printf(" %2d ", board[i][j]);
		}
		printf("\n");
	}
}

void test_MAIN() {
	int** transposed;
	RESULTS res;
	printf("in test_Main\n");
	board = allocateMemForBoardPTR();
	test_printBoard(board);
	printf("1\n");
	test_initBoard();
	test_printBoard(board);
	printf("2\n");
	printf("\n\n");
	solve();
	test_printBoard(res.solBoard);
	printf("3\n");
	printf("\n\n");
	transposed = transpose(res.solBoard);
	test_printBoard(transposed);
	printf("4\n");
	
}

int test_solverTest() {
	int good;
	printf("enter boardSize\n");
	if (scanf("%d", &boardSize)) {
		good = 1;
	}
	else {
		good = 0;
	}
	printf("enter blockHeight\n");
	if (scanf("%d", &blockHeight)) {
		good = 1;
	}
	else {
		good = 0;
	}
	printf("enter blockWidth\n");
	if (scanf("%d", &blockWidth)) {
		good = 1;
	}
	else {
		good = 0;
	}
	printf("%d\n", good);
	return 0;
}


/*solver-based functions*/


int hintSolve(int row, int coloumn) {
	int num;

	board = allocateMemForBoardPTR();
	copyMainBoardToGourobiBoard();

	solve();

	num = res.solBoard[row][coloumn];

	free(board);

	return num;

}


void generateSolve(int x, int y) {
	board = allocateMemForBoardPTR();
	do {
		test_initBoard();
		setRandom(board, x);
		solve();
	} while (res.optimstatus != GRB_OPTIMAL);
	
	deleteExcept(res.solBoard, y);
	copyGourobiBoardToMainBoard();
	free(board);
}


int validateSolve() {
	if (isErroneous) {
		printf("Error: board contains erroneous values\n");
		return 0;
	}
	board = allocateMemForBoardPTR();
	copyMainBoardToGourobiBoard();
	solve();

	if (res.optimstatus == GRB_OPTIMAL) {
		printf("Validation passed: board is solvable\n");
		return 1;
	}
	else if (res.optimstatus == GRB_INF_OR_UNBD) {
		printf("Validation failed: board is unsolvable\n");
		return 0;
	}
	free(board);
}








