#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"



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

	// DONE ALLOCATING MEMORY FOR BOARD //

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
	int* tempNum;
	char* tempDot;


	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			fscanf(fptr, "%d", tempNum); /*scanning the current Cell's number*/
			fscanf(fptr, "%d", tempDot);/*either a dot right after the number, or a white space*/

			mainGameBoard[i][j].currentCellvalue = *tempNum;

			if (strcmp(tempDot, ".") == 0) {
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

	// DONE ALLOCATING MEMORY FOR BOARD //

	LIFOCells = allocatedMemAddr;

	return 1;
}


void initialUndoRedoListAndLIFOCells() {
	int i;
	int j;

	UndoRedoList.currentMove = NULL;
	UndoRedoList.isEmpty = 1;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize;j++) {
			LIFOCells[i][j].first = NULL;
		}
	}

}



