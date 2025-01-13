#include <allegro.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

void draw_button(int x, int y, int width, int height, int color, const char* label) {
    rectfill(screen, x, y, x + width, y + height, color);
    textout_centre_ex(screen, font, label, x + width / 2, y + height / 2, makecol(255, 255, 255), -1);
}

int main() {
    if (allegro_init() != 0) {
        allegro_message("Falha ao inicializar o Allegro.");
        return 1;
    }

    install_keyboard();
    install_mouse();
    set_color_depth(32);
    install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    SAMPLE *sound1 = load_sample("440-red.wav");
    SAMPLE *sound2 = load_sample("554-green.wav");

    if (!sound1) {
        allegro_message("Falha ao carregar o arquivo de som 440-red.wav.");
        return 1;
    }
    if (!sound2) {
        allegro_message("Falha ao carregar o arquivo de som 554-green.wav.");
        return 1;
    }

    show_mouse(screen);

    int red_button_color = makecol(255, 0, 0);
    int green_button_color = makecol(0, 255, 0);

    bool running = true;
    while (running) {
        if (mouse_b & 1) {
            int x = mouse_x;
            int y = mouse_y;

            if (x >= 100 && x <= 300 && y >= 100 && y <= 200) {
                play_sample(sound1, 255, 128, 1000, 0);
                rest(500);
                stop_sample(sound1);
            }
            if (x >= 100 && x <= 300 && y >= 250 && y <= 350) {
                play_sample(sound2, 255, 128, 1000, 0);
                rest(500);
                stop_sample(sound2);
            }
        }

        clear_to_color(screen, makecol(0, 0, 0));

        draw_button(100, 100, 200, 100, red_button_color, "Vermelho");
        draw_button(100, 250, 200, 100, green_button_color, "Verde");

        rest(10);
    }

    destroy_sample(sound1);
    destroy_sample(sound2);

    return 0;
}
END_OF_MAIN();
