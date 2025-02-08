#include "addons.h"
#define VERMELHO 1
#define AMARELO 2
#define VERDE 3
#define AZUL 4

void inicializar_posicoes(int posicoes[][2]);
int identificar_cor_teclado(int tecla);
int identificar_cor_mouse(int x, int y);
void piscar_entrada(int entrada, ALLEGRO_BITMAP* prismaClaro, ALLEGRO_BITMAP* prismaPadrao, ALLEGRO_SAMPLE* sons[]);
void tocar_som(int entrada, ALLEGRO_SAMPLE* sons[]);