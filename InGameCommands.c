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


int checkBlockForNumber(int row, int coloumn, int number) {
	int i;
	int j;
	int rowBlockStart;
	int coloumnBlockStart;

	rowBlockStart = row / blockHeight;
	coloumnBlockStart = coloumn / blockWidth;

	for (i = rowBlockStart; i < rowBlockStart + blockHeight; i++) {
		for (j = coloumnBlockStart; j < coloumnBlockStart + blockWidth; j++) {
			if (mainGameBoard[i][j].currentCellvalue == number) {
				return 1;
			}
		}
	}

	return 0;

}


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


	if (checkBlockForNumber(row,coloumn,number)) {
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
	mainGameBoard[row][coloumn].currentCellvalue = -1;

}


void set(int row, int coloumn, int number, int isErroneus) {

	if (isErroneus) {
		mainGameBoard[row][coloumn].currentCellvalue = number;
		mainGameBoard[row][coloumn].isErroneus = 1;
		return 1;
	}
	else {
		mainGameBoard[row][coloumn].currentCellvalue = number;
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
	int counter;
	int number;
	int rowIndex;
	int colIndex;
	int flag;
	int rowBlockStart;
	int coloumnBlockStart;
	int sum;


	for (rowBlockStart=0 ; rowBlockStart < boardSize; rowBlockStart+=blockHeight) {

		for (coloumnBlockStart; coloumnBlockStart < boardSize; coloumnBlockStart+=blockWidth) {

			flag = 1;
			counter = 0;
			rowIndex = 0;
			colIndex = 0;

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



void clearSingleLIFOCell(int row, int coloumn, cellNode* nodeToDelete) {
	cellNode* next;
	cellNode* prev;

	next = nodeToDelete->next;
	prev = nodeToDelete->prev;

	if (prev == NULL && next == NULL) {
		LIFOCells[row][coloumn].first = NULL;
	}
	else if (prev == NULL) {
		LIFOCells[row][coloumn].first = next;
	}
	else if (next == NULL) {
		prev->next = NULL;
	}
	else {
		prev->next = next;
		next->prev = prev;
	}

	free(nodeToDelete->data);
	free(nodeToDelete);
}

void clearSingleURNode(URNode* nodeToDelete) {
	URNode* prev;
	URNode* next;
	cellNode* current;
	int row;
	int coloumn;


	current = nodeToDelete->move;
	row = nodeToDelete->row;
	coloumn = nodeToDelete->col;
	clearSingleLIFOCell(row, coloumn, current);


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



/* clear all the list from current position (not including the startNode, to clear all list, use clearList() */
void clearURListFromCurrentPosition(URNode* startNode) {
	URNode* current;
	URNode* next;

	current = startNode;
	while ((next = current->next) != NULL) {
		current = next;
		clearSingleURNode(current);
	}

	startNode->next = NULL;
	
}


/* mode: 0- generate, 1-Regular set, 2 - autofill */
void insertURListAfterSET(int row, int coloumn, Cell* cell, int mode, int isFirst) {
	URNode* next;
	void* tempPTR;
	URNode* currPos;
	cellNode* cellNodeToEnter;
	cellNode* temp;

	if (!isFirst) {
		currPos = UndoRedoList.currentMove;
	}

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
	next->type = mode;
	next->next = NULL;
	if (isFirst) {
		next->prev = NULL;
		UndoRedoList.next = next;
		UndoRedoList.isEmpty = 0;
	}
	else {
		next->prev = currPos;
		currPos->next = next;
	}

	UndoRedoList.currentMove = currPos;

	

}


void updateURListAfterSet(int row, int coloumn, Cell* cell, int mode) {
	int isFirst;


	if (UndoRedoList.isEmpty) {
		isFirst = 1;
	}
	else {
		isFirst = 0;
	}

	if (!UndoRedoList.isEmpty) {
		clearURListFromCurrentPosition(UndoRedoList.currentMove);
	}

	insertURListAfterSET(row, coloumn, cell, mode, isFirst);
}


void updateMainBoardAfterUndo(int row, int coloumn) {
	Cell* cellToEnter;


	cellToEnter = LIFOCells[row][coloumn].first->data;

	if (cellToEnter == NULL) {
		mainGameBoard[row][coloumn].currentCellvalue = -1;
		mainGameBoard[row][coloumn].isErroneus = 0;
		mainGameBoard[row][coloumn].isFixed = 0;
	}
	else {
		mainGameBoard[row][coloumn] = *cellToEnter;
	}

}


void disconnectNodeFromLIFOCell(int row, int coloumn, cellNode* cell) {
	cellNode* next;
	cellNode* prev;

	next = cell->next;
	prev = cell->prev;

	if ((next == NULL) && (prev == NULL)) {
		LIFOCells[row][coloumn].first = NULL;
	}
	else if (next == NULL) {
		prev->next = NULL;
	}
	else if (prev == NULL) {
		LIFOCells[row][coloumn].first = next;
	}
	else {
		prev->next = next;
		next->prev = prev;
	}

}


void undo() {
	URNode* currentMove;
	int prevNumber;
	int newNumber;


	currentMove = UndoRedoList.currentMove;
	prevNumber = currentMove->move->data->currentCellvalue;

	disconnectNodeFromLIFOCell(currentMove->row, currentMove->col, currentMove->move);

	updateMainBoardAfterUndo(currentMove->row, currentMove->col);

	UndoRedoList.currentMove = currentMove->prev;
	
	newNumber = UndoRedoList.currentMove->move->data->currentCellvalue;

	if (newNumber != -1) {
		printf("Undo %d,%d: from %d to %d\n", currentMove->row, currentMove->col, prevNumber, newNumber);
	}
	else {
		printf("Undo %d,%d: from %d to _\n", currentMove->row, currentMove->col, prevNumber);
	}
}


void undoMAIN() {
	URNode* currentMove;

	currentMove = UndoRedoList.currentMove;
	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to undo\n");
	}
	if (currentMove->prev == NULL) {
		UndoRedoList.currentMove = NULL;
	}

	if (currentMove->type == 1) {
		undo();
	}
	else if (currentMove->type == 2) {
		while (currentMove->type == 2) {
			currentMove = currentMove->prev;
			undo();
		}
	}

}








