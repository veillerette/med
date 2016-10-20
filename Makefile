CC=colorgcc
CFLAGS=-ansi -Wall -O2
CLIBS=-lm -lSDL -lSDL_gfx -lSDL_ttf
EXE=prog
OBJ=obj/


$(EXE): main.o Step.o System.o Staff.o Test.o Window.o Images.o Events.o
	cd $(OBJ) && $(CC) -o ../$(EXE) $? $(CFLAGS) $(CLIBS)

%.o: %.c
	if [ ! -d "$(OBJ)" ]; then mkdir $(OBJ); fi;	
	$(CC) -o $(OBJ)$@ -c $< $(CFLAGS)
	
clean:
	rm -f $(EXE)
	rm -f *~
	if [ -f "gmon.out" ]; then rm -f gmon.out; fi;
	if [ -d "$(OBJ)" ]; then rm -f $(OBJ)*.o; rmdir $(OBJ); fi;	
	
