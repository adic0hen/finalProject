#include <stdlib.h>
#include <stdio.h>
/*#include "GameDataStructs.h"*/
#include "gurobi_c.h"
#include <string.h>


int boardSize;
int blockHeight;
int blockWidth;
int** board;
int solverTest();

void solve();
void quit(int error, GRBenv *env);
void test_MAIN();

int main() {
	solverTest();
	return 0;
}


int allocateMemForBoard(int test) {
	int i;
	void* tempPTR;
	int** allocatedMemAddr;
	int size;


	if (test) {
		size = boardSize;
	}
	else {
		size = boardSize;
	}



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

	board = allocatedMemAddr;
	return 1;
}







void test_initBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			board[i][j] = -1;
		}
	}
}


void test_printBoard() {
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
		
		
	printf("\n\n\n NOW TESTING HEIGHT %d AND WIDTH %d\n\n\n", blockHeight, blockWidth);
	allocateMemForBoard(1);
	test_initBoard();

	test_printBoard();
	solve();

	test_printBoard();
	free(board);
	
}




/* -------- NO NEED THESE FUNCTIONS FOR TESTING-------------
void copyMainBoardToGourobiBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < board;j++) {
			if (mainGameBoard[i][j].currentCellvalue == -1) {
				board[i][j] = 0;
			}
			else {
				board[i][j] = mainGameBoard[i][j].currentCellvalue;
			}
		}
	}
}


void copyGourobiBoardToMainBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			mainGameBoard[i][j].currentCellvalue = board[i][j];
			mainGameBoard[i][j].isErroneus = 0;
			mainGameBoard[i][j].isFixed = 0;
		}
	}
}



int hintSolve(int row, int coloumn) {
	int num;

	allocateMemForBoard(0);
	copyMainBoardToGourobiBoard();

	solve();

	num = board[row][coloumn];

	free(board);

	return num;

}


void generateSolve() {

	allocateMemForBoard(0);

	solve();

	copyGourobiBoardToMainBoard();

	free(board);

}


int validateSolve() {
	int isSolvabe;
	allocateMemForBoard(0);
	copyMainBoardToGourobiBoard();


	isSolvabe = 0;
	free(board);

	return isSolvabe;
}

*/



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
	/*
	int zero;
	*/
	int error;
	double* sol;

	
	/*defining variables*/
	ind = (int*)malloc(boardSize * sizeof(int));
	val = (double*)malloc(boardSize * sizeof(double));
	lb = (double*)malloc(boardSize * boardSize * boardSize * sizeof(double));
	vtype = (char*)malloc(boardSize * boardSize * boardSize * sizeof(char));
	names = (char**)malloc(boardSize * boardSize * boardSize * sizeof(char));
	namestorage = (char*)malloc(10 * boardSize * boardSize * boardSize * sizeof(char));
	error = 0;
	/*
	zero = 0;
	*/
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

	/* Write model to 'sudoku.lp' */

	error = GRBwrite(model, "sudoku.lp");
	if (error) {
		quit(error, env);
	}

	/* Capture solution information */

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



	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL)
		printf("Optimal objective: %.4e\n", objval);
	else if (optimstatus == GRB_INF_OR_UNBD)
		printf("Model is infeasible or unbounded\n");
	else
		printf("Optimization was stopped early\n");
	printf("\n");

	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

}

void quit(int error, GRBenv *env) {

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
}

int solverTest() {
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
	test_MAIN();
	return 0;
}