#ifndef INITANDTERMINATEMODULE_H_
#define INITANDTERMINATEMODULE_H_
#include <stdio.h>

int allocateMemForMainBoard();
void initializeLoadedMainBoard(FILE *fptr);
void initializeMainBoard();
void initiallizeGameParameters(int N, int bHeight, int bWidth);
int allocateMemForLIFOCells();
void initialUndoRedoListAndLIFOCells();
void freeAll();
void freeMat(int** mat);
#endif
