#include <stdlib.h>
#include <stdio.h>
#include "GameDataStructs.h"
#include "InitAndTerminateModule.h"

/*The Backtrack module, all commands that implement the exhaustive backtrack algorithm are written here.
The commands managing the stack that is used to simulate the recursion is also implemented here.
The algorithm uses a copy of the game board on which the values are being set and solutions are examined*/


/*data structures to simulate recursion for the exhaustive backtrack algorithm*/
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

/* Declaration of functions*/

void allocateMemForBTBoard();
void copyMainBoardToBTboard();
void setToBTboard(int row, int coloumn, int number);
int checkBTBlockForNumber(int row, int coloumn, int number);
int checkBTValidityOfNum(int row, int coloumn, int number);
void pushToStack(int row, int coloum, int number);
stackNode* popFromStack();
void backtrack();
void freeResources();
void numOfSolutions();

/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/

/* before backtrack*/
void allocateMemForBTBoard() {
	int i;
	BTCell** address;
	void* tempPTR;


	tempPTR = malloc(sizeof(BTCell*) * boardSize);
	if (tempPTR == NULL) {
		memoryError();
		return;
	}
	else {
		address = (BTCell**)tempPTR;
	}

	for (i = 0; i < boardSize;i++) {
		tempPTR = malloc(sizeof(BTCell)*boardSize);
		if (tempPTR == NULL) {
			memoryError();
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

	stack.maxSize = 0;
	for (i = 0;i < boardSize;i++) {
		for (j = 0; j < boardSize;j++) {
			if ((tempNum = mainGameBoard[i][j].currentCellvalue) != -1) {
				BTboard[i][j].val = tempNum;
				BTboard[i][j].isStatic = 1;
			}
			else {
				BTboard[i][j].val = -1;
				BTboard[i][j].isStatic = 0;
				stack.maxSize += 1;
			}
		}
	}

}

/* ------ */

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
		memoryError();
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

}


stackNode* popFromStack() {
	stackNode* nodeToDelete;


	nodeToDelete = stack.first;
	stack.first = nodeToDelete->next;
	nodeToDelete->next = NULL;

	if (stack.first != NULL) {
		stack.first->prev = NULL;
	}

	stack.currentSize -= 1;

	return nodeToDelete;

}

/* ------ */

void backtrack() {
	int number;
	int row;
	int coloumn;
	stackNode* currNode;
	int goBack;

	goBack = 0;
	number = 1;
	row = 0;
	numberOfSolutions = 0;

	while (row < boardSize)
	{

		coloumn = 0;
		
		while (coloumn < boardSize)
		{

			if (BTboard[row][coloumn].isStatic) {
				coloumn += 1;
			}


			else
			{
				if (number <= boardSize) 
				{

					if (checkBTValidityOfNum(row, coloumn, number)) {
						setToBTboard(row, coloumn, number);
						pushToStack(row, coloumn, number);
						coloumn += 1;
						number = 1;
						if (stack.currentSize == stack.maxSize) {
							numberOfSolutions += 1;
							goBack = 1;
							coloumn -= 1;
						}
					}

					else {
						number += 1;
					}
				}


				if ((number == boardSize + 1) || goBack)
				{
					
					if (!goBack) 
					{

						do
						{
							if ((coloumn == 0) && (row == 0)) {
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
					}
							
					goBack = 0;

					currNode = popFromStack();
					setToBTboard(row, coloumn, -1);
					number = currNode->number + 1;
					free(currNode);
				}

			}

		}

		row += 1;
		number = 1;
	}
}


void freeResources() {
	int i;

	for (i = 0; i < boardSize;i++) {
		free(BTboard[i]);
	}

	free(BTboard);

}
	

void numOfSolutions() {
	allocateMemForBTBoard();
	copyMainBoardToBTboard();
	backtrack();
	printf("Number of solutions: %d\n", numberOfSolutions);
	if (numberOfSolutions == 1) {
		printf("This is a good board!\n");
	}
	else if (numberOfSolutions > 1) {
		printf("The puzzle has more than 1 solution, try to edit it further\n");
	}
	else {}

	freeResources();


	return;
}

