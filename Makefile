CC=gcc
CFLAGS=-ansi -Wall -O2
CLIBS=-lm -lSDL -lSDL_gfx -lSDL_ttf -lreadline
EXE=prog
OBJ=obj/
SRC=src/

$(EXE): $(OBJ)main2.o $(OBJ)Step.o $(OBJ)System.o $(OBJ)Parser.o $(OBJ)Staff.o $(OBJ)Test.o $(OBJ)Window2.o $(OBJ)Images.o $(OBJ)Events.o $(OBJ)ABC.o
	$(CC) -o $(EXE) $^ $(CFLAGS) $(CLIBS)

$(OBJ)%.o: $(SRC)%.c
	@if [ ! -d "$(OBJ)" ]; then mkdir $(OBJ); fi;	
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -f $(EXE)
	rm -f *~
	if [ -f "gmon.out" ]; then rm -f gmon.out; fi;
	if [ -d "$(OBJ)" ]; then rm -f $(OBJ)*.o; rmdir $(OBJ); fi;	
	
