#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

typedef enum {
    STATE_START,
    STATE_GAME,
    STATE_GAME_OVER
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
    al_init_primitives_addon();

    ALLEGRO_DISPLAY *display = al_create_display(800, 600);
    if (!display) {
        fprintf(stderr, "Falha ao criar a janela!\n");
        return -1;
    }
    al_set_window_title(display, "Memento");

    ALLEGRO_FONT *font = al_create_builtin_font();
    if (!font) {
        fprintf(stderr, "Falha ao carregar a fonte incorporada!\n");
        return -1;
    }

    ALLEGRO_BITMAP *logo_image = al_load_bitmap("logo.png");
    ALLEGRO_BITMAP *start_button_image = al_load_bitmap("start.png");
    ALLEGRO_BITMAP *menu_button_image = al_load_bitmap("menu.png");
    ALLEGRO_BITMAP *restart_button_image = al_load_bitmap("restart.png");

    if (!logo_image || !start_button_image || !menu_button_image || !restart_button_image) {
        fprintf(stderr, "Erro ao carregar as imagens.\n");
        return -1;
    }

    ALLEGRO_BITMAP *sprite = al_create_bitmap(1000, 800);
    al_set_target_bitmap(sprite);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_draw_filled_rectangle(200, 0, 400, 200, al_map_rgb(0, 255, 0));
    al_draw_filled_rectangle(400, 0, 600, 200, al_map_rgb(0, 0, 255));
    al_draw_filled_rectangle(600, 0, 800, 200, al_map_rgb(255, 255, 0));
    al_set_target_bitmap(al_get_backbuffer(display));

    int screen_width = al_get_display_width(display);
    int screen_height = al_get_display_height(display);

    // Botões e posicionamento
    int start_button_width = al_get_bitmap_width(start_button_image);
    int start_button_height = al_get_bitmap_height(start_button_image);
    int start_button_x = (screen_width - start_button_width) / 2;
    int start_button_y = 300;

    int menu_button_width = al_get_bitmap_width(menu_button_image);
    int menu_button_height = al_get_bitmap_height(menu_button_image);
    int menu_button_x = screen_width / 2 - menu_button_width - 20;
    int menu_button_y = 400;

    int restart_button_width = al_get_bitmap_width(restart_button_image);
    int restart_button_height = al_get_bitmap_height(restart_button_image);
    int restart_button_x = screen_width / 2 + 20;
    int restart_button_y = 400;

    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    GameState current_state = STATE_START;
    bool running = true;

    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (current_state == STATE_START) {
                if (event.mouse.x >= start_button_x && event.mouse.x <= start_button_x + start_button_width &&
                    event.mouse.y >= start_button_y && event.mouse.y <= start_button_y + start_button_height) {
                    current_state = STATE_GAME;
                }
            } else if (current_state == STATE_GAME) {
                // Simulação de fim de jogo
                current_state = STATE_GAME_OVER;
            } else if (current_state == STATE_GAME_OVER) {
                if (event.mouse.x >= menu_button_x && event.mouse.x <= menu_button_x + menu_button_width &&
                    event.mouse.y >= menu_button_y && event.mouse.y <= menu_button_y + menu_button_height) {
                    current_state = STATE_START;
                } else if (event.mouse.x >= restart_button_x && event.mouse.x <= restart_button_x + restart_button_width &&
                           event.mouse.y >= restart_button_y && event.mouse.y <= restart_button_y + restart_button_height) {
                    current_state = STATE_GAME;
                }
            }
        }

        al_clear_to_color(al_map_rgb(255, 255, 255));

        if (current_state == STATE_START) {
            al_draw_bitmap(logo_image, (screen_width - al_get_bitmap_width(logo_image)) / 2, 100, 0);
            al_draw_bitmap(start_button_image, start_button_x, start_button_y, 0);
        } else if (current_state == STATE_GAME) {
            // o jogo irá ficar aqui
            al_draw_bitmap_region(sprite, 0, 0, 200, 200, 300, 175, 0);
            al_draw_bitmap_region(sprite, 200, 0, 200, 200, 300, 425, 0);
            al_draw_bitmap_region(sprite, 400, 0, 200, 200, 175, 300, 0);
            al_draw_bitmap_region(sprite, 600, 0, 200, 200, 425, 300, 0);
        } else if (current_state == STATE_GAME_OVER) {
            al_draw_text(font, al_map_rgb(255, 0, 0), screen_width / 2, 200, ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
            al_draw_bitmap(menu_button_image, menu_button_x, menu_button_y, 0);
            al_draw_bitmap(restart_button_image, restart_button_x, restart_button_y, 0);
        }

        al_flip_display();
    }

    al_destroy_font(font);
    al_destroy_bitmap(logo_image);
    al_destroy_bitmap(start_button_image);
    al_destroy_bitmap(menu_button_image);
    al_destroy_bitmap(restart_button_image);
    al_destroy_bitmap(sprite);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
