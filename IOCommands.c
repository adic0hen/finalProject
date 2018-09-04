#include <stdio.h>
#include <stdlib.h>
#include "InGameCommands.h"
#include "GamePlay.h"
#include "GameDataStructs.h"
#include "InitAndTerminateModule.h"

/* Declaration of functions*/

int save(char* path);
writeBoardToFile(FILE* fptr);
int load(char* path);


/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/


int save(char* path) {
	FILE* fptr;
	

	if (mode == 3) { /*in Edit mode we need to make some pre checks*/
		if (isErroneous(mainGameBoard)) {
			printf("Error: board contains erroneous values\n");
			return 0;
		}
		/*need to add the validation part, it uses gurobi i think*/
	}
	fptr = fopen(path, "w");
	if (fptr == NULL) {/*when fopen fails, meaning that the file cannot be created or modified*/
		printf("Error: File cannot be created or modified\n");
		return 0;
	}
	writeBoardToFile(fptr);
	fclose(fptr);
	printf("Saved to: %s\n", path);
}

writeBoardToFile(FILE* fptr) {
	int i;
	int j;
	int num;
	Cell currentCell;
	fprintf(fptr, "%d %d\n", blockHeight, blockWidth);
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			currentCell = mainGameBoard[i][j];
			num = currentCell.currentCellvalue;
			if (num == -1) { /*this part relates to working with their save format*/
				num = 0;
			}
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

int load(char* path) {
	FILE* fptr;
	fptr = fopen(path, "r");
	if (fptr == NULL) {
		if (mode == 2) { /*error in solve mode*/
			printf("Error: File doesn't exit or cannot be opened\n");
		}
		if (mode == 3) { /*error in edit mode*/
			printf("Error: File cannot be opened\n");
		}
		return 0;
	}
	fscanf(fptr, "%d", &blockHeight);
	fscanf(fptr, "%d", &blockWidth);
	boardSize = (blockHeight * blockWidth);
	allocateMemForMainBoard(boardSize);
	initializeLoadedMainBoard(fptr);
	fclose(fptr);
	return 1;
}
/*TESTIG CODE, TO BE DELETED*/
void IOTest(){
	char* path;
	path = "file.txt";
	save(path);
	load(path);
	printBoard(0);
	printf("done testing \n");

}


