#include "../lib/addons.h"
#include "../lib/jogo.h"
#include "../lib/queue.h"
#define MAX_RODADAS 100

int main() {

    //PREPARAÇÃO DO AMBIENTE
    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        return -1;
    }

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(4);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY* tela = al_create_display(800, 800);
    ALLEGRO_SAMPLE* sons[5];
    inicializar_sons(sons);

    int posicoes[4][2];
    inicializar_posicoes(posicoes);
    al_register_event_source(event_queue, al_get_display_event_source(tela));

    if (!event_queue || !font || !timer || !tela) {
        printf("Erro ao criar recursos do Allegro.\n");

        if (font) al_destroy_font(font);
        if (timer) al_destroy_timer(timer);
        if (event_queue) al_destroy_event_queue(event_queue);
        if (tela) al_destroy_display(tela);

        return -1;
    }

    iniciar_addons(tela, event_queue, font, timer);

    ALLEGRO_BITMAP* prismaPadrao = al_load_bitmap("assets/img/prisma.png");
    ALLEGRO_BITMAP* prismaClaro = al_load_bitmap("assets/img/prisma_claro.png");
    ALLEGRO_FONT* font2 = al_load_font("assets/fonts/Poppins-Medium.ttf", 24, 0);
    ALLEGRO_BITMAP* icone = al_load_bitmap("assets/img/icone.png");

    al_set_display_icon(tela, icone);

    if (!prismaPadrao) {
        printf("Erro ao carregar a imagem prisma.png.\n");
        al_destroy_font(font);
        al_destroy_timer(timer);
        al_destroy_event_queue(event_queue);
        al_destroy_display(tela);
        return -1;
    }

    bool running = true;

    //LOOP PRINCIPAL DO JOGO
    while (running) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            running = false;
            break;
        }

        //PREPRARAÇÃO DA FILA
        T_Queue fila_original, sequencia, fila_copia;
        T_Info saiu, esperado;
        int errou = 0;
        int pontuacao = 0; 

        fila_original = init(MAX_RODADAS);
        sequencia = init(MAX_RODADAS);
        fila_copia = init(MAX_RODADAS);

        srand(time(NULL));

        for (int i = 0; i < MAX_RODADAS; i++) {
            int random = (rand() % 4) + 1;
            enqueue(fila_original, random);
        }

        desenhar_prisma(prismaPadrao);

        int rodada = 1;
        while (rodada <= MAX_RODADAS && !errou) {
            
            char texto_rodada[50];
            sprintf(texto_rodada, "RODADA %d", rodada);

            int y_texto = 50;
            int x_texto = (800 - al_get_text_width(font2, texto_rodada)) / 2;
            al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);

            // ADICIONEI AQUI
            // PONTUAÇÃO
            char texto_pontuacao[50];
            sprintf(texto_pontuacao, "%d pontos", pontuacao);
            int x_texto_pontuacao = (800 - al_get_text_width(font2, texto_pontuacao)) / 2; 
            int y_texto_pontuacao = 670; 
            al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);

            for (int i = 0; i < rodada; i++) {
                dequeue(fila_original, &saiu);
                enqueue(sequencia, saiu);
                enqueue(fila_copia, saiu);

                // EXIBINDO AS CORES DA RODADA
                switch (saiu) {
                    case 1:
                        // VERMELHO
                        al_draw_bitmap_region(prismaClaro, 0, 0, 200, 200, 300, 175, 0);
                        tocar_som(1,sons);
                        break;
                    case 2:
                        // AMARELO
                        al_draw_bitmap_region(prismaClaro, 200, 0, 200, 200, 300, 425, 0);
                        tocar_som(2,sons);
                        break;
                    case 3:
                        // VERDE
                        al_draw_bitmap_region(prismaClaro, 400, 0, 200, 200, 175, 300, 0);
                        tocar_som(3,sons);
                        break;
                    case 4:
                        // AZUL
                        al_draw_bitmap_region(prismaClaro, 600, 0, 200, 200, 425, 300, 0);
                        tocar_som(4,sons);
                        break;
                    default:
                        break;
                }
                
                al_flip_display();

                double start_time = al_get_time();
                while (al_get_time() - start_time < 0.2) {
                    while (al_get_next_event(event_queue, &event)) {
                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            running = false;
                            errou = 1;
                            break;
                        }
                    }
                    if (!running) break;
                }

                desenhar_prisma(prismaPadrao);
                al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);
                al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);
                al_flip_display();

                start_time = al_get_time();
                while (al_get_time() - start_time < 0.3) {
                    while (al_get_next_event(event_queue, &event)) {
                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            running = false;
                            errou = 1;
                            break;
                        }
                    }
                    if (!running) break;
                }

                if (!running) break;
            }

            while (!is_empty(fila_original)) {
                dequeue(fila_original, &saiu);
                enqueue(fila_copia, saiu);
            }

            al_flush_event_queue(event_queue);

            //WHILE DA ENTRADA DO USUÁRIO
            while (!is_empty(sequencia)) {
                dequeue(sequencia, &esperado);

                bool entrada_valida = false;
                int entrada = 0;

                al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);
                al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);
                char texto_sua_vez[50];
                sprintf(texto_sua_vez, "Sua vez");
                int x_texto_sua_vez = (800 - al_get_text_width(font2, texto_sua_vez)) / 2;
                al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_sua_vez, y_texto+50, 0, texto_sua_vez);
                al_flip_display();

                while (!entrada_valida) {
                    al_wait_for_event(event_queue, &event);

                    if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        running = false;
                        errou = 1;
                        break;
                    }
                    if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                        entrada = identificar_cor_teclado(event.keyboard.keycode);
                        entrada_valida = (entrada > 0 && entrada <= 4);
                    }
                    else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                        entrada = identificar_cor_mouse(event.mouse.x, event.mouse.y);
                        entrada_valida = (entrada > 0 && entrada <= 4);
                    }
                }

                piscar_entrada(entrada, prismaClaro, prismaPadrao, sons);

                if (entrada == esperado) {
                    printf("OK\n");
                    pontuacao += 5; // A CADA ACERTO + 5 PONTOS
                    sprintf(texto_pontuacao, "%d pontos", pontuacao);
                } else {
                    if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                        running = false;
                        errou = 1;
                        break;
                    }
                    char texto_erro[50];
                    char texto_enter[50];
                    sprintf(texto_erro, "ERROU");
                    int x_texto_erro = (800 - al_get_text_width(font2, texto_erro)) / 2;
                    al_draw_text(font2, al_map_rgb(255, 87, 51), x_texto_erro, y_texto, 0, texto_erro);
                    sprintf(texto_enter, "Pressione ENTER para jogar novamente");
                    int x_texto_enter = (800 - al_get_text_width(font2, texto_enter)) / 2;
                    al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_enter, y_texto+50, 0, texto_enter);
                    // PONTUAÇÃO FINAL
                    sprintf(texto_pontuacao, "Você chegou até a rodada %d e terminou com %d pontos", rodada, pontuacao);
                    x_texto_pontuacao = (800 - al_get_text_width(font2, texto_pontuacao)) / 2;
                    al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);

                    al_flip_display();

                    ALLEGRO_SAMPLE_ID sample_id;
                    al_play_sample(sons[4], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
                    al_rest(0.5);
                    al_stop_sample(&sample_id);
                    
                    bool enter_pressionado = false;
                    while (!enter_pressionado) {
                        ALLEGRO_EVENT event;
                        al_wait_for_event(event_queue, &event);

                        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                                enter_pressionado = true;
                            }
                        }
                        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            running = false;
                            errou = 1;
                            enter_pressionado = true;
                            break;
                        }
                    }
                    errou = 1;
                    break;
                }
            }

            while (!is_empty(fila_copia)) {
                dequeue(fila_copia, &saiu);
                enqueue(fila_original, saiu);
            }
            rodada++;
            al_rest(0.5);
        }

        destroy(fila_copia);
        destroy(sequencia);
        destroy(fila_original);
    }

    for (int i = 0; i < 4; i++) {
        al_destroy_sample(sons[i]);
    }
    al_destroy_bitmap(prismaPadrao);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(tela);

    return 0;
}