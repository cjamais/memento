#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

void iniciar_addons(ALLEGRO_DISPLAY* tela, ALLEGRO_EVENT_QUEUE* event_queue, ALLEGRO_FONT* font, ALLEGRO_TIMER* timer);
void desenhar_prisma(ALLEGRO_BITMAP* prisma);
void inicializar_sons(ALLEGRO_SAMPLE* sons[]);