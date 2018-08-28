#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"

int init() {
}

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




