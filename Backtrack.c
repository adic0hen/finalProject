#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"





typedef struct BTCell {
	int val;
	int isStatic;
} BTCell;


typedef struct stackNode {
	struct stackNode* next;
	struct stackNode* prev;
	int number;
	int row;
	int coloumn;
} stackNode;


typedef struct stackGuard {
	struct stackNode* first;
	int maxSize;
	int currentSize;
} stackGuard;


BTCell** BTboard;
stackGuard stack;
int numberOfSolutions;

/* For test*/ 

void printBoard_Test() {
	int i;
	int j;

	for (i = 0; i < boardSize; i++) {
		for (j = 0; j < boardSize; j++) {
			printf("%2d  ", BTboard[i][j].val);
		}
		printf("\n");
	}
}

/* before backtrack*/
void allocateMemForBTBoard() {
	int i;
	BTCell** address;
	void* tempPTR;


	tempPTR = malloc(sizeof(BTCell*) * boardSize);
	if (tempPTR == NULL) {
		return;
	}
	else {
		address = (BTCell**)tempPTR;
	}

	for (i = 0; i < boardSize;i++) {
		tempPTR = malloc(sizeof(BTCell)*boardSize);
		if (tempPTR == NULL) {
			return;
		}
		else {
			address[i] = (BTCell*)tempPTR;
		}
	}

	BTboard = address;

}

void copyMainBoardToBTboard() {
	int i;
	int j;
	int tempNum;

	for (i = 0;i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if ((tempNum = mainGameBoard[i][j].currentCellvalue) != -1) {
				BTboard[i][j].val = tempNum;
				BTboard[i][j].isStatic = 1;
			}
			else {
				BTboard[i][j].val = -1;
				BTboard[i][j].isStatic = 0;
			}
		}
	}

}

/* --- */




/* for backtrack*/
void setToBTboard(int row, int coloumn, int number) {
	BTboard[row][coloumn].val = number;
}


int checkBTBlockForNumber(int row, int coloumn, int number) {
	int i;
	int j;
	int rowBlockStart;
	int coloumnBlockStart;

	rowBlockStart = row / blockHeight;
	coloumnBlockStart = coloumn / blockWidth;

	for (i = (rowBlockStart*blockHeight); i < (rowBlockStart+1)*blockHeight; i++) {
		for (j = (coloumnBlockStart*blockWidth); j < (coloumnBlockStart+1)*blockWidth; j++) {
			if (BTboard[i][j].val == number) {
				return 1;
			}
		}
	}

	return 0;

}


int checkBTValidityOfNum(int row, int coloumn, int number) {
	int i;
	int j;


	if (checkBTBlockForNumber(row, coloumn, number)) {
		return 0;
	}

	for (i = 0; i < boardSize; i++) {
		if (BTboard[row][i].val == number) {
			return 0;
		}
	}

	for (i = 0; i < boardSize; i++) {
		if (BTboard[i][coloumn].val == number) {
			return 0;
		}
	}

	return 1;
}


void pushToStack(int row, int coloum, int number) {
	stackNode* newNode;


	newNode = malloc(sizeof(stackNode));
	if (newNode == NULL) {
		return;
	}

	newNode->row = row;
	newNode->coloumn = coloum;
	newNode->number = number; 
	newNode->next = stack.first;
	newNode->prev = NULL;
	if (stack.first != NULL) {
		stack.first->prev = newNode;
	}
	stack.first = newNode;

	stack.currentSize += 1;

	/* for test*/ 
	printf("\n\n");
	printf("%d  \n\n", stack.first->number);
	/**/

}


stackNode* popFromStack() {
	stackNode* nodeToDelete;


	nodeToDelete = stack.first;
	stack.first = nodeToDelete->next;
	nodeToDelete->next = NULL;
	stack.first->prev = NULL;

	stack.currentSize -= 1;

	return nodeToDelete;

}

/* ------ */



void backtrack() {
	int number;
	int row;
	int coloumn;
	stackNode* currNode;

	number = 1;
	row = 0;

	while (row < boardSize)
	{

		coloumn = 0;
		printBoard_Test();
		while (coloumn < boardSize)
		{

			if (BTboard[row][coloumn].isStatic) {
				coloumn += 1;
			}


			else
			{
				if (number <= boardSize) {

					if (checkBTValidityOfNum(row, coloumn, number)) {
						setToBTboard(row, coloumn, number);
						pushToStack(row, coloumn, number);
						coloumn += 1;
						number = 1;
					}

					else {
						number += 1;
					}
				}


				if (number == boardSize + 1) 
				{

					do 
					{
						if ((coloumn == 0) && (row == 0)) {
							numberOfSolutions = 0;
							return;
						}
						else if (coloumn == 0) {
							coloumn = boardSize - 1;
							row -= 1;
						}
						else {
							coloumn -= 1;
						}
					} while (BTboard[row][coloumn].isStatic);
							
				

					currNode = popFromStack();
					setToBTboard(row, coloumn, -1);
					number = currNode->number + 1;
					/* For Test*/
					if (row != currNode->row || coloumn != currNode->coloumn) {
						printf("TEST__Unmatch row or coloumn");
					}
					/**/

				}

			}

		}

		row += 1;
		number = 1;
	}
}




int numOfSolutions() {
	allocateMemForBTBoard();
	copyMainBoardToBTboard();
	backtrack();

	/* Need to free boards and stack*/

	return numberOfSolutions;
}


void BT_TEST() {
	allocateMemForBTBoard();
	copyMainBoardToBTboard();
}