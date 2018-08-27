	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "GamePlay.h"

int parse(char* buffer) {
	char* cmd;
	/*these variables will be used as part of the "set" command*/
	char* sRow;
	char* sCol;
	char* sVal;
	int row;
	int col;
	int val;
	/*this variable will be used as part of the "solve" or "edit" command*/
	char* filePath;

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
				}
				break;
			}
			if (strcmp(cmd, "solve") == 0) {
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) {
					mode = 2;
					/*need to complete code!*/
				}
				break;
			}

			if (strcmp(cmd, "edit") == 0) {
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) {
					mode = 3;
					/*need to complete code!*/
				}

				break;
			}
			if (strcmp(cmd, "mark_errors") == 0) {
				markerrors = atoi(strtok(NULL, " \t\r\n"));
				break;
			}
			if (strcmp(cmd, "print_board") == 0) {
				printBoard(markerrors);
				break;
			}
		}
	}
}
