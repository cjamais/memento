#include <allegro5/allegro.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdlib.h>

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

void tocar_som(int entrada, ALLEGRO_SAMPLE* sons[]) {
    if (entrada >= 1 && entrada <= 4) {
        al_play_sample(sons[entrada - 1], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
    }
}

int identificar_cor_teclado(int tecla) {
    switch (tecla) {
    case ALLEGRO_KEY_UP:    return 1; // Vermelho
    case ALLEGRO_KEY_LEFT:  return 3; // Verde
    case ALLEGRO_KEY_DOWN:  return 2; // Amarelo
    case ALLEGRO_KEY_RIGHT: return 4; // Azul
    default:                return 0;
    }
}

int identificar_cor_mouse(int x, int y) {
    if (x >= 300 && x <= 500 && y >= 175 && y <= 375) return 1; // Vermelho
    if (x >= 175 && x <= 375 && y >= 300 && y <= 500) return 3; // Verde
    if (x >= 300 && x <= 500 && y >= 425 && y <= 625) return 2; // Amarelo
    if (x >= 425 && x <= 625 && y >= 300 && y <= 500) return 4; // Azul
    return 0;
}

int main() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar Allegro.\n");
        return -1;
    }

    if (!al_install_audio() || !al_init_acodec_addon() || !al_reserve_samples(4)) {
        fprintf(stderr, "Erro ao inicializar o sistema de áudio.\n");
        return -1;
    }

    ALLEGRO_DISPLAY* display = al_create_display(640, 480);
    if (!display) {
        fprintf(stderr, "Falha ao criar display.\n");
        return -1;
    }

    al_install_keyboard();
    al_install_mouse();

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Erro ao criar fila de eventos.\n");
        return -1;
    }

    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_mouse_event_source());
    al_register_event_source(queue, al_get_display_event_source(display));

    ALLEGRO_SAMPLE* sons[4];
    inicializar_sons(sons);

    bool running = true;
    while (running) {
        ALLEGRO_EVENT evento;
        al_wait_for_event(queue, &evento);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (evento.type == ALLEGRO_EVENT_KEY_DOWN) {
            int cor = identificar_cor_teclado(evento.keyboard.keycode);
            if (cor) {
                tocar_som(cor, sons);
                printf("Tecla pressionada: Cor %d\n", cor);
            }
        } else if (evento.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            int cor = identificar_cor_mouse(evento.mouse.x, evento.mouse.y);
            if (cor) {
                tocar_som(cor, sons);
                printf("Clique do mouse: Cor %d\n", cor);
            }
        }
    }

    for (int i = 0; i < 4; i++) {
        al_destroy_sample(sons[i]);
    }

    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}
