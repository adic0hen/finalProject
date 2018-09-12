#include <stdlib.h>
#include <stdio.h>
#include "GamePlay.h"
#include "InGameCommands.h"
#include "GameDataStructs.h"
#include "Parser.h"
#include "InitAndTerminateModule.h"

/*The GamePlay module encloses the outmost view of the sudoku game, Containing the parsing loop and general initialising
code lines. The only function, game(), is being called by main at the beginning of the program*/

/* Declaration of functions*/

int game();

/* ----------------------------------------------------------*/

/* ------------------ Code Part -----------------------------*/

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


