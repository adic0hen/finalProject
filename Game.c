
typedef struct cell {
	int solVal; /* the number that solve the current board, will change after every Backtracking */
	int val; /* the number the user has entered. -1 if empty. val==solVal if isFixed */
	int isFixed; /* 1 if the cell is fixed, 0 else */
} Cell; /*Cell structure is reffered to as -cell- */

typedef struct node {
	Cell* data;
	struct node* next;
	struct node* prev;
} Node;


