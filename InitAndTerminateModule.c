#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"
#include "UndoRedoCommands.h"
#include "Solver.h"

/*function declarations*/
void freeMainGameBoard();
void freeAll();


int allocateMemForMainBoard() {
	Cell** allocatedMemAddr;
	int i;
	void*  tempPTR;

	allocatedMemAddr = (Cell**)malloc(sizeof(Cell*) *boardSize);
	if (allocatedMemAddr == NULL) {
		return 0;
	}

	for (i = 0; i < boardSize; i++) {
		tempPTR = (malloc(sizeof(Cell) *boardSize));
		if (tempPTR == NULL) {
			return 0;
		}
		allocatedMemAddr[i] = (Cell*)tempPTR;
	}

	/* DONE ALLOCATING MEMORY FOR BOARD */

	mainGameBoard = allocatedMemAddr;

	return 1;
}

void initializeMainBoard() {
	int i;
	int j;


	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			mainGameBoard[i][j].currentCellvalue = -1;
			mainGameBoard[i][j].isFixed = 0;
			mainGameBoard[i][j].isErroneus = 0;
		}
	}

}

void initializeLoadedMainBoard(FILE *fptr) {
	int i;
	int j;
	int tempNum;
	char tempDot;
	
	

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if(!fscanf(fptr, "%d", &tempNum)>0){; /*scanning the current Cell's number*/
			return;
			}
			if(!fscanf(fptr, "%c", &tempDot)>0){;/*either a dot right after the number, or a white space*/
				return;
			}
			if (tempNum == 0) { /*this part relates to working with their save format*/
				mainGameBoard[i][j].currentCellvalue = -1;
			}
			else {
				mainGameBoard[i][j].currentCellvalue = tempNum;
			}
			if (tempDot == '.'|| (mode == 3 && mainGameBoard[i][j].currentCellvalue != -1)) {
				mainGameBoard[i][j].isFixed = 1;
			}
			else {
				mainGameBoard[i][j].isFixed = 0;
			}

			mainGameBoard[i][j].isErroneus = 0;
		}
	}
}


void initiallizeGameParameters(int N, int bHeight, int bWidth) {
	boardSize = N;
	blockHeight = bHeight;
	blockWidth = bWidth;
}


int allocateMemForLIFOCells() {
	cellNodeGuard** allocatedMemAddr;
	int i;
	void*  tempPTR;

	allocatedMemAddr = (cellNodeGuard**)malloc(sizeof(cellNodeGuard*) *boardSize);
	if (allocatedMemAddr == NULL) {
		return 0;
	}

	for (i = 0; i < boardSize; i++) {
		tempPTR = (malloc(sizeof(cellNodeGuard) *boardSize));
		if (tempPTR == NULL) {
			return 0;
		}
		allocatedMemAddr[i] = (cellNodeGuard*)tempPTR;
	}

	/* DONE ALLOCATING MEMORY FOR BOARD */

	LIFOCells = allocatedMemAddr;

	return 1;
}


void initialUndoRedoListAndLIFOCells() {
	int i;
	int j;

	UndoRedoList.currentMove = NULL;
	UndoRedoList.isEmpty = 1;
	UndoRedoList.next = NULL;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize;j++) {
			LIFOCells[i][j].first = NULL;
		}
	}

	UndoRedoList.initialBoard = NULL;
	UndoRedoList.hasLoadedBoard = 0;
	UndoRedoList.selfCurrentMove = 0;
}





void freeAll() { /*frees the memory of the main game board, the solver board, and the URList*/
	freeMainGameBoard();
	freeSolver();
	freeURResources();
}

void freeMat(int** mat) { /*free the memory of a 2D array*/
	int i;
	for (i = 0; i < boardSize; i++) {
		free(mat[i]);
	}
	free(mat);

}

void freeMainGameBoard() {/*free the memory of the main game board only*/
	int i;
	int j;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; i < boardSize; j++) {
			free(&mainGameBoard[i][j]);
		}
	}
	for (i = 0; i < boardSize; i++) {
		free(mainGameBoard[i]);
	}
	free(mainGameBoard);
}

void reset() {
	clearURListFromCurrentPosition(NULL, 1);
	updateMainBoardToFirstBoard();
	printf("Board reset\n");
}

void prepForLoad() {
	freeAll();
	allocateMemForLIFOCells();
	initialUndoRedoListAndLIFOCells();
}


void exitGame() {
	freeAll();
	printf("Exiting...\n");

}