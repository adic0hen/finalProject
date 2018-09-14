#include <stdio.h>
#include <stdlib.h>
#include "GameDataStructs.h"
#include "InGameCommands.h"
#include "InitAndTerminateModule.h"


/* clears single cell from the stack-board*/
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

/* clearing a single Undo-Redo Node*/
void clearSingleURNode(URNode* nodeToDelete) {
	URNode* prev;
	URNode* next;
	cellNode* current;
	int row;
	int coloumn;


	current = nodeToDelete->move;
	row = nodeToDelete->row;
	coloumn = nodeToDelete->col;
	if (nodeToDelete->type != NULL_NODE) {
		clearSingleLIFOCell(row, coloumn, current);
	}




	prev = nodeToDelete->prev;
	next = nodeToDelete->next;

	if (prev == NULL && next == NULL) {
		UndoRedoList.next = NULL;
		UndoRedoList.currentMove = NULL;
	}

	else if (prev == NULL) {
		UndoRedoList.next = next;
		next->prev = NULL;
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
		memoryError();
		return;
	}
	else {
		next = (URNode*)tempPTR;
	}
	/* --- */



	/* allocate Mem and update fields for non-Null UR node */
	if (mode == GENERATE_SET || mode == REG_SET || mode == AUTOFILL_SET)
	{
		tempPTR = malloc(sizeof(cellNode));
		if (tempPTR == NULL) {
			memoryError();
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


/* managing the insertion of a UR- node after any kind of set, includes clonning the cell, delete the list and insert*/
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


/* disconnect a node from the cell-stack, without deleting it*/
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

void printUndoUpdate() {
	char* tempStr;
	char* tstr;
	int i;
	int j;
	int prevNum;
	int newNum;

	for (i = 0;i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if (URoutputBoard[i][j].isUpdated) {
				prevNum = URoutputBoard[i][j].prevNumber;
				newNum = URoutputBoard[i][j].newNumber;


				if (prevNum == -1) {
					tempStr = (char*)malloc(3 * sizeof(char));
					sprintf(tempStr, "%d", newNum);
					printf("Undo %d,%d: from %c to %s\n", j + 1, i + 1, '_', tempStr);
				}
				else {
					if (newNum == -1) {
						tempStr = (char*)malloc(3 * sizeof(char));
						sprintf(tempStr, "%d", prevNum);
						printf("Undo %d,%d: from %s to %c\n", j + 1, i + 1, tempStr, '_');
					}
					else {
						tempStr = (char*)malloc(3 * sizeof(char));
						sprintf(tempStr, "%d", prevNum);
						tstr = (char*)malloc(3 * sizeof(char));
						sprintf(tstr, "%d", newNum);
						printf("Undo %d,%d: from %s to %s\n", j + 1, i + 1, tempStr, tstr);
					}
				}
				free(tempStr);
				free(tstr);
			}
		}
	}
}

void updateOutputBoard(int row, int coloumn, int prevNum, int newNum) {
	URoutputBoard[row][coloumn].newNumber = newNum;
	URoutputBoard[row][coloumn].prevNumber = prevNum;
	URoutputBoard[row][coloumn].isUpdated = 1;
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
		updateOutputBoard(row, coloumn, prevNumber, -1);
	}
	else {
		updateOutputBoard(row, coloumn, prevNumber, nextNode->data->currentCellvalue);
	}
}

void undoFromFirstNode() {
	URNode* firstNode;
	int tempChar;
	int row;
	int coloumn;

	
	firstNode = UndoRedoList.currentMove;
	UndoRedoList.currentMove = NULL;
	row = firstNode->row;
	coloumn = firstNode->col;
	

	if (UndoRedoList.initialBoard[row][coloumn].currentCellvalue == -1) {
		tempChar = -1;
	}
	else {
		tempChar = UndoRedoList.initialBoard[row][coloumn].currentCellvalue;
	}
	
	updateMainBoardToFirstBoard();

	updateOutputBoard(row, coloumn, firstNode->move->data->currentCellvalue, tempChar);
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
			break;
		}
		else {
			undo();
			currentMove = UndoRedoList.currentMove; /*In undo current move goes one back*/
		}
	}

}

void initOutputBoard() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			URoutputBoard[i][j].newNumber = -1;
			URoutputBoard[i][j].prevNumber = -1;
			URoutputBoard[i][j].isUpdated = 0;
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

	initOutputBoard();

	/* case of undo from the first Node*/
	if (currentMove->prev == NULL) {
		undoFromFirstNode();
		printBoard();
		printUndoUpdate();
		return;
	}

	/* regular set*/
	if (currentMove->type == REG_SET) {
		undo();
	}

	else if (currentMove->type == AUTOFILL_SET) {
	}

	else if (currentMove->type == NULL_NODE) {
		undoFromNullNode();
	}

	updateErrStatAndCountEmptyCells();
	printBoard();
	printUndoUpdate();

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


void printRedoUpdates() {
	char* tempStr;
	char* tstr;
	int i;
	int j;
	int prevNum;
	int newNum;

	for (i = 0; i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if (URoutputBoard[i][j].isUpdated) {
				prevNum = URoutputBoard[i][j].prevNumber;
				newNum = URoutputBoard[i][j].newNumber;

				if (prevNum == -1) {
					tempStr = (char*)malloc(3 * sizeof(char));
					sprintf(tempStr, "%d", newNum);
					printf("Redo %d,%d: from %c to %s\n", j + 1, i + 1, '_', tempStr);
				}
				else {
					if (newNum == -1) {
						tempStr = (char*)malloc(3 * sizeof(char));
						sprintf(tempStr, "%d", prevNum);
						printf("Redo %d,%d: from %s to %c\n", j + 1, i + 1, tempStr, '_');
					}
					else {
						tempStr = (char*)malloc(3 * sizeof(char));
						sprintf(tempStr, "%d", prevNum);
						tstr = (char*)malloc(3 * sizeof(char));
						sprintf(tstr, "%d", newNum);
						printf("Redo %d,%d: from %s to %s\n", j + 1, i + 1, tempStr, tstr);
					}
				}
				free(tempStr);
				free(tstr);
			}
		}
	}
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
		updateOutputBoard(row, coloumn, -1, newNumber);
		/* if new node is -1 than it should never been updated*/
	}
	else {
		if (newNumber == -1) {
			updateOutputBoard(row, coloumn, prevLIFOCell->data->currentCellvalue , -1);
		}
		else {
			updateOutputBoard(row, coloumn, prevLIFOCell->data->currentCellvalue ,newNumber );
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

	if (currentMove->type == NULL_NODE) {
		UndoRedoList.currentMove = currentMove;
	}
}

void redoFromGuard() {
	URNode* nextNode;
	int row;
	int coloumn;
	int prevNum;
	int NextNum;

	nextNode = UndoRedoList.next;
	row = nextNode->row;
	coloumn = nextNode->col;
	if (UndoRedoList.initialBoard[row][coloumn].currentCellvalue == -1) {
		prevNum = -1;
	}
	else {
		prevNum = UndoRedoList.initialBoard[row][coloumn].currentCellvalue ;
	}
	NextNum = nextNode->move->data->currentCellvalue;

	if (nextNode->type == GENERATE_SET || nextNode->type == AUTOFILL_SET) {
		connectNodeToLIFOCell(row, coloumn, nextNode->move);
		updateMainBoardAfterUndoRedo(row, coloumn);
		updateOutputBoard(row, coloumn, prevNum, NextNum);
		UndoRedoList.currentMove = nextNode;
		redoUntilNullNode();
	}

	else {
		UndoRedoList.currentMove = nextNode;
		row = nextNode->row;
		coloumn = nextNode->col;
		connectNodeToLIFOCell(row, coloumn, nextNode->move);
		updateMainBoardAfterUndoRedo(row, coloumn);
		updateOutputBoard(row, coloumn, prevNum, NextNum);
	}
}

void redoMAIN() {
	URNode* currentMove;


	if (UndoRedoList.isEmpty) {
		printf("Error: no moves to Redo\n");
		return;
	}

	currentMove = UndoRedoList.currentMove;

	initOutputBoard();

	if (currentMove == NULL)
	{
		redoFromGuard();
		printBoard();
		printRedoUpdates();
		return;
	}

	if (currentMove->next == NULL) {
		printf("Error: no moves to Redo\n");
		return;
	}

	if (currentMove->next->type == REG_SET) {
		redo();
	}
	else if (currentMove->next->type == AUTOFILL_SET) {
		redoUntilNullNode();
	}
	else if (currentMove->next->type == GENERATE_SET) {
		redoUntilNullNode();
	}

	updateErrStatAndCountEmptyCells();
	printBoard();
	printRedoUpdates();
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
	int counter;

	counter = 0;
	tempPTR = malloc(sizeof(Cell*) *boardSize);
	if (tempPTR == NULL) {
		memoryError();
		return;
	}
	else {
		cloneFirstBoard = (Cell**)tempPTR;
	}

	for (i = 0; i < boardSize;i++) {
		tempPTR = malloc(sizeof(Cell) * boardSize);
		if (tempPTR == NULL) {
			memoryError();
			return;
		}
		else {
			cloneFirstBoard[i] = (Cell*)tempPTR;
		}
	}

	for (i = 0; i < boardSize;i++) {
		for (j = 0; j < boardSize; j++) {
			if (mainGameBoard[i][j].currentCellvalue == -1) {
				counter += 1;
			}
			cloneFirstBoard[i][j].currentCellvalue = mainGameBoard[i][j].currentCellvalue;
			cloneFirstBoard[i][j].isErroneus = mainGameBoard[i][j].isErroneus;
			cloneFirstBoard[i][j].isFixed = mainGameBoard[i][j].isFixed;
		}
	}

	UndoRedoList.initialBoard = cloneFirstBoard;
	UndoRedoList.isEmpty = 1; /* 1 beacuse that means that no URNodes is in the list */
	if (counter != 0) {
		UndoRedoList.hasLoadedBoard = 1;
	}

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

	/* free list*/

	clearURListFromCurrentPosition(NULL, 1);

	for (i = 0; i < boardSize; i++) {
		free(UndoRedoList.initialBoard[i]);
	}

	free(UndoRedoList.initialBoard);

	/* free LIFOCells*/

	for (i = 0; i < boardSize; i++) {
		free(LIFOCells[i]);
	}

	free(LIFOCells);

	/* free outputBoard*/

	for (i = 0; i < boardSize; i++) {
		free(URoutputBoard[i]);
	}

	free(URoutputBoard);
}

