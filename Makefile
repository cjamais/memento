SRC = src/main.c src/jogo_impl.c src/queue_impl.c src/addons_impl.c
OBJ = $(SRC:.c=.o)
NAME = Memento
CC = gcc
CFLAGS = -Wall -g

## Verifica o sistema operacional
ifeq ($(OS),Windows_NT)
    EXE = $(NAME).exe
    RM = del /F /Q
    INCLUDES = -I allegro\include
    LDFLAGS = -L allegro\lib -lallegro_monolith
else
    EXE = $(NAME)
    RM = rm -f
    INCLUDES = -I/usr/include/allegro5
    LDFLAGS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec
endif

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(RM) $(OBJ) $(EXE)

run: $(EXE)
	./$(EXE)