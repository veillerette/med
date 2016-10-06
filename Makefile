CFLAGS=-ansi -pedantic -Wall -O2
CLIBS=-lm
EXE=prog

$(EXE): main.c Step.c Step.h System.c System.h Staff.c Staff.h Test.c Test.h
	gcc -o $(EXE) $^ $(CFLAGS) $(CLIBS)

clean:
	rm -f $(EXE)
	rm -f *~
