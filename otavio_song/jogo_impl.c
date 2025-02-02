#include "jogo.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

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
        al_play_sample(sons[entrada - 1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
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

void inicializar_sons(ALLEGRO_SAMPLE* sons[]) {
    sons[0] = al_load_sample("440-red.wav"); // Vermelho
    sons[1] = al_load_sample("554-green.wav"); // Amarelo
    sons[2] = al_load_sample("659-blue.wav"); // Verde
    sons[3] = al_load_sample("740-yellow.wav"); // Azul

    for (int i = 0; i < 4; i++) {
        if (!sons[i]) {
            fprintf(stderr, "Erro ao carregar o som %d\n", i + 1);
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    ALLEGRO_DISPLAY* tela = al_create_display(640, 480);
    if (!tela) {
        fprintf(stderr, "Falha ao criar a janela.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* fila_eventos = al_create_event_queue();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60);
    ALLEGRO_FONT* fonte = al_load_ttf_font("fonte.ttf", 18, 0);

    if (!fila_eventos || !timer || !fonte) {
        fprintf(stderr, "Erro ao inicializar recursos.\n");
        return -1;
    }

    ALLEGRO_BITMAP* prisma = al_load_bitmap("prismas.png");
    ALLEGRO_BITMAP* prismaClaro = al_load_bitmap("prismas_claros.png");

    if (!prisma || !prismaClaro) {
        fprintf(stderr, "Erro ao carregar imagens.\n");
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(4);

    ALLEGRO_SAMPLE* sons[4];
    inicializar_sons(sons);

    bool running = true;
    while (running) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(fila_eventos, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int cor = identificar_cor_teclado(evento.keyboard.keycode);
            if (cor) {
                piscar_entrada(cor, prismaClaro, prisma, sons);
            }
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int cor = identificar_cor_mouse(evento.mouse.x, evento.mouse.y);
            if (cor) {
                piscar_entrada(cor, prismaClaro, prisma, sons);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        al_destroy_sample(sons[i]);
    }

    al_destroy_bitmap(prisma);
    al_destroy_bitmap(prismaClaro);
    al_destroy_font(fonte);
    al_destroy_timer(timer);
    al_destroy_event_queue(fila_eventos);
    al_destroy_display(tela);

    return 0;
}
