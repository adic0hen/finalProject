#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int parse(char* buffer) {
	char* cmd;
	/*these parameters will be used as part of the "set" command*/
	char* sRow;
	char* sCol;
	char* sVal;
	int row;
	int col;
	int val;

	if (fgets(buffer, 1024, stdin) != NULL) {
		cmd = strtok(buffer, " \t\r\n");
		while (cmd != NULL) {
			if (strcmp(cmd, "command") == 0) { /*general command comparison of the parser, to be copied and replaced with relevant command names*/


				break;
			}
			if (strcmp(cmd, "set") == 0) {
				sRow = strtok(NULL, " \t\r\n");
				sCol = strtok(NULL, " \t\r\n");
				sVal = strtok(NULL, " \t\r\n");
				if (sCol != NULL && sRow != NULL && sVal != NULL) { /*making sure we have all the values*/
					col = atoi(sCol) - 1;
					row = atoi(sRow) - 1;
					val = atoi(sVal);
					setMain(row, col, val);
				break;
			}
		}
	}

}
