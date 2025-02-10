SRC = src/main.c src/jogo_impl.c src/queue_impl.c src/addons_impl.c
OBJ = $(SRC:.c=.o)
NAME = Memento
CC = gcc
CFLAGS = -Wall -g

# Detecta o sistema operacional
UNAME_S := $(shell uname -s)

ifeq ($(OS),Windows_NT) # Windows
    EXE = $(NAME).exe
    RM = del /F /Q
    INCLUDES = -I allegro\include
    LDFLAGS = -L allegro\lib -lallegro_monolith
else ifeq ($(UNAME_S), Darwin)  # macOS
    EXE = $(NAME)
    RM = rm -f
    INCLUDES = -I/usr/local/opt/allegro/include
    LDFLAGS = -L/usr/local/opt/allegro/lib -lallegro -lallegro_image -lallegro_font -lallegro_ttf -lallegro_primitives -lallegro_audio -lallegro_acodec -lallegro_main
else  # Linux
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
