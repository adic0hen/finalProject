#ifndef GAMEDATASTRUCTS_H_
#define GAMEDATASTRUCTS_H_

/*Board is internally represented as an array of N*N pointers to cells, N = n*m */


typedef struct Cell {
	int currentCellvalue; /* the number the user has entered. -1 if empty. val==solVal if isFixed */
	int isFixed; /* 1 if the cell is fixed, 0 else */
	int isErroneus;
} Cell; /*Cell structure is reffered to as -cell- */

typedef struct node {
	Cell* data;
	struct node* next;
	struct node* prev;
} Node;

typedef struct Block {
	int* numbersList;
} Block;


extern struct Cell** mainGameBoard;
extern struct Block** mainBlockBoard;


extern int boardSize;
extern int blockHeight;
extern int blockWidth;

#define MAXNUM = boardSize;






#endif
