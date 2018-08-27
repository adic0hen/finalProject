#include <stdlib.h>
#include <stdio.h>
#include "GamePlay.h"

int game() {
	char* buffer;
	buffer = malloc(1024);
	while (mode != 0 || mode != 4) {
		buffer = scanf("%s", buffer);
		parse(buffer);
	}
}
