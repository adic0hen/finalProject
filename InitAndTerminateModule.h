#ifndef INITANDTERMINATEMODULE_H_
#define INITANDTERMINATEMODULE_H_
#include <stdio.h>

int allocateMemForMainBoard();
void initializeLoadedMainBoard(FILE *fptr);
void initializeMainBoard();
void initiallizeGameParameters(int N, int bHeight, int bWidth);
int allocateMemForLIFOCellsAndOutputBoard();
void initialUndoRedoListAndLIFOCells();
void freeAll();
void freeMat(int** mat);
void exitGame();
void reset();
void initAllDefault();
void initAllFromFile(int blockHeight, int blockWidth);
void memoryError();

#endif
