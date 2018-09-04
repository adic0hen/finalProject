	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "GamePlay.h"
	#include "InGameCommands.h"
	#include "InitAndTerminateModule.h"
	#include "IOCommands.h"
	#include "GameDataStructs.h"
void invalid(char* cmd);
int parse(char* buffer) {
	char* cmd;
	/*these variables will be used as part of the "set","hint" commands*/
	char* sRow;
	char* sCol;
	char* sVal;
	int row;
	int col;
	int val;
	/*this variable will be used as part of the "solve","edit","save" commands*/
	char* filePath;
	/*these variables will be used as part of the "generate" command*/
	char* sCellsToFill;
	char* sCellsToKeep;
	int cellsToFill;
	int cellsToKeep;
	/*this variable will be used as part of the "mark_errors" command*/
	int mark;

	if (fgets(buffer, 256, stdin) != NULL) {
		cmd = strtok(buffer, " \t\r\n");
		while (cmd != NULL) {
			if (strcmp(cmd, "set") == 0) {
				if (mode == 2 || mode == 3) { /*command available only in solve or edit modes*/
					sRow = strtok(NULL, " \t\r\n");
					sCol = strtok(NULL, " \t\r\n");
					sVal = strtok(NULL, " \t\r\n");
					if (sCol != NULL && sRow != NULL && sVal != NULL) { /*making sure we have all the values*/
						col = atoi(sCol);
						row = atoi(sRow);
						val = atoi(sVal);
						setMAIN(row, col, val);
					}
					break;
				}
				else {
					invalid(cmd); /*this will result in getting to the invalid command part*/
					break;
				}
			}
			if (strcmp(cmd, "solve") == 0) {
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) {
					mode = 2;
					load(filePath);
				}
				break;
			}
			if (strcmp(cmd, "edit") == 0) {
				mode = 3;
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) {
					load(filePath);
				}
				else { /*initializing an empty board because there was no file path given*/
					initializeMainBoard();
				}
				break;
			}
			if (strcmp(cmd, "mark_errors") == 0) {
				int mark;
				mark = atoi(strtok(NULL, " \t\r\n"));
				
				if (markErrors(mark)) {/*this is True when we are in Solve mode*/
					break;
				}
				else {
					invalid(cmd);/*this will result in getting to the invalid command part*/
					break;
				}
			}
			if (strcmp(cmd, "print_board") == 0) {
				if (mode == 2 || mode == 3) { /*command available only in solve or edit modes*/
					printBoard(markerrors);
					break;
				}
				else {
					invalid(cmd); /*this will result in getting to the invalid command part*/
					break;
				}
			}
			if (strcmp(cmd, "validate") == 0) {
				/*need to connect the hintSolve to a normal hint function*/
				break;
			}
			if (strcmp(cmd, "generate") == 0) {
				if (mode != 3) {
					invalid(cmd);
					break;
				}
				sCellsToFill = strtok(NULL, " \t\r\n");
				sCellsToKeep = strtok(NULL, " \t\r\n");
				if (sCellsToFill != NULL || sCellsToKeep != NULL) {
					cellsToFill = atoi(sCellsToFill);
					cellsToKeep = atoi(sCellsToKeep);
					generate(sCellsToFill, sCellsToKeep, cellsToFill, cellsToKeep);
				}
				break;
			}
			if (strcmp(cmd, "undo") == 0) {
				undoMAIN();
				/*need to complete code!*/
				break;
			}
			if (strcmp(cmd, "redo") == 0) {
				/*need to complete code!*/
				break;
			}
			if (strcmp(cmd, "save") == 0) {
				filePath = strtok(NULL, " \t\r\n");
				save(filePath);
				break;
			}
			if (strcmp(cmd, "hint") == 0) {
				if (mode != 2) {
					invalid(cmd);
					break;
				}
				sRow = strtok(NULL, " \t\r\n");
				sCol = strtok(NULL, " \t\r\n");
				if (sRow != NULL && sCol != NULL) {
					row = atoi(sRow);
					col = atoi(sCol);
					hint(row, col);
				}
				break;
			}
			if (strcmp(cmd, "num_solutions") == 0) {
				/*need to complete code!*/
				break;
			}
			if (strcmp(cmd, "autofill") == 0) {
				if (mode != 2) {/*need to complete code! making sure it is in solve mode*/
				}
				autofill();
				/*need to complete code!*/
				break;
			}
			if (strcmp(cmd, "reset") == 0) {
				mode = 4;
				break;
			}
			if (strcmp(cmd, "exit") == 0) {
				mode = 0;
				break;
			}
			/*Invalid command part - we get to this part of the parser only if there was an invalid or unrecognized command*/
			invalid(cmd);
			break;
		}
	}
}

void invalid(char* cmd) {
		printf("Error: invalid command\n");
		cmd = NULL;
	}
