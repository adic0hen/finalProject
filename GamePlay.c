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
	mode = 1;
	markerrors = 1;

	buffer = malloc(256); /*the allocation size is the same as the max length of a valid input*/

	/*main game loop*/
	while (mode != 0) {/*while mode is not "exit", if mode == reset than reset() is called and game starts over*/
		printf("Enter your command:\n");
		parse(buffer);
	}
	free(buffer);
	return 1;
}


