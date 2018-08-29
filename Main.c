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

int main(int argc, char *argv[]) {
	initiallizeGameParameters(9, 3, 3);
	allocateMemForMainBoard();
	initializeMainBoard();
	printBoard(0);
	mode = 1;
	markerrors = 1;
	game();


}




