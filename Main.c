#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "GamePlay.h"
#include "GameDataStructs.h"
#include "InGameCommands.h"
#include "InitAndTerminateModule.h"
#include "IOCommands.h"
#include "Backtrack.h"



/*
int main(int argc, char* argv[]) {
	printf("hi");
	return 0;
}
*/



/* main for tests*/ 

int main(int argc, char *argv[]) {
	initiallizeGameParameters(9, 3, 3);
	allocateMemForMainBoard();
	initializeMainBoard();
	printBoard(0);


	numOfSolutions();
}





