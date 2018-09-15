CC = gcc
OBJS = Backtrack.o GameDataStructs.o GamePlay.o\
InGameCommands.o InitAndTerminateModule.o IOCommands.o\
Main.o Parser.o Solver.o UndoRedoCommands.o
 
EXEC = sudoku-console
COMP_FLAGS = -ansi -O3 -Wall -Wextra -Werror -pedantic-errors
GUROBI_COMP = -I/usr/local/lib/gurobi563/include
GUROBI_LIB = -L/usr/local/lib/gurobi563/lib -lgurobi56

$(EXEC): $(OBJS)
	$(CC) $(OBJS) $(GUROBI_LIB) -o $@ -lm
Main.o: Backtrack.h GameDataStructs.h GamePlay.h\
InGameCommands.h InitAndTerminateModule.h IOCommands.h\
Parser.h Solver.h UndoRedoCommands.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Backtrack.o: Backtrack.c Backtrack.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GameDataStructs.o: GameDataStructs.c GameDataStructs.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
GamePlay.o: GamePlay.c GamePlay.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
InGameCommands.o: InGameCommands.c InGameCommands.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
InitAndTerminateModule.o: InitAndTerminateModule.c\
InitAndTerminateModule.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
IOCommands.o: IOCommands.c IOCommands.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Parser.o: Parser.c Parser.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
Solver.o: Solver.c Solver.h
	$(CC) $(COMP_FLAGS) $(GUROBI_COMP) -c $*.c
UndoRedoCommands.o: UndoRedoCommands.c UndoRedoCommands.h
clean:
	rm -f *.o $(EXEC)
all: $(EXEC)

