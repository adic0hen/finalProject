#include <stdlib.h>
#include <stdio.h>
#include "GamePlay.h"

int game() {
	/*initialize buffer*/
	char* buffer;
	buffer = malloc(1024);
	/*main game loop*/
	while (mode != 0 || mode != 4) {
		buffer = scanf("%s", buffer);
		parse(buffer);
	}
	
}
