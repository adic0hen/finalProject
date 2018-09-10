	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include "GamePlay.h"
	#include "InGameCommands.h"
	#include "InitAndTerminateModule.h"
	#include "IOCommands.h"
	#include "GameDataStructs.h"
#include "UndoRedoCommands.h"
#include "Solver.h"
#include "Backtrack.h"


void invalid();
 

int parse(char* buffer)
{

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

	if (fgets(buffer, 256, stdin) != NULL) 
	{
		cmd = strtok(buffer, " \t\r\n");
		while (cmd != NULL) 
		{
			if (strcmp(cmd, "set") == 0) 
			{
				if (mode == 2 || mode == 3) 
				{ /*command available only in solve or edit modes*/
					sRow = strtok(NULL, " \t\r\n");
					printf("sRow - %s\n", sRow);/*for testing*/
					sCol = strtok(NULL, " \t\r\n");
					printf("sCol - %s\n", sCol);/*for testing*/
					sVal = strtok(NULL, " \t\r\n");
					printf("sVal - %s\n", sVal);/*for testing*/
					if (sCol != NULL && sRow != NULL && sVal != NULL) 
					{ /*making sure we have all the values*/
						col = atoi(sCol);
						row = atoi(sRow);
						val = atoi(sVal);
						if (val == 0 && strcmp(sVal, "0") != 0) { /*this makes sure that we erase only when 0 is entered*/
							printf("Error: value not in range 0-%d\n", boardSize);
							break;
						}
						printf("col -%d, row -%d, val %d\n", col, row, val);/*for testing*/
						setMAIN(row, col, val);
					}
					else {
						invalid();
					}
					break;
				}
				else {
					invalid(); /*this will result in getting to the invalid command part*/
					break;
				}
			}

			if (strcmp(cmd, "solve") == 0) 
			{
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) 
				{
					mode = 2;
					load(filePath);
					updateURListAfterSolveAndEdit();
					printBoard();
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "edit") == 0) 
			{
				mode = 3;
				markerrors = 1;
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) 
				{
					load(filePath);
				}
				else
				{ /*initializing an empty board because there was no file path given*/
					freeAll();
					initAllDefault();
				}
				updateURListAfterSolveAndEdit();
				printBoard();/*markerrors is always 1 in edit mode*/
				break;
			}

			if (strcmp(cmd, "mark_errors") == 0) 
			{
				int mark;
				char* sMark;
				sMark = strtok(NULL, " \t\r\n");
				if (sMark == NULL) {
					invalid();
					break;
				}
				mark = atoi(sMark);				
				if (mode == 2) {
					markErrors(mark);
					break;
				}
				else 
				{
					invalid();/*this will result in getting to the invalid command part*/
					break;
				}
			}

			if (strcmp(cmd, "print_board") == 0) 
			{
				if (mode == 2 || mode==3) 
				{ /*command available only in solve or edit modes*/
					printBoard();
					break;
				}
				else
				{
					invalid(); /*this will result in getting to the invalid command part*/
					break;
				}
			}

			if (strcmp(cmd, "validate") == 0) 
			{
				if (mode == 2 || mode == 3) 
				{ /*command available only in solve or edit modes*/
					validate(0);
					break;
				}
			}

			if (strcmp(cmd, "generate") == 0) 
			{
				if (mode != 3) 
				{
					invalid();
					break;
				}

				sCellsToFill = strtok(NULL, " \t\r\n");
				sCellsToKeep = strtok(NULL, " \t\r\n");
				if (sCellsToFill != NULL && sCellsToKeep != NULL) 
				{
					cellsToFill = atoi(sCellsToFill);
					cellsToKeep = atoi(sCellsToKeep);
					generate(sCellsToFill, sCellsToKeep, cellsToFill, cellsToKeep);
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "undo") == 0)
			{
				if (mode == 2 || mode == 3) {
					undoMAIN();
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "redo") == 0)
			{
				if (mode == 2 || mode == 3) {
					redoMAIN();
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "save") == 0)
			{
				filePath = strtok(NULL, " \t\r\n");
				save(filePath);
				break;
			}

			if (strcmp(cmd, "hint") == 0)
			{
				printf("in parser\n");/*for testing!*/
				if (mode != 2) 
				{
					invalid();
					break;
				}
				sRow = strtok(NULL, " \t\r\n");
				sCol = strtok(NULL, " \t\r\n");
				if (sRow != NULL && sCol != NULL)
				{
					row = atoi(sRow);
					col = atoi(sCol);
					hint(row, col);
				}
				break;
			}

			if (strcmp(cmd, "num_solutions") == 0) 
			{
				if (mode == 2 || mode == 3) {
					if (isErroneous()) {
						printf("Error: board contains erroneous values\n");
					}
					else {
						numOfSolutions();
					}
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "autofill") == 0) 
			{
				if (mode != 2) 
				{
					invalid();
					break;
				}
				if (isErroneous()) {
					printf("Error: board contains erroneous values\n");
				}
				else {
					autofill();
					printBoard();
				}
				break;
			}

			if (strcmp(cmd, "reset") == 0) 
			{
				if (mode != 2 && mode != 3) 
				{/*only available in solve or edit modes*/
					invalid();
					break;
				}
				reset();
				break;
			}

			if (strcmp(cmd, "exit") == 0) 
			{
				mode = 0;
				exitGame();
				break;
			}
			/*Invalid command part - we get to this part of the parser only if there was an invalid or unrecognized command*/
			invalid();
			break;
		}
	}

	else { /*when a command longer than 256 characters has been entered*/
		printf("ERROR: invalid command\n");
	}
	return 1;
}


void invalid() {
		printf("ERROR: invalid command\n");
	}
