#ifndef INGAMECOMMANDS_H_
#define INGAMECOMMANDS_H_

int autofill();
void redo();
int setMAIN(int row, int coloumn, int number);
void undoMAIN();
int isErroneous(Cell** mainGameBoard);

#endif