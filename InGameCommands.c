#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"


/* Declaration of functions*/

void autofillROWS(int** boardToFill, int expectedSum);
void autofillCOLOUMS(int** boardToFill,int expectedSum);
void autofillBLOCKS(int** boardToFill, int expectedSum);
void autofillFILLCELLS(int** boardToFill);



/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/


void printBoard(int markErrors) {
	int i;
	int j;
	int rowCounter;
	int numberOfBlocks;

	numberOfBlocks = boardSize / blockWidth;
	rowCounter = 0;
	while (rowCounter < boardSize)
	{

		/*print dashes*/
		if (rowCounter % blockHeight == 0)
		{
			for (i = 0; i < (4 * boardSize + blockWidth + 1);i++) {
				printf("-");
			}
		}

		/* for each row of numbers*/
		if (rowCounter % blockHeight != 0)
		{

			/* for each block*/
			for (i = 0; i < numberOfBlocks; i++)
			{
				printf("|");

				/* for each cell in the block, per row*/
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


void set(int row, int coloumn, int number, int isErroneus) {
	int blockRow;
	int blockCol;
	
	blockRow = row / (boardSize / blockHeight);
	blockCol = coloumn / (boardSize / blockWidth);

	
	if (isErroneus) {
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


int setMAIN(int row, int coloumn, int number) {
	int isValidNumber;


	if (number > boardSize || number < 0) {
		return 0; /*Error of Invalid Number*/
	}
	if (row >= boardSize || coloumn >= boardSize) {
		return 0;
	}

	if (number == 0) {
		if (mainGameBoard[row][coloumn].isFixed) {
			return 2; /*Error of clearing a fixed cell*/
		}
		else {
			deleteCell(row, coloumn, number);
			return 1;
		}
	}

	isValidNumber = checkValidityOfNum(number, row, coloumn);

	if (!isValidNumber) {
		set(row, coloumn, number, 1);
	}
	else {
		set(row, coloumn, number, 0);
	}


}



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

	autofillROWS(&boardToFill ,expectedSum);
	autofillCOLOUMS(&boardToFill, expectedSum);
	autofillBLOCKS(&boardToFill, expectedSum);
	autofillFILLCELLS(&boardToFill);

	for (i = 0; i < boardSize; i++) {
		free(boardToFill[i]);
	}
	free(boardToFill);

}


void autofillROWS(int** boardToFill, int expectedSum) {
	int i;
	int j;
	int counter;
	int sum;
	int col;
	int numToFill;

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
					break;
				}
			}
			numToFill = expectedSum - sum;
			if (checkValidityOfNum(i, col, numToFill)) {
				boardToFill[i][col] = numToFill;
			}
		}
	}
}


void autofillCOLOUMS(int** boardToFill,int expectedSum){
	int i;
	int j;
	int counter;
	int sum;
	int row;
	int numToFill;

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
	int k;
	int l;
	int m;
	int counter;
	int number;
	int rowStart;
	int colStart;
	int rowIndex;
	int colIndex;
	int flag;


	for (i = 0; i < numOfBlockBoardRows; i++) {

		for (j = 0; j < numOfBlockBoardColoums; j++) {
			flag = 1;
			counter = 0;
			rowIndex = 0;
			rowStart = 0;
			colIndex = 0;
			colStart = 0;

			/* checking if block has N-1 numbers*/
			for (k = 0; k < boardSize; k++) {
				if (mainBlockBoard[i][j].numbersList[k] != 0) {
					counter += 1;
				}
			}

			/* if block has N-1 number then do*/
			if (counter == boardSize - 1) {
				/*checking which number*/
				for (k = 0; k < boardSize; k++) {
					if (mainBlockBoard[i][j].numbersList[k] == 0) {
						number = k + 1;
						break;
					}
				}

				/*updating the row start and coloumn start of the block*/
				rowStart = i / numOfBlockBoardRows;
				colStart = j / numOfBlockBoardColoums;


				/* find the index of the row and coloumn */
				for (l = rowStart; l < (rowStart + blockHeight) && flag; l++) {
					for (m = colStart; m < (colStart + blockWidth) && flag; m++) {
						if (mainGameBoard[l][m].currentCellvalue == -1) {
							rowIndex = l;
							colIndex = m;
							flag = 0;
						}
					}
				}

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


	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (boardToFill[i][j] != 0) {
				numToFill = boardToFill[i][j];
				set(i, j, numToFill, 0);
				printf("Cell %d,%d set to %d\n", i + 1, j + 1, numToFill);
			}
		}
	}
}





// UNDO-REDO commands//
// Need to save the state of the board after these commands: 
//set,  autofill(as one),  generate 


void clearSingleURNode(URNode* nodeToDelete) {
	URNode* prev;
	URNode* next;
	cellNode* prevC;
	cellNode* nextC;
	cellNode* current;
	int row;
	int coloumn;


	current = nodeToDelete->move;
	prevC = current->prev;
	nextC = current->next;
	if (prevC == NULL && nextC == NULL) {
		row = nodeToDelete->row;
		coloumn = nodeToDelete->col;
		LIFOCells[row][coloumn].first = NULL;
	}
	else if (prevC == NULL) {
		row = nodeToDelete->row;
		coloumn = nodeToDelete->col;
		LIFOCells[row][coloumn].first = nextC;
	}
	else if (nextC == NULL) {
		prevC->next = NULL;
	}
	else {
		prevC->next = nextC;
		nextC->prev = prev;
	}

	free(current->data);
	free(current);


	prev = nodeToDelete->prev;
	next = nodeToDelete->next;

	if (prev == NULL && next == NULL) {
		UndoRedoList.next = NULL;
		UndoRedoList.currentMove = NULL;
	}

	else if (prev == NULL) {
		UndoRedoList.next = next;
	}

	else if (next == NULL) {
		prev->next = NULL;
	}
	else {
		prev->next = next;
		next->prev = prev;
	}


	free(nodeToDelete);
}




void clearURListFromCurrentPosition(URNode* startNode) {
	int i;
	URNode* next;
}
void updateURListAfterRegSET(int row, int coloumn, Cell* cell) {
	URNode* next;
	void* tempPTR;
	URNode* currPos;
	cellNode* cellNodeToEnter;
	cellNode* temp;

	currPos = UndoRedoList.currentMove;

	tempPTR = malloc(sizeof(URNode));
	if (tempPTR == NULL) {
		return;
	}
	else {
		next = (URNode*)tempPTR;
	}

	tempPTR = malloc(sizeof(cellNode));
	if (tempPTR == NULL) {
		return;
	}
	else {
		cellNodeToEnter = (cellNode*)tempPTR;
	}

	temp = LIFOCells[row][coloumn].first; 
	cellNodeToEnter->data = cell;
	cellNodeToEnter->next = temp;
	cellNodeToEnter->prev = NULL;
	temp->prev = cellNodeToEnter;
	LIFOCells[row][coloumn].first = cellNodeToEnter;

	next->row = row;
	next->col = coloumn;
	next->move = cellNodeToEnter;
	next->prev = currPos;
	next->next = NULL;
	next->type = 1;
	currPos->next = next;

}





