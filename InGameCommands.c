#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"





void printBoard(int markErrors) {
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
					else if (mainGameBoard[rowCounter][j].isErroneus) {
						printf("*");
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


int checkValidityOfNum(int number, int row, int coloumn) {
	int i;
	int j;
	int blockRow;
	int blockCol;

	blockRow = row / (boardSize / blockHeight);
	blockCol = coloumn / (boardSize / blockWidth);

	if (mainBlockBoard[blockRow][blockCol].numbersList[number - 1] == number) {
		return 0;
	}

	for (i = 0; i < boardSize; i++) {
		if (mainGameBoard[row][i].currentCellvalue == number) {
			return 0;
		}
	}

	for (i = 0; i < boardSize; i++) {
		if (mainGameBoard[i][coloumn].currentCellvalue == number) {
			return 0;
		}
	}

	return 1;
}


void deleteCell(int row, int coloumn, int number) {
	int blockRow;
	int blockCol;

	blockRow = row / (boardSize / blockHeight);
	blockCol = coloumn / (boardSize / blockWidth);

	mainBlockBoard[blockRow][blockCol].numbersList[number - 1] = 0;
	mainGameBoard[row][coloumn].currentCellvalue = -1;

}


int set(int row, int coloumn, int number) {
	int blockRow;
	int blockCol;
	int isValidNumber;

	blockRow = row / (boardSize / blockHeight);
	blockCol = coloumn / (boardSize / blockWidth);

	if (number > boardSize || number < 0) {
		return 0; //Error of Invalid Number//
	}
	if (row >= boardSize || coloumn >= boardSize) {
		return 0;
	}

	if (number == 0) {
		if (mainGameBoard[row][coloumn].isFixed) {
			return 2; //Error of clearing a fixed cell//
		}
		else {
			deleteCell(row, coloumn, number);
			return 1;
		}
	}

	isValidNumber = checkValidityOfNum(number, row, coloumn);

	if (isValidNumber == 0) {
		mainGameBoard[row][coloumn].currentCellvalue = number;
		mainGameBoard[row][coloumn].isErroneus = 1;
		mainBlockBoard[blockRow][blockCol].numbersList[number - 1] = number;
		return 1;
	}
	else {
		mainGameBoard[row][coloumn].currentCellvalue = number;
		mainBlockBoard[blockRow][blockCol].numbersList[number - 1] = number;
	}
}




		






