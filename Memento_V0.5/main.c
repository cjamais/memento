#include "addons.h"
#include "jogo.h"
#include "queue.h"

int main() {

    //PREPARAÇÃO DO AMBIENTE
    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        return -1;
    }

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY* tela = al_create_display(800, 800);
    ALLEGRO_COLOR cores[4];

    int posicoes[4][2];
    inicializar_posicoes(posicoes);

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
      ALLEGRO_BITMAP* prismaClaro = al_load_bitmap("prisma_claro.png");

    if (!prismaPadrao) {
        printf("Erro ao carregar a imagem prisma.png.\n");
        al_destroy_font(font);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_destroy_display(tela);
        return -1;
    }

    //PREPRARAÇÃO DA FILA
    T_Queue fila_original, sequencia, fila_copia;
    T_Info saiu, esperado, entrada_usuario;
    int errou = 0;

    fila_original = init(20);
    sequencia = init(20);
    fila_copia = init(20);

    srand(time(NULL));

    for (int i = 0;i < 20;i++) {
        int random = (rand() % 4) + 1;
        printf("%d %s\n", random, enqueue(fila_original, random) ? "Entrou" : "Não entrou");
    }

    int rodada = 1;


    while (true) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

		desenhar_prisma(prismaPadrao);

        int rodada = 1;
        while (rodada <= 20 && !errou) {
            printf("\nRodada %d\nCores que piscaram:", rodada);
            for (int i = 0;i < rodada;i++) {
                dequeue(fila_original, &saiu);
                enqueue(sequencia, saiu);
                enqueue(fila_copia, saiu);
                printf(" %d", saiu);

                //EXIBINDO AS CORES DA RODADA
                switch (saiu) {
                case 1:
                    //VERMELHO
                    al_draw_bitmap_region(prismaClaro, 0, 0, 200, 200, 300, 175, 0);
                    al_flip_display();
                    al_rest(0.7);
                    break;
                case 2:
                    //AMAERELO
                    al_draw_bitmap_region(prismaClaro, 200, 0, 200, 200, 300, 425, 0);
                    al_flip_display();
                    al_rest(0.7);
                    break;
                case 3:
                    //VERDE
                    al_draw_bitmap_region(prismaClaro, 400, 0, 200, 200, 175, 300, 0);
                    al_flip_display();
                    al_rest(0.7);
                    break;
                case 4:
                    //AZUL
                    al_draw_bitmap_region(prismaClaro, 600, 0, 200, 200, 425, 300, 0);
                    al_flip_display();
                    al_rest(0.7);
                    break;
                default:
                    break;
                }
                desenhar_prisma(prismaPadrao);
				al_rest(0.3);
            }

            while (!is_empty(fila_original)) {
                dequeue(fila_original, &saiu);
                enqueue(fila_copia, saiu);
            }

			//WHILE DA ENTRADA DO USUÁRIO
            while (!is_empty(sequencia)) {
                dequeue(sequencia, &saiu);

                /*printf("\nVez do usuário: ");
                
                identificar_cor_teclado(int tecla);
                identificar_cor_mouse(int x, int y);
                e depois piscar a cor correspondente
                al_rest(5);*/

                /*scanf("%d", &entrada_usuario);
                if (entrada_usuario == saiu) {
                    printf("OK\n");
                }
                else {
                    printf("ERROU!\n");
                    errou = 1;
                    break;
                }*/
            }

            while (!is_empty(fila_copia)) {
                dequeue(fila_copia, &saiu);
                enqueue(fila_original, saiu);
            }
            rodada++;
            al_rest(1);
        }

        destroy(fila_copia);
        destroy(sequencia);
        destroy(fila_original);
    }

    al_destroy_bitmap(prismaPadrao);
    al_destroy_font(font);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(tela);

    return 0;
}
