#include <stdio.h>
#include <stdlib.h>
#include "InGameCommands.h"
#include "GamePlay.h"
#include "GameDataStructs.h"
#include "InitAndTerminateModule.h"
/*IOCommands module encloses implementation of commands with the regard of loading or saving the game board to a file*/

/* Declaration of functions*/

int save(char* path);
int writeBoardToFile(FILE* fptr);
int load(char* path);


/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/


int save(char* path) {
	FILE* fptr;
	

	if (mode == EDIT_MODE) { /*in Edit mode*/
		if (isErroneous(mainGameBoard)) {
			printf("Error: board contains erroneous values\n");
			return 0;
		}
		if (!validate(1)) {
			printf("Error: board validation failed\n");
			return 0;
		}
	}
	fptr = fopen(path, "w");
	if (fptr == NULL) {
		printf("Error: File cannot be created or modified\n");
		return 0;
	}
	writeBoardToFile(fptr);
	fclose(fptr);
	printf("Saved to: %s\n", path);
	return 1;
}


int writeBoardToFile(FILE* fptr) {
	int i;
	int j;
	int num;
	Cell currentCell;
	fprintf(fptr, "%d %d\n", blockHeight, blockWidth);
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			currentCell = mainGameBoard[i][j];
			num = currentCell.currentCellvalue;
			if (num == -1) { /*Converting from our definition for empty cell to the save format*/
				num = 0;
			}
			if (((currentCell.isFixed == 1) || (mode == EDIT_MODE))&&num) {
				fprintf(fptr, "%d. ", num);
			}
			else {
				fprintf(fptr, "%d ", num);
			}
		}
		fprintf(fptr, "\n");
	}
	return 1;
}


int load(char* path) {
	FILE* fptr;
	int bHeight;
	int bWidth;

	fptr = fopen(path, "r");
	if (fptr == NULL) {
		if (mode == SOLVE_MODE) { /*error in solve mode*/
			printf("Error: File doesn't exist or cannot be opened\n");
		}
		if (mode == EDIT_MODE) { /*error in edit mode*/
			printf("Error: File cannot be opened\n");
		}
		return 0;
	}
	if (!fscanf(fptr, "%d", &bHeight) > 0) {
		return 0;
	}
	if (!fscanf(fptr, "%d", &bWidth) > 0){
		return 0;
	}
	freeAll();
	initAllFromFile(bHeight, bWidth);
	initializeLoadedMainBoard(fptr);
	updateErrStatAndCountEmptyCells();
	printBoard();
	fclose(fptr);
	return 1;
}



