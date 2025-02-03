#include "../lib/jogo.h"

void inicializar_posicoes(int posicoes[][2]) {
    /*
    * A matriz `posicoes` armazena as coordenadas (x, y) do centro de cada cor/prisma.
    * Usamos `posicoes[cor - 1]` para mapear a cor (1-4) ao �ndice correto (0-3).
    */
    posicoes[0][0] = 300; posicoes[0][1] = 175; // Vermelho
	posicoes[1][0] = 300; posicoes[1][1] = 425; // Amarelo
    posicoes[2][0] = 175; posicoes[2][1] = 300; // Verde
    posicoes[3][0] = 425; posicoes[3][1] = 300; // Azul
}

int identificar_cor_teclado(int tecla) {
    switch (tecla) {
    case ALLEGRO_KEY_UP:    return 1;
    case ALLEGRO_KEY_LEFT:  return 3;
    case ALLEGRO_KEY_DOWN:  return 2;
    case ALLEGRO_KEY_RIGHT: return 4;
    default:                return 0;
    }
}

int identificar_cor_mouse(int x, int y) {
    if (x >= 300 && x <= 500 && y >= 175 && y <= 375) return 1;
    if (x >= 175 && x <= 375 && y >= 300 && y <= 500) return 3;
    if (x >= 300 && x <= 500 && y >= 425 && y <= 625) return 2;
    if (x >= 425 && x <= 625 && y >= 300 && y <= 500) return 4;
    return 0;
}

void piscar_entrada(int entrada, ALLEGRO_BITMAP* prismaClaro, ALLEGRO_BITMAP* prismaPadrao, ALLEGRO_SAMPLE* sons[]) {
    if (entrada >= 1 && entrada <= 4) {
        tocar_som(entrada, sons);  // Toca o som correspondente
    }

    switch (entrada) {
    case 1:
        //VERMELHO
        al_draw_bitmap_region(prismaClaro, 0, 0, 200, 200, 300, 175, 0);
        al_flip_display();
        al_rest(0.2);
        break;
    case 2:
        //AMARELO
        al_draw_bitmap_region(prismaClaro, 200, 0, 200, 200, 300, 425, 0);
        al_flip_display();
        al_rest(0.2);
        break;
    case 3:
        //VERDE
        al_draw_bitmap_region(prismaClaro, 400, 0, 200, 200, 175, 300, 0);
        al_flip_display();
        al_rest(0.2);
        break;
    case 4:
        //AZUL
        al_draw_bitmap_region(prismaClaro, 600, 0, 200, 200, 425, 300, 0);
        al_flip_display();
        al_rest(0.2);
        break;
    default:
        break;
    }
    desenhar_prisma(prismaPadrao);
}

void tocar_som(int entrada, ALLEGRO_SAMPLE* sons[]) {
    if (entrada >= 1 && entrada <= 4) {
        ALLEGRO_SAMPLE_ID sample_id;
        al_play_sample(sons[entrada - 1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
        al_rest(0.2);
        al_stop_sample(&sample_id);
    }
}
