#ifndef GAMEDATASTRUCTS_H_
#define GAMEDATASTRUCTS_H_



typedef struct Cell {
	int currentCellvalue;
	int isFixed;
	int isErroneus;
} Cell;



/* Undo - Redo Nodes*/ 

typedef struct URNodeGuard {
	struct URNode* next;
	struct URNode* currentMove;
	struct Cell** initialBoard;
	int isInitBoardValid;
	int isEmpty;
} URNodeGuard;

typedef struct URNode {
	int type; 
	/* 0 - Generate, 1- set, 2- autfill) */
	int row;
	int col;
	struct cellNode* move;
	struct URNode* next;
	struct URNode* prev;
} URNode;

typedef struct cellNode {
	Cell* data;
	struct cellNode* next;
	struct cellNode* prev;
} cellNode;

typedef struct cellNodeGuard {
	cellNode* first;
} cellNodeGuard;


extern struct URNodeGuard UndoRedoList;
extern struct cellNodeGuard** LIFOCells;


extern struct Cell** mainGameBoard;
extern int boardSize;
extern int blockHeight;
extern int blockWidth;

extern int mode;
/* mode values and their meaning
1 = init
2 = solve
3 = edit
4 = restart
0 = exit
*/

extern int markerrors;









#endif
