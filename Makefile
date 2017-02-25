CC=colorgcc
CFLAGS=-ansi -Wall -Wextra -O3
CLIBS=-lm -lSDL -lSDL_gfx -lSDL_ttf -lreadline -lfluidsynth
EXE=prog
OBJ=obj/
SRC=src/
INCL=include/
FILEC:= $(wildcard $(SRC)*.c)
FILEO:= $(patsubst $(SRC)%.c,$(OBJ)%.o,$(FILEC))

$(EXE): $(FILEO)
	$(CC) -o $(EXE) $^ $(CFLAGS) $(CLIBS)

obj/main.o:src/main.c
	@if [ ! -d "$(OBJ)" ]; then mkdir $(OBJ); fi;	
	$(CC) -o $@ -c $< $(CFLAGS)
	
$(OBJ)%.o: $(SRC)%.c $(INCL)%.h
	@if [ ! -d "$(OBJ)" ]; then mkdir $(OBJ); fi;	
	$(CC) -o $@ -c $< $(CFLAGS)
	
clean:
	rm -f $(EXE)
	rm -f *~
	if [ -f "gmon.out" ]; then rm -f gmon.out; fi;
	if [ -d "$(OBJ)" ]; then rm -f $(OBJ)*.o; rmdir $(OBJ); fi;	

wc:
	@wc $(SRC)*.c $(INCL)*.h Makefile | sort -n

push:
	git add $(INCL)*.h $(SRC)*.c Makefile
	git commit -m "$M"
	git push

pull:
	git pull

stats:
	@echo -n "Number of files : " && ls -l include/ src/ | grep "\.h\|\.c" | wc -l;
	@echo -n "Number of lines : " && wc -l $(SRC)*.c $(INCL)*.h Makefile | grep "total" | cut -d " " -f2;
	@echo -n "Number of car : " && wc -c $(SRC)*.c $(INCL)*.h Makefile | grep "total" | cut -d " " -f1;
