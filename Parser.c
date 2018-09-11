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
	/*These variables will be used as part of the "set","hint" commands*/
	char* sRow;
	char* sCol;
	char* sVal;
	int row;
	int col;
	int val;
	/*This variable will be used as part of the "solve","edit","save" commands*/
	char* filePath;
	/*These variables will be used as part of the "generate" command*/
	char* sCellsToFill;
	char* sCellsToKeep;
	int cellsToFill;
	int cellsToKeep;

	if (fgets(buffer, 256, stdin) != NULL) 
	{
		cmd = strtok(buffer, " \t\r\n");
		while (cmd != NULL) 
		{
			if (strcmp(cmd, "set") == 0) 
			{
				if (mode == SOLVE_MODE || mode == EDIT_MODE) 
				{ /*command available only in solve or edit modes*/
					sRow = strtok(NULL, " \t\r\n");
					sCol = strtok(NULL, " \t\r\n");
					sVal = strtok(NULL, " \t\r\n");
					if (sCol != NULL && sRow != NULL && sVal != NULL) 
					{ /*Making sure we have all the values*/
						col = atoi(sCol);
						row = atoi(sRow);
						val = atoi(sVal);
						if (val == 0 && strcmp(sVal, "0") != 0) { /*Makes sure that we erase only when "0" is entered*/
							printf("Error: value not in range 0-%d\n", boardSize);
							break;
						}
						setMAIN(row, col, val);
					}
					else {
						invalid();
					}
					break;
				}
				else {
					invalid();
					break;
				}
			}

			if (strcmp(cmd, "solve") == 0) 
			{
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) 
				{
					mode = SOLVE_MODE;
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
				mode = EDIT_MODE;
				markerrors = 1;
				filePath = strtok(NULL, " \t\r\n");
				if (filePath != NULL) 
				{
					load(filePath);
				}
				else
				{ /*In case that no file path was given*/
					freeAll();
					initAllDefault();
				}
				updateURListAfterSolveAndEdit();
				printBoard();
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
				if (mode == SOLVE_MODE) {
					markErrors(mark);
					break;
				}
				else 
				{
					invalid();
					break;
				}
			}

			if (strcmp(cmd, "print_board") == 0) 
			{
				if (mode == SOLVE_MODE || mode==EDIT_MODE) 
				{ /*command available only in solve or edit modes*/
					printBoard();
					break;
				}
				else
				{
					invalid();
					break;
				}
			}

			if (strcmp(cmd, "validate") == 0) 
			{
				if (mode == SOLVE_MODE || mode == EDIT_MODE) 
				{ /*command available only in solve or edit modes*/
					validate(0);
					break;
				}
			}

			if (strcmp(cmd, "generate") == 0) 
			{
				if (mode != EDIT_MODE) 
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
				if (mode == SOLVE_MODE || mode == EDIT_MODE) {
					undoMAIN();
				}
				else {
					invalid();
				}
				break;
			}

			if (strcmp(cmd, "redo") == 0)
			{
				if (mode == SOLVE_MODE || mode == EDIT_MODE) {
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
				if (mode != SOLVE_MODE) 
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
				if (mode == SOLVE_MODE || mode == EDIT_MODE) {
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
				if (mode != SOLVE_MODE) 
				{
					invalid();
					break;
				}
				if (isErroneous()) {
					printf("Error: board contains erroneous values\n");
				}
				else {
					autofill();
				}
				break;
			}

			if (strcmp(cmd, "reset") == 0) 
			{
				if (mode != SOLVE_MODE && mode != EDIT_MODE) 
				{/*only available in solve or edit modes*/
					invalid();
					break;
				}
				reset();
				break;
			}

			if (strcmp(cmd, "exit") == 0) 
			{
				mode = EXIT_MODE;
				exitGame();
				break;
			}
			/*------*/
			/*Invalid command part - we get to this part of the parser only if there was an invalid or unrecognized command*/
			invalid();
			break;
		}
	}

	else {
		invalid();
	}
	return 1;
}


void invalid() {
		printf("ERROR: invalid command\n");
	}
