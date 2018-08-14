#ifndef GAME_H_
#define GAME_H_
typedef struct Cell {
	int solVal; /* the number that solve the current board, will change after every Backtracking */
	int val; /* the number the user has entered. -1 if empty. val==solVal if isFixed */
	int isFixed; /* 1 if the cell is fixed, 0 else */
} cell; /*Cell structure is reffered to as -cell- */

/*Board is internally represented as an array of m*n pointers to cells */
#endif
