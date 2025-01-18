#include "addons.h"

int main() {

    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY* tela = al_create_display(800, 800);

    if (!event_queue || !font || !timer || !tela) {
        printf("Erro ao criar recursos do Allegro.\n");

        if (font) al_destroy_font(font);
        if (timer) al_destroy_timer(timer);
        if (event_queue) al_destroy_event_queue(event_queue);
        if (tela) al_destroy_display(tela);

        return -1;
    }

    iniciar_addons(tela, event_queue, font, timer);

    ALLEGRO_BITMAP* prismaPadrao = al_load_bitmap("prisma.png");
    if (!prismaPadrao) {
        printf("Erro ao carregar a imagem prisma.png.\n");
        al_destroy_font(font);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_destroy_display(tela);
        return -1;
    }

    bool running = true;
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {
            desenhar_prisma(prismaPadrao);
        }
    }

    al_destroy_bitmap(prismaPadrao);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(tela);

    return 0;
}
