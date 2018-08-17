#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"





void printBoard() {
	int i;
	int j;
	int rowCounter;
	int numberOfBlocks;

	numberOfBlocks = boardSize / blockWidth;
	rowCounter = 0;
	while (rowCounter < boardSize)
	{

		//print dashes//
		if (rowCounter % blockHeight == 0)
		{
			for (i = 0; i < (4 * boardSize + blockWidth + 1);i++) {
				printf("-");
			}
		}

		// for each row of numbers//
		if (rowCounter % blockHeight != 0)
		{

			// for each block//
			for (i = 0; i < numberOfBlocks; i++)
			{
				printf("|");

				// for each cell in the block, per row//
				for (j = 0; j < blockWidth; j++)
				{
					printf(" ");
					if (mainGameBoard[rowCounter][j].currentCellvalue == -1) {
						printf("  ");
					}
					else {
						printf("%2d", mainGameBoard[rowCounter][j].currentCellvalue);
					}

					if (mainGameBoard[rowCounter][j].isFixed) {
						printf(".");
					}
					else {
						printf(" ");
					}
				}

				printf("|");
			}

			rowCounter += 1;
		}
	}
}

		






