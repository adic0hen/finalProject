#include <stdio.h>
#include <stdlib.h>
#include "Parser.h"
#include "GamePlay.h"
#include "GameDataStructs.h"
#include "InGameCommands.h"
#include "InitAndTerminateModule.h"



/*int main() {
	mode = 1;
	markerrors = 1; default value is 1 - "True"
	game();
} 
*/


/* main for tests*/ 

int main() {
	int i;
	initiallizeGameParameters(9, 3, 3);
	allocateMemForMainBoard();
	initializeMainBoard();
	printBoard(0);

	printBoard(0);

	autofill();

	printBoard(1);


}




