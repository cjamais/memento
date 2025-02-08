CC = gcc
CFLAGS = -Wall -g
INCLUDES = -I/usr/include/allegro5

## Verifica o sistema operacional
ifeq ($(OS),Windows_NT)
    LDFLAGS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec
    EXE = Memento.exe
else
    LDFLAGS = -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec
    EXE = Memento
endif

SRC = src/main.c src/jogo_impl.c src/queue_impl.c src/addons_impl.c
OBJ = $(SRC:.c=.o)

$(EXE): $(OBJ)
	$(CC) $(OBJ) -o $(EXE) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXE)
