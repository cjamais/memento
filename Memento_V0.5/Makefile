SRC=src/*.c
NAME=Memento
CC=gcc
RM=rm -f
OBJ=*.o
LIB=-I allegro\include
MONOLITH=liballegro_monolith.dll.a

win: files.o
	$(CC) $(LIB) $(OBJ) -o $(NAME).exe $(MONOLITH)

files.o:
	$(CC) $(LIB) -c $(SRC)

play:
	.\$(NAME).exe
