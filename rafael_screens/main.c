#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>

// Define os estados do jogo
typedef enum {
    STATE_START,
    STATE_GAME
} GameState;

int main() {
    if (!al_init()) {
        fprintf(stderr, "Falha ao inicializar o Allegro!\n");
        return -1;
    }
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_image_addon();
    al_install_mouse();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Falha ao criar a janela!\n");
        return -1;
    }
    al_set_window_title(display, "Simon says");

    ALLEGRO_FONT *font = al_create_builtin_font();
    ALLEGRO_TIMER *timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_BITMAP *start_image = al_load_bitmap("start.png"); 
    ALLEGRO_BITMAP *sprite = al_load_bitmap("prisma.png"); 
    if (!start_image || !sprite) {
        fprintf(stderr, "Erro ao carregar as imagens.\n");
        return -1;
    }

    // Calcula as posições centrais para a imagem `start.png`
    int screen_width = al_get_display_width(display);
    int screen_height = al_get_display_height(display);

    int start_image_width = al_get_bitmap_width(start_image);
    int start_image_height = al_get_bitmap_height(start_image);

    int start_image_x = (screen_width - start_image_width) / 2;
    int start_image_y = (screen_height - start_image_height) / 2;

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_start_timer(timer);

    GameState current_state = STATE_START; // Estado inicial
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Troca de estado ao clicar na imagem central
            if (current_state == STATE_START) {
                if (event.mouse.x >= start_image_x && event.mouse.x <= start_image_x + start_image_width &&
                    event.mouse.y >= start_image_y && event.mouse.y <= start_image_y + start_image_height) {
                    current_state = STATE_GAME; // Troca para o jogo
                }
            }
        }

        if (event.type == ALLEGRO_EVENT_TIMER) {
            al_clear_to_color(al_map_rgb(255, 255, 255));

            if (current_state == STATE_START) {
                al_draw_bitmap(start_image, start_image_x, start_image_y, 0);
            } else if (current_state == STATE_GAME) {
                al_draw_bitmap_region(sprite, 0, 0, 200, 200, 300, 175, 0);
                al_draw_bitmap_region(sprite, 200, 0, 200, 200, 300, 425, 0);
                al_draw_bitmap_region(sprite, 400, 0, 200, 200, 175, 300, 0);
                al_draw_bitmap_region(sprite, 600, 0, 200, 200, 425, 300, 0);
            }

            al_flip_display();
        }
    }

    al_destroy_font(font);
    al_destroy_bitmap(start_image);
    al_destroy_bitmap(sprite);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
