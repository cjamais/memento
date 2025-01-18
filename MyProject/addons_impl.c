#include "addons.h"

void iniciar_addons(ALLEGRO_DISPLAY* tela, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font, ALLEGRO_TIMER* timer) {
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();
    al_install_keyboard();
    al_init_acodec_addon();
    al_install_audio();
    al_reserve_samples(4);

    al_set_window_position(tela, 600, 100);
    al_set_window_title(tela, "Memento");

    al_register_event_source(event_queue, al_get_display_event_source(tela));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));

    al_start_timer(timer);
}

void desenhar_prisma(ALLEGRO_BITMAP* prisma) {
    // Limpa a tela com a cor branca
    al_clear_to_color(al_map_rgb(255, 255, 255));

    // Desenha as regiões do prisma
    al_draw_bitmap_region(prisma, 0, 0, 200, 200, 300, 175, 0);  // Vermelho
    al_draw_bitmap_region(prisma, 200, 0, 200, 200, 300, 425, 0); // Amarelo
    al_draw_bitmap_region(prisma, 400, 0, 200, 200, 175, 300, 0); // Verde
    al_draw_bitmap_region(prisma, 600, 0, 200, 200, 425, 300, 0); // Azul

    // Atualiza a exibição
    al_flip_display();
}
