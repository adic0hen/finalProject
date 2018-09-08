#ifndef INGAMECOMMANDS_H_
#define INGAMECOMMANDS_H_

int autofill();
void redo();
int setMAIN(int row, int coloumn, int number);
void undoMAIN();
int isErroneous();
int isEmpty();
int markErrors(int mark);
void printBoard();
void set(int row, int coloumn, int number, int isErroneous);
void undoMAIN();
void redoMAIN();
int checkValidityOfNum(int number, int row, int coloumn);
int checkGenerateParameters(char*sCellsToFill, char*sCellsToKeep, int cellsToFill, int cellsToKeep);
int validate();
int generate(char *sCellsToFill, char *sCellsToKeep, int cellsToFill, int cellsToKeep);
int hint(int row, int col);
int updateErrStatAndCountEmptyCells();


#endif