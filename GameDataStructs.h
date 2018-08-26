#ifndef GAMEDATASTRUCTS_H_
#define GAMEDATASTRUCTS_H_

/*Board is internally represented as an array of N*N pointers to cells, N = n*m */


typedef struct Cell {
	int currentCellvalue; /* the number the user has entered. -1 if empty. val==solVal if isFixed */
	int isFixed; /* 1 if the cell is fixed, 0 else */
	int isErroneus;
} Cell; /*Cell structure is reffered to as -cell- */


typedef struct Block {
	int* numbersList;
	int isAlmostFull;
} Block;


/* Undo - Redo Nodes*/ 

typedef struct URNodeGuard {
	struct URNode* next;
	struct URNode* currentMove;
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


struct URNodeGuard UndoRedoList;
struct cellNodeGuard** LIFOCells;





extern struct Cell** mainGameBoard;
extern struct Block** mainBlockBoard;


extern int boardSize;
extern int blockHeight;
extern int blockWidth;
extern int numOfBlockBoardRows;
extern int numOfBlockBoardColoums;

#define MAXNUM = boardSize;






#endif
