#include "jogo.h"

void inicializar_posicoes(int posicoes[][2]) {
    /*
    * A matriz `posicoes` armazena as coordenadas (x, y) do centro de cada cor/prisma.
    * Usamos `posicoes[cor - 1]` para mapear a cor (1-4) ao índice correto (0-3).
    */
    posicoes[0][0] = 300; posicoes[0][1] = 175; // Vermelho
	posicoes[1][0] = 300; posicoes[1][1] = 425; // Amarelo
    posicoes[2][0] = 175; posicoes[2][1] = 300; // Verde
    posicoes[3][0] = 425; posicoes[3][1] = 300; // Azul
}

int identificar_cor_teclado(int tecla) {
    switch (tecla) {
    case ALLEGRO_KEY_UP:    return VERMELHO;
    case ALLEGRO_KEY_LEFT:  return VERDE;
    case ALLEGRO_KEY_DOWN:  return AMARELO;
    case ALLEGRO_KEY_RIGHT: return AZUL;
    default:                return 0;
    }
}

int identificar_cor_mouse(int x, int y) {
    if (x >= 300 && x <= 500 && y >= 175 && y <= 375) return VERMELHO;
    if (x >= 175 && x <= 375 && y >= 300 && y <= 500) return VERDE;
    if (x >= 300 && x <= 500 && y >= 425 && y <= 625) return AMARELO;
    if (x >= 425 && x <= 625 && y >= 300 && y <= 500) return AZUL;
    return 0;
}