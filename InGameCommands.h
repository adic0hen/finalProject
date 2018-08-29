#ifndef INGAMECOMMANDS_H_
#define INGAMECOMMANDS_H_

int autofill();
void redo();
int setMAIN(int row, int coloumn, int number);
void undoMAIN();
int isErroneous();
int markErrors(int mark);
void printBoard(int markErrors);
void set(int row, int coloumn, int number, int isErroneous);
void undoMAIN();
void redoMAIN();

#endif