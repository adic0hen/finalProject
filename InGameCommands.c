#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"
#include "GamePlay.h"


/* Declaration of functions*/

void autofillROWS(int** boardToFill, int expectedSum);
void autofillCOLOUMS(int** boardToFill, int expectedSum);
void autofillBLOCKS(int** boardToFill, int expectedSum);
void autofillFILLCELLS(int** boardToFill);
void updateMainBoardAfterUndoRedo(int row, int coloumn);
int isErroneous();
int markErrors(int mark);
void updateURListAfterSet(int row, int coloumn, Cell* cell, int mode);



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


void printBoard(int markErrors) {
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
						printf("*");
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
	int j;


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


	if (number > boardSize || number < 0) {
		return 0; /*Error of Invalid Number*/
	}
	if (row > boardSize || coloumn > boardSize) {
		return 0;
	}
	if (row < 1 || coloumn < 1) {
		return 0;
	}


	if (number == 0) {
		if (mainGameBoard[row - 1][coloumn - 1].isFixed) {
			return 2; /*Error of clearing a fixed cell*/
		}
		else {
			deleteCell(row - 1, coloumn - 1);
		}
	}

	isValidNumber = checkValidityOfNum(number, row - 1, coloumn - 1);

	if (!isValidNumber) {
		set(row - 1, coloumn - 1, number, 1);
	}
	else {
		set(row - 1, coloumn - 1, number, 0);
	}

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

	if (mode == 2) {
		if (mark == 0 || mark == 1) {
			markerrors = mark;
		}
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

/*~~~~~~~~NEED  TO TEST THIS FUNCTION~~~~~*/
int generate(char *sCellsToFill, char *sCellsToKeep, int cellsToFill, int cellsToKeep) {
	int i;
	i = 0;
	if (!checkGenerateParameters(sCellsToFill, sCellsToKeep, cellsToFill, cellsToKeep)) {/*if the parameters are not valid*/
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
	return 1;
}

/*Sub function for generate*/
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

/*~~~~~~~~NEED  TO TEST THIS FUNCTION~~~~~*/
int hint(int row, int col) {
	int h;
	if (checkHintParameters(row, col)) {
		return 0;
	}
	h = hintSolve(row -1, col-1); /*returns the hint value if board is solvable, 0 otherwise*/
	if (h == 0) {
		return 0;
	}
	printf("Hint, set cell to %d\n",h);
}
int checkHintParameters(int row, int col) {
	if (row<1 || row>boardSize || col<1 || col>boardSize) {
		printf("Error: value not in range 1-%d\n", boardSize);
		return 0;
	}
	if (isErroneous) {
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


	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			if (boardToFill[i][j] != 0) {
				numToFill = boardToFill[i][j];
				set(i, j, numToFill, 0);
				updateURListAfterSet(i, j, &mainGameBoard[i][j], 2);
				printf("Cell %d,%d set to %d\n", i + 1, j + 1, numToFill);
			}
		}
	}
	/* Marking the end of the autofill*/
	updateURListAfterSet(-1, -1, NULL, 3);
}






/* ---------------UNDO-REDO commands----------------- */



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
	else {
		currPos = NULL;
	}

	/* allocate Mem for UR Node, need anyway*/
	tempPTR = malloc(sizeof(URNode));
	if (tempPTR == NULL) {
		return;
	}
	else {
		next = (URNode*)tempPTR;
	}
	/* --- */



	/* allocate Mem and update fields for non-guard UR node */
	if (mode == 1 || mode == 2)
	{
		tempPTR = malloc(sizeof(cellNode));
		if (tempPTR == NULL) {
			return;
		}
		else {
			cellNodeToEnter = (cellNode*)tempPTR;
		}

		/* enter to LIFOCells */
		temp = LIFOCells[row][coloumn].first;
		cellNodeToEnter->data = cell;
		cellNodeToEnter->next = temp;
		cellNodeToEnter->prev = NULL;
		if (temp != NULL) {
			temp->prev = cellNodeToEnter;
		}
		LIFOCells[row][coloumn].first = cellNodeToEnter;
		/* --- */


		next->row = row;
		next->col = coloumn;
		next->move = cellNodeToEnter;
		if (isFirst) {
			next->prev = NULL;
			UndoRedoList.next = next;
			UndoRedoList.isEmpty = 0;
		}
		else {
			next->prev = currPos;
			currPos->next = next;
		}
	}
	/* --- */

	next->type = mode;
	next->next = NULL;
	UndoRedoList.currentMove = next;

}



void updateURListAfterSet(int row, int coloumn, Cell* cell, int mode) {
	int isFirst;
	Cell* cloneCell;

	if (cell != NULL) {
		cloneCell = (Cell*)malloc(sizeof(cell));
		cloneCell->currentCellvalue = cell->currentCellvalue;
		cloneCell->isErroneus = cell->isErroneus;
		cloneCell->isFixed = cell->isFixed;
	}
	else {
		cloneCell = NULL;
	}


	if (UndoRedoList.isEmpty) {
		isFirst = 1;
		UndoRedoList.isEmpty = 0;
	}
	else {
		isFirst = 0;
	}

	if (!isFirst) {
		clearURListFromCurrentPosition(UndoRedoList.currentMove);
	}

	insertURListAfterSET(row, coloumn, cloneCell, mode, isFirst);
}



void updateMainBoardAfterUndoRedo(int row, int coloumn) {
	Cell* cellToEnter;


	if (LIFOCells[row][coloumn].first == NULL) {
		mainGameBoard[row][coloumn].currentCellvalue = -1;
		mainGameBoard[row][coloumn].isErroneus = 0;
		mainGameBoard[row][coloumn].isFixed = 0;
	}
	else {
		cellToEnter = LIFOCells[row][coloumn].first->data;
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

	cell->next = NULL;
	cell->prev = NULL;

}


void updateMainBoardToNone() {
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


void updateMainBoardToFirstBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			mainGameBoard[i][j].currentCellvalue = (UndoRedoList.initialBoard[i][j]).currentCellvalue;
			mainGameBoard[i][j].isFixed = (UndoRedoList.initialBoard[i][j]).isFixed;
			mainGameBoard[i][j].isErroneus = (UndoRedoList.initialBoard[i][j]).isErroneus;
		}
	}
}


void undo() {
	URNode* currentMove;
	cellNode* nextNode;
	int prevNumber;
	int row;
	int coloumn;

	currentMove = UndoRedoList.currentMove;
	prevNumber = currentMove->move->data->currentCellvalue;
	row = currentMove->row;
	coloumn = currentMove->col;

	disconnectNodeFromLIFOCell(row, coloumn, currentMove->move);

	updateMainBoardAfterUndoRedo(row, coloumn);

	UndoRedoList.currentMove = currentMove->prev;

	nextNode = LIFOCells[row][coloumn].first;

	if (nextNode != NULL) {
		printf("Undo %d,%d: from %d to %d\n", row, coloumn, prevNumber, nextNode->data->currentCellvalue);
	}
	else {
		printf("Undo %d,%d: from %d to _\n", row, coloumn, prevNumber);
	}
}


void undoMAIN() {
	URNode* currentMove;


	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to undo\n");
		return;
	}

	currentMove = UndoRedoList.currentMove;

	if (currentMove->prev == NULL) {
		if (currentMove->type == 0) {
			/* case of encounter a generate/load from file guard */
			UndoRedoList.currentMove = NULL;
			updateMainBoardToNone();
		}
		else {
			UndoRedoList.currentMove = NULL;
		}
		return;
	}

	if (currentMove->type == 1) {
		undo();
	}
	else if (currentMove->type == 2) {
		while (currentMove->type == 2) {
			undo();
		}
	}
	else if (currentMove->type == 3) {
		UndoRedoList.currentMove = currentMove->prev;
		while (currentMove->type == 2) {
			undo();
		}
	}

}



void connectNodeToLIFOCell(int row, int coloumn, cellNode* cell) {
	cellNode* next;
	cellNode* prev;

	if (LIFOCells[row][coloumn].first == NULL) {
		LIFOCells[row][coloumn].first = cell;
	}
	else {
		next = LIFOCells[row][coloumn].first;
		LIFOCells[row][coloumn].first = cell;
		cell->next = next;
		(cell->next)->prev = cell;
		cell->prev = NULL;
	}
}


void redo() {
	URNode* currentNode;
	URNode* nextNode;
	cellNode* prevNode;
	int row;
	int coloumn;
	int newNumber;

	nextNode = UndoRedoList.currentMove->next;

	UndoRedoList.currentMove = nextNode;
	row = nextNode->row;
	coloumn = nextNode->col;

	prevNode = LIFOCells[row][coloumn].first;

	connectNodeToLIFOCell(row, coloumn, nextNode->move);
	updateMainBoardAfterUndoRedo(row, coloumn);

	newNumber = LIFOCells[row][coloumn].first->data->currentCellvalue;

	UndoRedoList.currentMove = nextNode;

	if (prevNode != NULL) {
		printf("Redo %d,%d: from %d to %d\n", row, coloumn, prevNode->data->currentCellvalue, newNumber);
	}
	else {
		printf("Redo %d,%d: from _ to %d\n", row, coloumn, newNumber);
	}
}


void redoMAIN() {
	URNode* currentMove;

	currentMove = UndoRedoList.currentMove;
	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to Redo\n");
		return;
	}

	else if (currentMove == NULL) {
		if (UndoRedoList.isInitBoardValid) {
			updateMainBoardToFirstBoard();
		}
		UndoRedoList.currentMove = UndoRedoList.next;
	}
	else if (currentMove->next == NULL) {
		printf("Error: no moves to Redo\n");
	}
	else {
		if (currentMove->type == 1) {
			redo();
		}
		else if (currentMove->type == 2) {
			while (currentMove->type == 2) {
				redo();
			}
		}
		else if (currentMove->type == 3) {
			if (UndoRedoList.currentMove->next == NULL) {
			}
			else {
				UndoRedoList.currentMove = currentMove->next;
			}
		}
		else {}
		/* NEED TO HANDLE CASES OF GENERATE AND END OF AUTOFILL*/

	}


}


/* Use to clone the first board - After load from file and generate */
void updateURListAfterFirstBoard() {
	int i;
	int j;
	Cell** cloneFirstBoard;
	void* tempPTR;

	tempPTR = malloc(sizeof(Cell*) *boardSize);
	if (tempPTR == NULL) {
		return 0;
	}
	else {
		cloneFirstBoard = (Cell**)tempPTR;
	}

	for (i = 0; i < boardSize;i++) {
		tempPTR = malloc(sizeof(Cell) * boardSize);
		if (tempPTR == NULL) {
			return 0;
		}
		else {
			cloneFirstBoard[i] = (Cell*)tempPTR;
		}
	}

	for (i = 0; i < boardSize;i++) {
		for (j = 0; j < boardSize; j++) {
			cloneFirstBoard[i][j].currentCellvalue = mainGameBoard[i][j].currentCellvalue;
			cloneFirstBoard[i][j].isErroneus = mainGameBoard[i][j].isErroneus;
			cloneFirstBoard[i][j].isFixed = mainGameBoard[i][j].isFixed;
		}
	}

	UndoRedoList.initialBoard = cloneFirstBoard;
	updateURListAfterSet(-1, -1, NULL, 0);
	UndoRedoList.isEmpty = 0;
	UndoRedoList.isInitBoardValid = 1;

}
















