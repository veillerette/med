CFLAGS=-ansi -pedantic -Wall
CLIBS=-lm
EXE=test

$(EXE): Step.c System.c Staff.c
	gcc -o $(EXE) $^ $(CFLAGS) $(CLIBS)

clean:
	rm $(EXE)
	rm *~
