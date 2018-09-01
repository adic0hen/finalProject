#ifndef GAMEDATASTRUCTS_H_
#define GAMEDATASTRUCTS_H_

/*Board is internally represented as a 2D array of N*N pointers to cells, N = n*m */


typedef struct Cell {
	int currentCellvalue; /* the number the user has entered. -1 if empty. val==solVal if isFixed */
	int isFixed; /* 1 if the cell is fixed, 0 else */
	int isErroneus; /*1 if the cell is erroneus, 0 else*/
} Cell; /*Cell structure is reffered to as -Cell- */



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
/* mode values and their meaning:
1 = init
2 = solve
3 = edit
4 = restart
0 = exit
*/

extern int markerrors;









#endif
