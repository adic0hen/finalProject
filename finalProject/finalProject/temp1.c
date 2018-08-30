#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "gurobi_c.h"



int main()
{
	GRBenv   *env = NULL;
	GRBmodel *model = NULL;
	int       board[9][9];
	int       ind[9];
	double    val[9];
	double    lb[9*9*9];
	char      vtype[9*9*9];
	char     *names[9*9*9];
	char      namestorage[10 * 9*9*9];
	char     *cursor;
	int       optimstatus;
	double    objval;
	int       i, j, v, ig, jg, count;
	int       error = 0;
	int DIM = 9;
	int SUBDIM = 3;

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; i++) {
			board[i][j] = -1;
		}
	}
	printf("0");
	
	/*
	for (i = 0; i < DIM; i++) {
		fgets(inputline, 100, fp);
		if (strlen(inputline) < 9) {
			fprintf(stderr, "Error: not enough board positions specified\n");
			exit(1);
		}
		for (j = 0; j < DIM; j++) {
			board[i][j] = (int)inputline[j] - (int) '1';
			if (board[i][j] < 0 || board[i][j] >= DIM)
				board[i][j] = -1;
		}
	}
	*/

	/* Create an empty model */

	cursor = namestorage;
	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				if (board[i][j] == v)
					lb[i*DIM*DIM + j * DIM + v] = 1;
				else
					lb[i*DIM*DIM + j * DIM + v] = 0;
				vtype[i*DIM*DIM + j * DIM + v] = GRB_BINARY;

				names[i*DIM*DIM + j * DIM + v] = cursor;
				sprintf(names[i*DIM*DIM + j * DIM + v], "x[%d,%d,%d]", i, j, v + 1);
				cursor += strlen(names[i*DIM*DIM + j * DIM + v]) + 1;
			}
		}
	}
	printf("1");
	/* Create environment */

	error = GRBloadenv(&env, "sudoku.log");
	if (error) goto QUIT;

	/* Create new model */
	printf("2");
	error = GRBnewmodel(env, &model, "sudoku", DIM*DIM*DIM, NULL, lb, NULL,
		vtype, names);
	if (error) goto QUIT;
	printf("3");
	/* Each cell gets a value */

	for (i = 0; i < DIM; i++) {
		for (j = 0; j < DIM; j++) {
			for (v = 0; v < DIM; v++) {
				ind[v] = i * DIM*DIM + j * DIM + v;
				val[v] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto QUIT;
		}
	}
	printf("4");
	/* Each value must appear once in each row */

	for (v = 0; v < DIM; v++) {
		for (j = 0; j < DIM; j++) {
			for (i = 0; i < DIM; i++) {
				ind[i] = i * DIM*DIM + j * DIM + v;
				val[i] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto QUIT;
		}
	}
	printf("5");
	/* Each value must appear once in each column */

	for (v = 0; v < DIM; v++) {
		for (i = 0; i < DIM; i++) {
			for (j = 0; j < DIM; j++) {
				ind[j] = i * DIM*DIM + j * DIM + v;
				val[j] = 1.0;
			}

			error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
			if (error) goto QUIT;
		}
	}
	printf("6");
	/* Each value must appear once in each subgrid */

	for (v = 0; v < DIM; v++) {
		for (ig = 0; ig < SUBDIM; ig++) {
			for (jg = 0; jg < SUBDIM; jg++) {
				count = 0;
				for (i = ig * SUBDIM; i < (ig + 1)*SUBDIM; i++) {
					for (j = jg * SUBDIM; j < (jg + 1)*SUBDIM; j++) {
						ind[count] = i * DIM*DIM + j * DIM + v;
						val[count] = 1.0;
						count++;
					}
				}

				error = GRBaddconstr(model, DIM, ind, val, GRB_EQUAL, 1.0, NULL);
				if (error) goto QUIT;
			}
		}
	}
	printf("7");
	/* Optimize model */

	error = GRBoptimize(model);
	if (error) goto QUIT;
	printf("8");
	/* Write model to 'sudoku.lp' */

	error = GRBwrite(model, "sudoku.lp");
	if (error) goto QUIT;
	printf("9");
	/* Capture solution information */

	error = GRBgetintattr(model, GRB_INT_ATTR_STATUS, &optimstatus);
	if (error) goto QUIT;
	printf("10");
	error = GRBgetdblattr(model, GRB_DBL_ATTR_OBJVAL, &objval);
	if (error) goto QUIT;

	printf("\nOptimization complete\n");
	if (optimstatus == GRB_OPTIMAL)
		printf("Optimal objective: %.4e\n", objval);
	else if (optimstatus == GRB_INF_OR_UNBD)
		printf("Model is infeasible or unbounded\n");
	else
		printf("Optimization was stopped early\n");
	printf("\n");

QUIT:

	/* Error reporting */

	if (error) {
		printf("ERROR: %s\n", GRBgeterrormsg(env));
		exit(1);
	}


	/* Free model */

	GRBfreemodel(model);

	/* Free environment */

	GRBfreeenv(env);

	return 0;
}