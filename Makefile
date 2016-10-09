CFLAGS=-ansi -Wall -O2
CLIBS=-lm -lSDL -lSDL_gfx -lSDL_ttf
EXE=prog

$(EXE): main.c Step.c Step.h System.c System.h Staff.c Staff.h Test.c Test.h Window.c Window.h Images.c Images.h
	gcc -o $(EXE) $^ $(CFLAGS) $(CLIBS)

clean:
	rm -f $(EXE)
	rm -f *~
