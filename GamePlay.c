#include <stdlib.h>
#include <stdio.h>
#include "GamePlay.h"
#include "InGameCommands.h"
#include "GameDataStructs.h"
#include "Parser.h"
#include "InitAndTerminateModule.h"

int game() {
	/*initialize buffer*/
	char* buffer;

	/* start game in init mode*/
	mode = INIT_MODE;
	markerrors = 1;

	buffer = malloc(256); 

	printf("Sudoku\n------\n");

	/*main game loop*/
	while (mode != EXIT_MODE) {/*while mode is not "exit"*/
		printf("Enter your command:\n");
		parse(buffer);
	}
	free(buffer);
	return 1;
}


