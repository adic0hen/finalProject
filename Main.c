#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "GamePlay.h"
#include "GameDataStructs.h"
#include "InGameCommands.h"
#include "InitAndTerminateModule.h"
#include "IOCommands.h"
#include "Backtrack.h"
#include <time.h>

void TESTRandomalyFillBoard(int numOfTries);

/*
int main(int argc, char* argv[]) {
	printf("hi");
	return 0;
}
*/



/* main for tests*/ 

/*int main(int argc, char *argv[]) {*/
int main(){
	srand(time(NULL));
	game();
	return 1;
}



void TESTRandomalyFillBoard(int numOfTries) {
	int row;
	int coloumn;
	int counter;
	int counterOfFull;
	int number;

	counter = 0;
	counterOfFull = 0;

	while (counter < numOfTries && counterOfFull < (boardSize*boardSize)) {
		row = rand() % boardSize;
		coloumn = rand() % boardSize;
		number = rand() % boardSize + 1;

		if (checkValidityOfNum(number, row, coloumn)) {
			set(row, coloumn, number, 0);
			counterOfFull += 1;
		}

		counter += 1;
	}
}

/*
void URTest(int numOfsets,int numToDelete, int numOfAF, int numOfUndo,int numOfRedo) {
	int row;
	int coloumn;
	int c1;
	int c2;
	int c3;
	int c4;
	int c5;
	int counterOfFull;
	int number;

	counter = 0;
	counterOfFull = 0;

	while (counter < numOfTries && counterOfFull < (boardSize*boardSize)) {
		row = rand() % boardSize;
		coloumn = rand() % boardSize;
		number = rand() % boardSize + 1;

		if (checkValidityOfNum(number, row, coloumn)) {
			set(row, coloumn, number, 0);
			counterOfFull += 1;
		}

		counter += 1;
	}
}
*/