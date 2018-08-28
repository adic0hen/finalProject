#include <stdlib.h>
#include <stdio.h>
#include "GamePlay.h"
#include "InGameCommands.h"

int game() {
	/*initialize buffer*/
	char* buffer;
	buffer = malloc(256*(sizeof(char))); /*the allocation size is the same as the max length of a valid input*/

	/*main game loop*/
	while (mode != 0 && mode != 4) {/*while mode is not "exit" and not "reset"*/
		printf("Enter your command:\n");
		buffer = scanf("%s", buffer);
		parse(buffer);
	}
	
}
