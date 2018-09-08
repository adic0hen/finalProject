#include <stdio.h>
#include <stdlib.h>
#include "GameDataStructs.h"
#include "InGameCommands.h"




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

/* if clearFromGuard ==1, startNode is irrelevant, if clearFromGuard ==0, firstNodeToDelete = startNode.next */
void clearURListFromCurrentPosition(URNode* startNode, int clearFromGuard) {
	URNode* nodeToDelete;
	URNode* nextNodeToDelete;

	if (clearFromGuard) {
		nodeToDelete = UndoRedoList.next;
		if (nodeToDelete != NULL) {
			nextNodeToDelete = nodeToDelete->next;
			clearSingleURNode(nodeToDelete);
			nodeToDelete = nextNodeToDelete;
		}
		
	}
	else {
		nodeToDelete = startNode->next;
	}

	
	while (nodeToDelete != NULL) {
		nextNodeToDelete = nodeToDelete->next;
		clearSingleURNode(nodeToDelete);
		nodeToDelete = nextNodeToDelete;
	}

	if (clearFromGuard) {
		UndoRedoList.next = NULL;
		UndoRedoList.isEmpty = 1;
	}
	else {
		startNode->next = NULL;
	}

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



	/* allocate Mem and update fields for non-Null UR node */
	if (mode == 0 || mode == 1 || mode == 2)
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
	}

	/*---*/

	if (isFirst) {
		next->prev = NULL;
		UndoRedoList.next = next;
		UndoRedoList.isEmpty = 0;
	}
	else {
		next->prev = currPos;
		currPos->next = next;
	}

	next->type = mode;
	next->next = NULL;
	UndoRedoList.currentMove = next;

}



void updateURListAfterSet(int row, int coloumn, Cell* cell, int mode) {
	int isFirst;
	Cell* cloneCell;

	if (cell != NULL) {
		cloneCell = (Cell*)malloc(sizeof(Cell));
		cloneCell->currentCellvalue = cell->currentCellvalue;
		cloneCell->isErroneus = cell->isErroneus;
		cloneCell->isFixed = cell->isFixed;
	}
	else {
		cloneCell = NULL;
	}


	if (UndoRedoList.currentMove == NULL) {
		isFirst = 1;
	}
	else {
		isFirst = 0;
	}

	
	clearURListFromCurrentPosition(UndoRedoList.currentMove, isFirst);

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
		next->prev = NULL;
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


/* Undo commands*/

void printUndoUpdate(int row, int coloumn, char from, char to) {
	printf("Undo %d,%d: from %c to %c\n", row+1, coloumn+1, from, to);
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

	if (nextNode == NULL) {
		printUndoUpdate(row, coloumn, prevNumber + '0', '_');
	}
	else {
		if (prevNumber == -1) {
			printUndoUpdate(row, coloumn, '_', nextNode->data->currentCellvalue + '0');
		}
		else {
			printUndoUpdate(row, coloumn, prevNumber + '0', (nextNode->data->currentCellvalue) + '0');
		}
	}
}

void undoFromFirstNode() {
	URNode* firstNode;
	char tempChar;
	/* case of having a loaded board*/

	firstNode = UndoRedoList.currentMove;
	if (UndoRedoList.hasLoadedBoard == 1) {
		UndoRedoList.currentMove = NULL;
		UndoRedoList.selfCurrentMove = 1;
		updateMainBoardToFirstBoard();
		if (UndoRedoList.initialBoard[firstNode->row][firstNode->col].currentCellvalue == -1) {
			tempChar = '_';
		}
		else {
			tempChar = UndoRedoList.initialBoard[firstNode->row][firstNode->col].currentCellvalue + '0';
		}
	}
	else {
		/* Back to empty board*/
		UndoRedoList.currentMove = NULL;
		updateMainBoardToNone();
		tempChar = '_';
	}
	printUndoUpdate(firstNode->row, firstNode->col, firstNode->move->data->currentCellvalue + '0', tempChar);
}

void undoFromNullNode() {
	URNode* currentMove;
	int type;

	currentMove = UndoRedoList.currentMove;
	type = UndoRedoList.currentMove->prev->type;

	currentMove = currentMove->prev;
	UndoRedoList.currentMove = currentMove;

	while (currentMove->type == type) {
		if (currentMove->prev == NULL) {
			undoFromFirstNode();
		}
		else {
			undo();
			currentMove = UndoRedoList.currentMove; /*in undo current move goes one back*/
		}
	}

}

void undoMAIN() {
	URNode* currentMove;


	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to undo\n");
		return;
	}

	currentMove = UndoRedoList.currentMove;

	/* case of currentMove pointing on guard. anyway no moves to undo (either empty board or loaded board (which is first)) */
	if (currentMove == NULL) {
		printf("Error: no moves to undo\n");
		return;
	}

	/* case of undo from the first Node*/
	if (currentMove->prev == NULL) {
		undoFromFirstNode();
		return;
	}

	/* regular set*/
	if (currentMove->type == 1) {
		undo();
	}

	else if (currentMove->type == 2) {
		printf("TEST: ERROR IN UNDO REDO LIST"); /* For Test, need to delete!*/
	}
	else if (currentMove->type == 3) {
		undoFromNullNode();
	}

	updateErrStatAndCountEmptyCells();

}






/* Redo -related commands*/

void connectNodeToLIFOCell(int row, int coloumn, cellNode* cell) {
	cellNode* next;

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


void printRedoUpdate(int row, int coloumn, char from, char to) {
	printf("Redo %d,%d: from %c to %c\n", row+1, coloumn+1, from, to);
}

void redo() {
	URNode* nextNode;
	cellNode* prevLIFOCell;
	int row;
	int coloumn;
	int newNumber;

	nextNode = UndoRedoList.currentMove->next;

	UndoRedoList.currentMove = nextNode;
	row = nextNode->row;
	coloumn = nextNode->col;

	prevLIFOCell = LIFOCells[row][coloumn].first;

	connectNodeToLIFOCell(row, coloumn, nextNode->move);
	updateMainBoardAfterUndoRedo(row, coloumn);

	newNumber = LIFOCells[row][coloumn].first->data->currentCellvalue;

	if (prevLIFOCell == NULL) {
		/* case of empty cell - cell that never been filled*/
		printRedoUpdate(row, coloumn, '_', newNumber+'0');
		/* if new node is -1 than it should never been updated*/
	}
	else {
		if (newNumber == -1) {
			printRedoUpdate(row, coloumn, (prevLIFOCell->data->currentCellvalue) + '0', '_');
		}
		else {
			printRedoUpdate(row, coloumn, (prevLIFOCell->data->currentCellvalue) + '0',newNumber + '0' );
		}
	}

}

void redoUntilNullNode() {
	URNode* currentMove;
	int type;

	currentMove = UndoRedoList.currentMove->next;
	type = currentMove->type;

	while (currentMove->type == type) {
		redo();
		currentMove = UndoRedoList.currentMove->next;
	}

	if (currentMove->type == 3) {
		UndoRedoList.currentMove = currentMove;
	}
}

void redoFromGuard() {
	URNode* nextNode;
	int row;
	int coloumn;
	int newNumber;
	int prevNumber;
	int flag;

	flag = 0;

	if (UndoRedoList.hasLoadedBoard == 1) {
		if (UndoRedoList.selfCurrentMove == 0) {
			updateMainBoardToFirstBoard();
			UndoRedoList.selfCurrentMove = 1;
		}

		else {
			if (UndoRedoList.next != NULL) {
				flag = 1;
			}

			else {
				printf("Error: no moves to Redo\n");
			}
		}
	}

	else {
		if (UndoRedoList.next != NULL) {
			flag = 1;
		}
		else {
			printf("Error: no moves to Redo\n");
		}
	}

	if (flag) {
		nextNode = UndoRedoList.next;
		UndoRedoList.currentMove = nextNode;
		row = nextNode->row;
		coloumn = nextNode->col;
		if (UndoRedoList.hasLoadedBoard) {
			prevNumber = UndoRedoList.initialBoard[row][coloumn].currentCellvalue;
		}
		else {
			prevNumber = -1;
		}

		connectNodeToLIFOCell(row, coloumn, nextNode->move);
		updateMainBoardAfterUndoRedo(row, coloumn);

		newNumber = LIFOCells[row][coloumn].first->data->currentCellvalue;

		if (prevNumber != -1) {
			printf("Redo %d,%d: from %d to %d\n", row+1, coloumn+1, prevNumber, newNumber);
		}
		else {
			printf("Redo %d,%d: from _ to %d\n", row+1, coloumn+1, newNumber);
		}
	}
}

void redoMAIN() {
	URNode* currentMove;


	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to Redo\n");
		return;
	}

	currentMove = UndoRedoList.currentMove;

	if (currentMove == NULL)
	{
		redoFromGuard();
		return;
	}

	if (currentMove->next == NULL) {
		printf("Error: no moves to Redo\n");
		return;
	}

	if (currentMove->next->type == 1) {
		redo();
	}
	else if (currentMove->next->type == 2) {
		redoUntilNullNode();
	}
	else if (currentMove->next->type == 0) {
		redoUntilNullNode();
	}

	updateErrStatAndCountEmptyCells();
}







void insertNullNode() {
	updateURListAfterSet(-1, -1, NULL, 3);
}

/* Use to clone the first board - After load from file */
void updateURListAfterSolveAndEdit() {
	int i;
	int j;
	Cell** cloneFirstBoard;
	void* tempPTR;

	tempPTR = malloc(sizeof(Cell*) *boardSize);
	if (tempPTR == NULL) {
		return;
	}
	else {
		cloneFirstBoard = (Cell**)tempPTR;
	}

	for (i = 0; i < boardSize;i++) {
		tempPTR = malloc(sizeof(Cell) * boardSize);
		if (tempPTR == NULL) {
			return;
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
	UndoRedoList.isEmpty = 0;
	UndoRedoList.hasLoadedBoard = 1;
	UndoRedoList.selfCurrentMove = 1;

}



void updateURListAfterGenerate() {
	int i;
	int j;
	int counter;

	counter = 0;
	for (i = 0; i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if (mainGameBoard[i][j].currentCellvalue != -1) {
				counter += 1;
				updateURListAfterSet(i, j, &mainGameBoard[i][j], 0);
			}
		}
	}
	if (counter != 0) {
		insertNullNode();
	}
}



void freeURResources() {
	int i;
	int j;

	/* free list*/

	clearURListFromCurrentPosition(NULL, 1);

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			free(&UndoRedoList.initialBoard[i][j]);
		}
		free(&UndoRedoList.initialBoard[i]);
	}
	free(UndoRedoList.initialBoard);


	/* free LIFOCells*/


	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			LIFOCells[i][j].first = NULL;
			free(&LIFOCells[i][j]);
		}
		free(&LIFOCells[i]);
	}

	free(LIFOCells);

}

