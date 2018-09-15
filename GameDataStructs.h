#ifndef GAMEDATASTRUCTS_H_
#define GAMEDATASTRUCTS_H_


/* This module concentrates all the Data structers that being used in several modules*/


/* Main Game Board Cell struct*/
typedef struct Cell {
	int currentCellvalue;
	int isFixed;
	int isErroneus;
} Cell;



/* Undo - Redo Nodes*/ 

/* Guard (Sentinel) of the Undo Redo list*/
typedef struct URNodeGuard {
	struct URNode* next;
	struct URNode* currentMove;
	struct Cell** initialBoard;
	int hasLoadedBoard; /* 0 means the board is default*/
	int isEmpty;
} URNodeGuard;

/* An Undo-Redo Node*/
typedef struct URNode {
	int type; 
	/* 0 - Generate, 1- set, 2- autfill) */
	int row;
	int col;
	struct cellNode* move;
	struct URNode* next;
	struct URNode* prev;
} URNode;

#define GENERATE_SET 0
#define REG_SET 1
#define AUTOFILL_SET 2
#define NULL_NODE 3

/* a stack Node for LIFO-cells*/
typedef struct cellNode {
	Cell* data;
	struct cellNode* next;
	struct cellNode* prev;
} cellNode;

typedef struct cellNodeGuard {
	cellNode* first;
	
} cellNodeGuard;

typedef struct URupdateCell {
	int prevNumber;
	int newNumber;
	int isUpdated;
} URupdateCell;


extern struct URupdateCell** URoutputBoard;
extern struct URNodeGuard UndoRedoList;
/* The LIFOCells is a matrix of stack, each stack represents the corresponding cell in the mainBoard, and
saves the history of each cell*/
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

#define EXIT_MODE 0
#define INIT_MODE 1
#define SOLVE_MODE 2
#define EDIT_MODE 3
#define RESTART_MODE 4


extern int markerrors;









#endif
