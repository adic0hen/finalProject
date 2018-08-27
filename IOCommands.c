#include <stdio.h>
#include <stdlib.h>
#include "InGameCommands.h"
#include "GamePlay.h"
#include "GameDataStructs.h"

FILE* fptr;

int save(char* path) {
	if (mode == 3) { /*in Edit mode*/
		if (isErroneous(mainGameBoard)) {
			printf("Error: board contains erroneous values\n")
		}
	}
	if(mode == 2){ /*in Solve mode*/
	}
	fptr = fopen(*path, "w");
	if (fptr == NULL) {/*when fopen fails, meaning that the file cannot be created or modified*/
		printf("Error: File cannot be created or modified\n");
	}


}