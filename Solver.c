#include <stdlib.h>
#include <stdio.h>

#include "gurobi_c.h"
#include <string.h>
#include "InitAndTerminateModule.h"
#include "InGameCommands.h"
#include "GameDataStructs.h"



/*Outer variable declarations*/
/* COMMENTED OUR FOR COMPILATION
int boardSize;
int blockHeight;
int blockWidth;
int** mainGameBoard;
*/

int** board; /*is used in the gurobi optimization process, "gurobi board"*/
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
/*void freeVars(int* ind, double* val, double* lb, char* vtype, char** names, char* namestorage, double* sol);*/
/*test function declarations
void test_MAIN();
int testGEN();
int test_generate(int cellsToFill, int cellsToKeep);
void test_transpose();
int test_solverTest();
void test_initMainBoard();
void test_copySolvedBoardToMainBoard();
*/


/*COMMENTED OUR FOR COMPILATION
int main() {
	printf("in main\n");
	boardSize = 9;
	blockHeight = 3;
	blockWidth = 3;
	mainGameBoard = allocateMemForBoardPTR();
	test_initMainBoard();
	printf("generate test\n");
	testGEN();
	return 0;
}
*/
int solveMain(int isGenerate) {
	int temp;
	int toTranspose;
	/*
	printf("check and maybe free the board before allocationg\n");
	if (board != NULL) {
		freeMat(board);
	}
	*/
	/*test_initBoard();*/
	if (!isGenerate) {
		copyMainBoardToGourobiBoard();
	}
	if (blockHeight > blockWidth) {
		toTranspose = 1;
	}
	else {
		toTranspose = 0;
	}
	if (toTranspose) { /*solver only works if blockHeight>=blockWidth*/
		board = transpose(board);
		temp = blockHeight;
		blockHeight = blockWidth;
		blockWidth = temp;
	}
	printf("starting the solve function\n");
	solve();
	if (toTranspose) {
		res.solBoard = transpose(res.solBoard);
		temp = blockHeight;
		blockHeight = blockWidth;
		blockWidth = temp;
	}
	/*copySolvedBoardToMainBoard();*/
	/*
	if (fromHint) {
		return 1;
	}
	else {
		freeSolver();
	}
	*/
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

	printf("ENTER SOLVEr\n\n");
	printf("current sizes are:\n boardSize - %d  bloackHeight - %d blockWidth - %d\n",boardSize,blockHeight,blockWidth);
	res.objval = 0.0;
	res.optimstatus = 0;
	/*defining variables*/
	printf("allocating for ind\n");
	ind = malloc(boardSize * sizeof(int));
	if (!ind) {
		printf("error in ind");
	}
	printf("allocating for val\n");
	val = malloc(boardSize * sizeof(double));
	if (!val) {
		printf("error in val");
	}
	printf("allocating for lb\n");
	lb = malloc(boardSize * boardSize * boardSize * sizeof(double));
	if (!lb) {
		printf("error in lb");
	}
	printf("allocating for vtype\n");
	vtype = malloc(boardSize * boardSize * boardSize * sizeof(char));
	if (!vtype) {
		printf("error in vtype");
	}
	printf("allocating for names\n");
	names = malloc(boardSize * boardSize * boardSize * sizeof(char*));
	if (!names) {
		printf("error in names");
	}
	printf("allocating for namestorage\n");
	namestorage = malloc(20 * boardSize * boardSize * boardSize * sizeof(char));
	if (!namestorage) {
		printf("error in namestorage");
	}
	printf("allocating for sol\n");
	sol = malloc(boardSize * boardSize*boardSize * sizeof(double));
	if (!sol) {
		printf("error in sol");
	}
	error = 0;
	model = NULL;
	env = NULL;
	
	

	printf("ALLOCATED MEM\n\n");
	

	/* Create an empty model */

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

	printf("Created empty model\n\n");

	/* Create environment */
	
		error = GRBloadenv(&env, NULL);
		if (error) {
			printf("ERROR IN loadenv: %d", error);
			quit(error, env);
		}
	

	printf("Created loadedenv\n\n");

	/* Create new model */
	
	error = GRBnewmodel(env, &model, "sudoku", boardSize*boardSize*boardSize, NULL, lb, NULL, vtype, names);
	if (error) {
		printf("ERROR IN newmodel: %d", error);
		quit(error, env);
	}

	printf("Created new model\n\n");


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
				printf("ERROR IN addconstr: %d", error);
				quit(error, env);
			}
		}
	}

	printf("Added constraints of cells\n\n");
	/* Each value must appear once in each row */

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

	printf("Added constraints of rows\n\n");


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

	printf("Added constraints of coloumns\n\n");

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
	printf("Added constraints of blocks\n\n");


	/* Optimize model */

	error = GRBoptimize(model);
	if (error) {
		printf("ERROR IN OPTIMIZATION: %d", error);
		quit(error, env);
	}
	
	printf("After optimization\n\n");


	/* Capture solution information */
	
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
		
		printf("freeing rest of the model\n");
	}

	/*print test ~~~~~~~~~CAN BE DELETED~~~~~ COMMENTED OUT FOR NOW~~~~~~*/
	/*
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
	*/



	printf("Free env and model\n");
	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

	printf("freeing variables\n");
	freeVars(ind, val, lb, vtype, sol,names,namestorage);
}


/*assisting subfunctions*/

void quit(int error, GRBenv *env) {

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}
}

/*~~~~~~~~~~~THIS FUNCTION MAY NOT BE NECESSARY~~~~~~~~~~~*/
void freeVars(int* ind, double* val, double* lb, char* vtype, double* sol,char** names, char*namestorage) {
	
	printf("free ind\n");
	free(ind);
	printf("free val\n");
	free(val);
	printf("free lb\n");
	free(lb);
	printf("free vtype\n");
	free(vtype);
	printf("free sol\n");
	free(sol);
	printf("free namestorage\n");
	free(namestorage);
	printf("free names\n");
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
		return 0;
	}
	else {
		allocatedMemAddr = (int**)tempPTR;
	}

	for (i = 0; i < boardSize; i++) {
		tempPTR = malloc(sizeof(int)*boardSize);
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
	freeMat(board);/*board is freed because the pointer is going to be set to the transposed bord*/
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
				} while (options[num - 1] == 0); /*finding a number that we havent tried yet*/
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
				printf("impossible cell\n");/*for tests*/
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
				return 0; /*this means invalid*/
			}
		}
	}

	return 1; /*is valid*/

}

int** deleteExcept(int** board, int y) {
	
	int row;
	int col;
	int toRemove;
	int k;
	printf("in deletExcept\n"); /*for testing*/
	toRemove = boardSize * boardSize - y;
	k = 0;
	while (k < toRemove) {
		row = rand() % boardSize;
		col = rand() % boardSize;
		/*printf("deleting row - %d and col - %d\n", row, col); for testing*/
		if (board[row][col] != -1) {
			board[row][col] = -1;
			k++;
		}
		/*printf("done deleting\n"); for testing*/
	}
	return board;
}


/*everything that starts with 'test'*/

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

/*solver-based functions*/


int hintSolve(int row, int coloumn) { /*returns the hint value if board is solvable, 0 otherwise*/
	int num;
	printf("in hintSolve");/*for testing!*/
	board = allocateMemForBoardPTR();
	solveMain(0);
	if (res.optimstatus == GRB_OPTIMAL) {
		printf("is optimal\n");/*for testing!*/
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
	printf("in generateSolve\n");
	b = 0; /*b will contain boolean value: weather the board was successfully solved or not*/
	board = allocateMemForBoardPTR();
	initBoardSolver();
	if (setRandom(board, x) == NULL) {
		return b;
	}
	printf("done setting random values\n");
	printBoardSolver(board); /*for testing*/
	solveMain(1);
	if (res.optimstatus == GRB_OPTIMAL) {
		deleteExcept(res.solBoard, y);
		copySolvedBoardToMainBoard(); /*the real function! the board is not "solved" but it's the one we want to use*/
		/*test_copySolvedBoardToMainBoard(); only for tests!*/
		/* Need to add update to Undo-Redo List (can be in the middle of the game, while URList is not empty) */
		printf("going to free mem\n");/*for testing*/
		freeSolver();
		b = 1;
	}
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
	printf("Enter freeSolver\n");
	freeMat(board);
	printf("freed mat1\n");
	freeMat(res.solBoard);
}










