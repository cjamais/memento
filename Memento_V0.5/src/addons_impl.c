#include "../lib/addons.h"

void iniciar_addons(ALLEGRO_DISPLAY* tela, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font, ALLEGRO_TIMER* timer) {
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();

    al_set_window_position(tela, 600, 100);
    al_set_window_title(tela, "Memento");

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(tela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
}

void desenhar_prisma(ALLEGRO_BITMAP* prisma) {
    // Limpa a tela com a cor branca
    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Desenha as regi�es do prisma
    al_draw_bitmap_region(prisma, 0, 0, 200, 200, 300, 175, 0);  // Vermelho
    al_draw_bitmap_region(prisma, 200, 0, 200, 200, 300, 425, 0); // Amarelo
    al_draw_bitmap_region(prisma, 400, 0, 200, 200, 175, 300, 0); // Verde
    al_draw_bitmap_region(prisma, 600, 0, 200, 200, 425, 300, 0); // Azul

    // Atualiza a exibi��o
    al_flip_display();
}

void inicializar_sons(ALLEGRO_SAMPLE* sons[]) {
    char* arquivos[5] = {
        "assets/sounds/red.wav",
        "assets/sounds/yellow.wav",
        "assets/sounds/green.wav",
        "assets/sounds/blue.wav",
        "assets/sounds/error.wav"
    };

    for (int i = 0; i < 5; i++) {
        sons[i] = al_load_sample(arquivos[i]);
        if (!sons[i]) {
            fprintf(stderr, "Erro ao carregar %s\n", arquivos[i]);
            exit(EXIT_FAILURE);
        }
    }
}
