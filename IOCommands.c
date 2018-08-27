#include <stdio.h>
#include <stdlib.h>
#include "InGameCommands.h"
#include "GamePlay.h"
#include "GameDataStructs.h"

/* Declaration of functions*/

int save(char* path);
writeBoardToFile(FILE* fptr);


/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/


int save(char* path) {
	FILE* fptr;

	if (mode == 3) { /*in Edit mode we need to make some validations*/
		if (isErroneous(mainGameBoard)) {
			printf("Error: board contains erroneous values\n");
			return 0;
		}
		/*need to add the validation part, it uses gurobi i think*/
	}
	fptr = fopen(*path, "w");
	if (fptr == NULL) {/*when fopen fails, meaning that the file cannot be created or modified*/
		printf("Error: File cannot be created or modified\n");
	}
	writeBoardToFile(fptr);
	fclose(fptr);
	printf("Saved to: %s", path);
}

writeBoardToFile(FILE* fptr) {
	int i;
	int j;
	int num;
	Cell currentCell;
	fprintf(fptr, "%d %d", blockHeight, blockWidth);
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			currentCell = mainGameBoard[i][j];
			num = currentCell.currentCellvalue;
			if (currentCell.isFixed) {
				fprintf(fptr, "%d. ", num);
			}
			else {
				fprintf(fptr, "%d ", num);
			}
		}
		fprintf(fptr, "\n");
	}
}