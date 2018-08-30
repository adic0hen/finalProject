#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"




int** board;

int test_boardSize;


int allocateMemForBoard(int test) {
	int i;
	void* tempPTR;
	int** allocatedMemAddr;
	int size;


	if (test) {
		size = test_boardSize;
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

}







void test_initBoard() {
	int i;
	int j;

	for (i = 0; i < test_boardSize; i++) {
		for (j = 0; j < test_boardSize; j++) {
			board[i][j] = 0;
		}
	}
}


void test_printBoard() {
	int i;
	int j;



	for (i = 0; i < test_boardSize; i++) {
		for (j = 0; j < test_boardSize; j++) {
			printf(" %2d ", board[i][j]);
		}
		printf("\n");
	}

}

void test_MAIN() {
	test_boardSize = 9;
	allocateMemForBoard(1);
	test_initBoard;
	board[1][2] = 5;
	board[3][4] = 7;
	board[8][1] = 4;
	board[5][3] = 4;
	board[1][3] = 9;
	board[0][0] = 7;
	board[0][3] = 2;

	test_printBoard();
	solve();

	test_printBoard();
}




/* -------- NO NEED THESE FUNCTIONS FOR TESTING-------------*/
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

	/* NEED TO CHECK IF SOLVER HAS FINISHED */
	isSolvabe = 0; /* ---TEMP VALUE--- NEED TO REPLACE WITH THE TRUE VALUE */

	free(board);

	return isSolvabe;
}