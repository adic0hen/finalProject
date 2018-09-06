#ifndef UNDOREDOCOMMANDS_H_
#define UNDOREDOCOMMANDS_H_
#include "GameDataStructs.h"


void undoMAIN();
void redoMAIN();
void updateURListAfterSolveAndEdit();
void updateURListAfterGenerate();
void updateURListAfterSet(int row, int coloumn, Cell* cell, int mode);
void insertNullNode();
void freeURResources();
void clearURListFromCurrentPosition(URNode* startNode, int clearFromGuard);
void updateMainBoardToFirstBoard();




#endif

