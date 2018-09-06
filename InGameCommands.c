#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"
#include "GamePlay.h"
#include "UndoRedoCommands.h"
#include "Solver.h"
#include <string.h>

/* Declaration of functions*/

void autofillROWS(int** boardToFill, int expectedSum);
void autofillCOLOUMS(int** boardToFill, int expectedSum);
void autofillBLOCKS(int** boardToFill, int expectedSum);
void autofillFILLCELLS(int** boardToFill);
int isErroneous();
int markErrors(int mark);
int checkGenerateParameters(char*sCellsToFill, char*sCellsToKeep, int cellsToFill, int cellsToKeep);
int checkHintParameters(int row, int col);
int validate();
int generate(char *sCellsToFill, char *sCellsToKeep, int cellsToFill, int cellsToKeep);
int hint(int row, int col);




/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/


int checkBlockForNumber(int row, int coloumn, int number) {
	int i;
	int j;
	int rowBlockStart;
	int coloumnBlockStart;

	rowBlockStart = row / blockHeight;
	coloumnBlockStart = coloumn / blockWidth;

	for (i = (rowBlockStart*blockHeight); i < (rowBlockStart+1)*blockHeight; i++) {
		for (j = (coloumnBlockStart*blockWidth); j < (coloumnBlockStart+1)*blockWidth; j++) {
			if (mainGameBoard[i][j].currentCellvalue == number) {
				return 1;
			}
		}
	}

	return 0;

}


void printBoard() {
	int i;
	int j;
	int rowCounter;
	int numberOfBlocksInRow;
	int boardRow;
	int numOfIterations;

	numOfIterations = boardSize + (boardSize / blockHeight);
	numberOfBlocksInRow = boardSize / blockWidth;
	rowCounter = 0;
	boardRow = 0;

	while (rowCounter <= numOfIterations)
	{

		/*print dashes*/
		if (rowCounter % (blockHeight + 1) == 0)
		{
			for (i = 0; i < (4 * boardSize + blockHeight + 1);i++) {
				printf("-");
			}
			printf("\n");
		}

		/* for each row of numbers*/
		if (rowCounter % (blockHeight + 1) != 0)
		{

			/* for each block*/
			for (i = 0; i < numberOfBlocksInRow; i++)
			{
				printf("|");

				/* for each cell in the block, per row*/
				for (j = (i*blockWidth); j < ((i + 1)*blockWidth); j++)
				{
					printf(" ");
					if (mainGameBoard[boardRow][j].currentCellvalue == -1) {
						printf("  ");
					}
					else {
						printf("%2d", mainGameBoard[boardRow][j].currentCellvalue);
					}

					if (mainGameBoard[boardRow][j].isFixed) {
						printf(".");
					}
					else if (mainGameBoard[boardRow][j].isErroneus) {
						if (markerrors) {
							printf("*");
						}
						else {
							printf(" ");
						}
					}
					else {
						printf(" ");
					}
				}
			}

			boardRow += 1;
			printf("|\n");

		}

		rowCounter += 1;
	}
}


int checkValidityOfNum(int number, int row, int coloumn) {
	int i;


	if (checkBlockForNumber(row, coloumn, number)) {
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

int checkBoardValidity() {
	int isValidNumber;
	int num;
	int row;
	int col;
	for (row = 0; row < boardSize; row++) {
		for (col = 0; col < boardSize; col++) {
			if (mainGameBoard[row][col].currentCellvalue == -1) {
				continue;
			}
			num = mainGameBoard[row][col].currentCellvalue;
			mainGameBoard[row][col].currentCellvalue = -1;
			isValidNumber = checkValidityOfNum(num,row,col);
			if (!isValidNumber&&mainGameBoard[row][col].isFixed == 0) {
				mainGameBoard[row][col].isErroneus = 1;
			}
			else {
				mainGameBoard[row][col].isErroneus = 0;
			}
			mainGameBoard[row][col].currentCellvalue = num;
		}
	}
	return 1;
}


void deleteCell(int row, int coloumn) {
	mainGameBoard[row][coloumn].currentCellvalue = -1;
	mainGameBoard[row][coloumn].isErroneus = 0;

}


void set(int row, int coloumn, int number, int isErroneous) {

	if (isErroneous) {
		mainGameBoard[row][coloumn].currentCellvalue = number;
		mainGameBoard[row][coloumn].isErroneus = 1;
	}
	else {
		mainGameBoard[row][coloumn].currentCellvalue = number;
	}
}


/* row,coloumn as user entered */
int setMAIN(int row, int coloumn, int number) {
	int isValidNumber;
	int prevNumber;


	if (number > boardSize || number < 0) {
		return 0; /*Error of Invalid Number*/
	}
	if (row > boardSize || coloumn > boardSize) {
		return 0;
	}
	if (row < 1 || coloumn < 1) {
		return 0;
	}

	prevNumber = mainGameBoard[row - 1][coloumn - 1].currentCellvalue;

	if (number == 0) {
		if (mainGameBoard[row - 1][coloumn - 1].isFixed) {
			return 2; /*Error of clearing a fixed cell*/
		}
		else {
			deleteCell(row - 1, coloumn - 1);
			return 1;
		}

		if (prevNumber == -1) {
			return 1;
		}
	}

	if (number != 0) {
		if (prevNumber == number) {
			return 1;
		}
		else {
			isValidNumber = checkValidityOfNum(number, row - 1, coloumn - 1);
			if (!isValidNumber) {
				set(row - 1, coloumn - 1, number, 1);
			}
			else {
				set(row - 1, coloumn - 1, number, 0);
			}
		}
	}

	checkBoardValidity();
	
	updateURListAfterSet(row - 1, coloumn - 1, &mainGameBoard[row - 1][coloumn - 1], 1);

	return 1;
}


int isErroneous() {
	int i;
	int j;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[i][j].isErroneus == 1) {/*if one of the cells is erroneus*/
				return 1;
			}
		}
	}
	return 0;
}

int markErrors(int mark) {

	if (mark == 0 || mark == 1) {
		markerrors = mark;
		return 1;
	}
	else {
		printf("Error: the value should be 0 or 1\n");
		return 0;
	}
}

int isEmpty() {
	int i;
	int j;

	for (i = 0; i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if (mainGameBoard[i][j].currentCellvalue != -1) {
				return 0;
			}
		}
	}

	return 1;
}


/*solver relying functions - NEED TO BE TESTED*/

int generate(char *sCellsToFill, char *sCellsToKeep, int cellsToFill, int cellsToKeep) {
	int i;
	i = 0;
	if (!checkGenerateParameters(sCellsToFill, sCellsToKeep, cellsToFill, cellsToKeep)) {
		return 0;
	}
	if (!isEmpty()) {
		printf("Error: board is now empty\n");
		return 0;
	}
	while (i < 1000) {
		if (generateSolve(cellsToFill, cellsToKeep)) {
			break;
		}
		else {
			i++;
		}
	}
	if (i == 1000) {
		printf("Error: puzzle generator failed\n");
		return 0;
	}
	printBoard();
	return 1;
}



int checkGenerateParameters(char*sCellsToFill, char*sCellsToKeep, int cellsToFill, int cellsToKeep) {
	int error;
	int isValid;
	int numOfCells;
	error = 0;
	numOfCells = boardSize * boardSize;
	if ((strcmp(sCellsToFill, "0") != 0) && (cellsToFill == 0)) {
		error = 1;
	}
	else if ((strcmp(sCellsToKeep, "0")) != 0 && (cellsToKeep == 0)) {
		error = 1;
	}
	else if (cellsToFill < 0 || cellsToFill> boardSize*boardSize) {
		error = 1;
	}

	if (error) {
		printf("Error: value not in range 0-%d", numOfCells);
	}
	isValid = !error;
	return isValid;
}



int hint(int row, int col) {
	int h;
	if (checkHintParameters(row, col)) {
		return 0;
	}
	h = hintSolve(row -1, col-1); 
	if (h == 0) {
		return 0;
	}
	printf("Hint, set cell to %d\n",h);
	return 0;
}

int checkHintParameters(int row, int col) {
	if (row<1 || row>boardSize || col<1 || col>boardSize) {
		printf("Error: value not in range 1-%d\n", boardSize);
		return 0;
	}
	if (isErroneous()) {
		printf("Error: board contains erroneous values\n");
		return 0;
	}
	if (mainGameBoard[row - 1][col - 1].isFixed == 1) {
		printf("Error: cell is fixed\n");
		return 0;
	}
	if (mainGameBoard[row - 1][col - 1].currentCellvalue != -1) {
		printf("Error: cell already contains a value\n");
		return 0;
	}
	return 1;
}



int validate() {
	int isValid;
	isValid = 0;
	if (isErroneous()) {
		printf("Error: board contains erroneous values\n");
		return 0;
	}
	isValid = validateSolve();
	if (isValid == 1) {
		printf("Validation passed: board is solvable\n");
		return 1;
	}
	else if (isValid == 0) {
		printf("Validation failed: board is unsolvable\n");
		return 0;
	}
	return 0;
}



/* -------------- autofill commands--------------- */


int autofill() {
	int i;
	int j;
	int expectedSum;
	int ** boardToFill;
	void* tempPTR;


	tempPTR = malloc(sizeof(int*) * boardSize);
	if (tempPTR == NULL) {
		return 0;
	}
	else {
		boardToFill = (int**)tempPTR;
		for (i = 0; i < boardSize;i++) {
			tempPTR = malloc(sizeof(int) * boardSize);
			if (tempPTR == NULL) {
				return 0;
			}
			else {
				boardToFill[i] = (int*)tempPTR;
			}
		}
	}

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			boardToFill[i][j] = 0;
		}
	}

	expectedSum = 0;
	for (i = 1; i <= boardSize; i++) {
		expectedSum += i;
	}

	autofillROWS(boardToFill, expectedSum);
	autofillCOLOUMS(boardToFill, expectedSum);
	autofillBLOCKS(boardToFill, expectedSum);
	autofillFILLCELLS(boardToFill);

	for (i = 0; i < boardSize; i++) {
		free(boardToFill[i]);
	}
	free(boardToFill);
	return 1;
}


void autofillROWS(int** boardToFill, int expectedSum) {
	int i;
	int j;
	int counter;
	int sum;
	int col;
	int numToFill;
	col = 0;/*initializing*/
	/* i stands for rows*/
	for (i = 0; i < boardSize; i++) {
		counter = 0;
		sum = 0;
		numToFill = 0;
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[i][j].currentCellvalue != -1) {
				counter += 1;
				sum += mainGameBoard[i][j].currentCellvalue;
			}
		}

		if (counter == boardSize - 1) {
			for (j = 0; j < boardSize; j++) {
				if (mainGameBoard[i][j].currentCellvalue == -1) {
					col = j;
				}
			}
			numToFill = expectedSum - sum;
			if (checkValidityOfNum(i, col, numToFill)) {
				boardToFill[i][col] = numToFill;
			}
		}
	}
}


void autofillCOLOUMS(int** boardToFill, int expectedSum) {
	int i;
	int j;
	int counter;
	int sum;
	int row;
	int numToFill;

	row = 0; /*initializing*/
	/* i stands for colums*/
	for (i = 0; i < boardSize; i++) {
		counter = 0;
		sum = 0;
		numToFill = 0;
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[j][i].currentCellvalue != -1) {
				counter += 1;
				sum += mainGameBoard[j][i].currentCellvalue;
			}
		}

		if (counter == boardSize - 1) {
			for (j = 0; j < boardSize; j++) {
				if (mainGameBoard[j][i].currentCellvalue == -1) {
					row = j;
					break;
				}
			}
			numToFill = expectedSum - sum;
			if (checkValidityOfNum(row, i, numToFill)) {
				boardToFill[row][i] = numToFill;
			}
		}
	}
}


void autofillBLOCKS(int** boardToFill, int expectetSum) {
	int i;
	int j;
	int counter;
	int number;
	int rowIndex;
	int colIndex;
	int flag;
	int rowBlockStart;
	int coloumnBlockStart;
	int sum;


	for (rowBlockStart = 0; rowBlockStart < boardSize; rowBlockStart += blockHeight) {

		for (coloumnBlockStart = 0; coloumnBlockStart < boardSize; coloumnBlockStart += blockWidth) {

			flag = 1;
			counter = 0;
			rowIndex = 0;
			colIndex = 0;
			sum = 0;

			/* checking each block*/
			for (i = rowBlockStart; i < rowBlockStart + blockHeight; i++) {
				for (j = coloumnBlockStart; j < coloumnBlockStart + blockWidth; j++) {

					if (mainGameBoard[i][j].currentCellvalue != -1) {
						counter += 1;
						sum += mainGameBoard[i][j].currentCellvalue;
					}
				}
			}

			/* if block has N-1 numbers */
			if (counter == boardSize - 1) {

				for (i = rowBlockStart; (i < rowBlockStart + blockHeight) && flag; i++) {
					for (j = coloumnBlockStart; (j < coloumnBlockStart + blockWidth) && flag; j++) {

						if (mainGameBoard[i][j].currentCellvalue == -1) {
							rowIndex = i;
							colIndex = j;
							flag = 0;
						}
					}
				}

				number = expectetSum - sum;

				if (checkValidityOfNum(rowIndex, colIndex, number)) {
					boardToFill[rowIndex][colIndex] = number;
				}
			}
		}
	}

}


void autofillFILLCELLS(int** boardToFill) {
	int i;
	int j;
	int numToFill;
	int counter;

	counter = 0;
	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (boardToFill[i][j] != 0) {
				numToFill = boardToFill[i][j];
				set(i, j, numToFill, 0);
				updateURListAfterSet(i, j, &mainGameBoard[i][j], 2);
				printf("Cell %d,%d set to %d\n", i + 1, j + 1, numToFill);
				counter += 1;
			}
		}
	}
	/* Marking the end of the autofill*/
	if (counter > 0) {
		insertNullNode();
	}
}













