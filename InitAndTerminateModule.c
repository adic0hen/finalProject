#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"


int allocateMemForMainBoard(int N) {
	Cell** allocatedMemAddr;
	int i;
	void*  tempPTR;

	allocatedMemAddr = (Cell**)malloc(sizeof(Cell*) *N);
	if (allocatedMemAddr == NULL) {
		return 0;
	}

	for (i = 0; i < N; i++) {
		tempPTR = (malloc(sizeof(Cell) *N));
		if (tempPTR == NULL) {
			return 0;
		}
		allocatedMemAddr[i] = (Cell*)tempPTR;
	}

	// DONE ALLOCATING MEMORY FOR BOARD //

	mainGameBoard = allocatedMemAddr;

	return 1;
}


int allocateMemForBlockBoard() {
	Block** allocatedMemAddr;
	int i;
	int j;
	void* tempPTR;
	int numOfRows;
	int numOfCols;

	numOfRows = boardSize / blockHeight;
	numOfCols = boardSize / blockWidth;

	tempPTR =malloc(sizeof(Block*) *numOfRows);
	if (allocatedMemAddr == NULL) {
		return 0;
	}
	allocatedMemAddr = (Block**)tempPTR;


	for (i = 0; i < numOfRows; i++) {
		tempPTR = (malloc(sizeof(Block) *numOfCols));
		if (tempPTR == NULL) {
			return 0;
		}
		allocatedMemAddr[i] = (Block*)tempPTR;
	}

	// DONE ALLOCATING MEMORY FOR BOARD //

	mainBlockBoard = allocatedMemAddr;

	for (i = 0; i < numOfRows; i++) {
		for (j = 0; j < numOfCols; j++) {
			tempPTR = malloc(sizeof(int) * boardSize);
			if (tempPTR == NULL) {
				return 0;
			}
			mainBlockBoard[i][j].numbersList = (int*)tempPTR;
		}
	}


	return 1;
}


void initialaizeMainBoard() {
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

void initializeMainBlockBoard() {
	int i;
	int j;
	int k;
	int numOfRows;
	int numOfCols;
	Block currBlock;

	numOfRows = boardSize / blockHeight;
	numOfCols = boardSize / blockWidth;

	for (i = 0; i < numOfRows; i++) {
		for (j = 0; j < numOfCols; j++) {
			currBlock = mainBlockBoard[i][j];
			for (k = 0; k < boardSize; k++) {
				currBlock.numbersList[k] = 0;
			}
		}
	}
}


