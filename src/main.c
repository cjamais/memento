#include "../lib/addons.h"
#include "../lib/jogo.h"
#include "../lib/queue.h"
#define MAX_RODADAS 5

int main() {

    //PREPARAÇÃO DO AMBIENTE
    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        return -1;
    }

    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(9);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY* tela = al_create_display(800, 800);
    ALLEGRO_SAMPLE* sons[9];
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
    ALLEGRO_BITMAP* home = al_load_bitmap("assets/img/home.png");
    ALLEGRO_BITMAP* restart = al_load_bitmap("assets/img/restart.png");
    ALLEGRO_BITMAP* play_button = al_load_bitmap("assets/img/play.png");
    ALLEGRO_BITMAP* play_button_clear = al_load_bitmap("assets/img/play_claro.png");
    ALLEGRO_BITMAP* controles = al_load_bitmap("assets/img/controles.png");
    ALLEGRO_BITMAP* logo = al_load_bitmap("assets/img/inicio.png");
    ALLEGRO_FONT* font2 = al_load_font("assets/fonts/LeagueSpartan-Medium.ttf", 24, 0);
    ALLEGRO_BITMAP* icone = al_load_bitmap("assets/img/icone.png");
    ALLEGRO_BITMAP* cjamais = al_load_bitmap("assets/img/cjamais.png");

    al_set_display_icon(tela, icone);
    bool window_running = true;

    al_clear_to_color(al_map_rgb(255, 255, 255));
    al_draw_bitmap(cjamais, 150, 150, 0);
    al_flip_display();  // Atualiza a tela

    ALLEGRO_SAMPLE_ID cjamais_sound;
    al_play_sample(sons[6], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &cjamais_sound);

    // Guarda o tempo inicial
    float start_time = al_get_time(); // Tempo inicial (momento em que a música começa)

    // Enquanto o tempo não passar 5 segundos, continua a exibir a imagem e tocar a música
    while (al_get_time() - start_time < 5.0) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);  // Espera por eventos, como teclas ou clique

        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                break;
            }
        }

        // Verifica o clique do mouse
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.x >= 0 && event.mouse.x <= 800 &&
                event.mouse.y >= 0 && event.mouse.y <= 800) {
                break;
            }
        }

        al_rest(0.01); // Breve pausa para evitar alta carga da CPU
    }

    // Após 5 segundos, para a música
    al_stop_sample(&cjamais_sound);

    do {
        al_clear_to_color(al_map_rgb(255, 255, 255));
    
        int x_inicio = (800 - al_get_bitmap_width(logo)) / 2;
        int y_inicio = (800 - al_get_bitmap_height(logo)) / 2;
    
        char texto_inicio[50];
        sprintf(texto_inicio, "Aperte ENTER ou clique no botão para jogar");
        int x_texto = (800 - al_get_text_width(font2, texto_inicio)) / 2;
    
        // Desenha o logo
        al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);
    
        // Desenha o texto
        al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_inicio + 150, 0, texto_inicio);
    
        // Obtém o tamanho do botão "Play"
        int play_width = al_get_bitmap_width(play_button);
        int play_height = al_get_bitmap_height(play_button);
    
        // Centraliza o botão Play abaixo do texto
        int play_x = (800 - play_width) / 2;
        int play_y = y_inicio + 200;
    
        // Desenha o botão Play
        al_draw_bitmap(play_button, play_x, play_y, 0);
    
        al_flip_display();
    
        // Toca a música de fundo
        ALLEGRO_SAMPLE_ID background_song;
        al_play_sample(sons[5], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &background_song);
    
        bool enter_start = false;
        bool game_running = false;
    
        while (!enter_start) {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);
    
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    enter_start = true;
                    game_running = true;
    
                    // Efeito visual ao pressionar ENTER
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);
                    al_draw_text(font2, al_map_rgb(200, 200, 200), x_texto, y_inicio + 150, 0, texto_inicio);
                    al_draw_bitmap(play_button_clear, play_x, play_y, 0);
                    al_flip_display();
                    al_rest(0.1);
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);
                    al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_inicio + 150, 0, texto_inicio);
                    al_draw_bitmap(play_button, play_x, play_y, 0);
                    al_flip_display();
                    al_stop_sample(&background_song);

                    al_draw_bitmap(controles, 0, 0, 0);
                    al_flip_display();
                    al_rest(3);
                }
            }
    
            // Verifica o clique do mouse
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.x >= play_x && event.mouse.x <= play_x + play_width &&
                    event.mouse.y >= play_y && event.mouse.y <= play_y + play_height) {
                    enter_start = true;
                    game_running = true;    
                    // Efeito visual ao clicar no botão Play
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);
                    al_draw_text(font2, al_map_rgb(200, 200, 200), x_texto, y_inicio + 150, 0, texto_inicio);
                    al_flip_display();
                    al_rest(0.1);
                    al_clear_to_color(al_map_rgb(255, 255, 255));
                    al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);
                    al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_inicio + 150, 0, texto_inicio);
                    al_flip_display();
                    al_stop_sample(&background_song);
                    
                    al_draw_bitmap(controles, 0, 0, 0);
                    al_flip_display();
                    al_rest(3);
                }
            }
    
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                window_running = false;
                break;
            }    
        }
    

        //LOOP PRINCIPAL DO JOGO
        while (game_running) {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);

            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                game_running = false;
                window_running = false;
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
                                game_running = false;
                                window_running = false;
                                errou = 1;
                                break;
                            }
                        }
                        if (!game_running) break;
                    }

                    desenhar_prisma(prismaPadrao);
                    al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);
                    al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);
                    al_flip_display();

                    start_time = al_get_time();
                    while (al_get_time() - start_time < 0.3) {
                        while (al_get_next_event(event_queue, &event)) {
                            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                game_running = false;
                                window_running = false;
                                errou = 1;
                                break;
                            }
                        }
                        if (!game_running) break;
                    }

                    if (!game_running) break;
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
                    al_draw_text(font2, al_map_rgb(77, 149, 151), x_texto_sua_vez, y_texto+50, 0, texto_sua_vez);
                    al_flip_display();

                    while (!entrada_valida) {
                        al_wait_for_event(event_queue, &event);

                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            game_running = false;
                            window_running = false;
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
                        pontuacao += 5; // A CADA ACERTO + 5 PONTOS
                        sprintf(texto_pontuacao, "%d pontos", pontuacao);
                    } else {
                        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            game_running = false;
                            window_running = false;
                            errou = 1;
                            break;
                        }
                        char texto_erro[50];
                        sprintf(texto_erro, "ERROU");
                        int x_texto_erro = (800 - al_get_text_width(font2, texto_erro)) / 2;
                        al_draw_text(font2, al_map_rgb(255, 87, 51), x_texto_erro, y_texto, 0, texto_erro);
                        // PONTUAÇÃO FINAL
                        sprintf(texto_pontuacao, "Você chegou até a rodada %d e terminou com %d pontos", rodada, pontuacao);
                        x_texto_pontuacao = (800 - al_get_text_width(font2, texto_pontuacao)) / 2;
                        al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto+50, 0, texto_pontuacao);
                        ALLEGRO_SAMPLE_ID sample_id;
                        al_play_sample(sons[4], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
                        al_rest(0.5);
                        al_stop_sample(&sample_id);
                        
                        int restart_x = 450, restart_y = 620;
                        int home_x = 250, home_y = 620;

                        // Obtém tamanho das imagens
                        int restart_width = al_get_bitmap_width(restart);
                        int restart_height = al_get_bitmap_height(restart);
                        int home_width = al_get_bitmap_width(home);
                        int home_height = al_get_bitmap_height(home);

                        al_draw_bitmap(restart, restart_x, restart_y, 0);
                        al_draw_bitmap(home, home_x, home_y, 0);
                        al_flip_display();

                        bool opcao_escolhida = false;
                        while (!opcao_escolhida) {
                            ALLEGRO_EVENT event;
                            al_wait_for_event(event_queue, &event);
                            
                            // Captura cliques do mouse
                            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                                int mouse_x = event.mouse.x;
                                int mouse_y = event.mouse.y;

                                // Verifica se o clique foi na imagem "Restart"
                                if (mouse_x >= restart_x && mouse_x <= restart_x + restart_width &&
                                    mouse_y >= restart_y && mouse_y <= restart_y + restart_height) {
                                    opcao_escolhida = true;
                                }

                                // Verifica se o clique foi na imagem "Home"
                                if (mouse_x >= home_x && mouse_x <= home_x + home_width &&
                                    mouse_y >= home_y && mouse_y <= home_y + home_height) {
                                    opcao_escolhida = true;
                                    game_running = false; // Sai do jogo
                                }
                            }

                            // Captura teclas do teclado
                            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                                    opcao_escolhida = true;
                                } else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                                    opcao_escolhida = true;
                                    game_running = false;
                                }
                            }

                            // Fecha o jogo se a janela for fechada
                            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                opcao_escolhida = true;
                                game_running = false;
                                window_running = false;
                                errou = 1;
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
            if (rodada > MAX_RODADAS) {
                ALLEGRO_SAMPLE_ID violins;
                ALLEGRO_SAMPLE_ID victory;
                al_play_sample(sons[7], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &violins);
            
                // Carregar as imagens
                ALLEGRO_BITMAP* v1 = al_load_bitmap("assets/img/victory/1.png");
                ALLEGRO_BITMAP* v2 = al_load_bitmap("assets/img/victory/2.png");
                ALLEGRO_BITMAP* v3 = al_load_bitmap("assets/img/victory/3.png");
                ALLEGRO_BITMAP* v4 = al_load_bitmap("assets/img/victory/4.png");
                ALLEGRO_BITMAP* v5 = al_load_bitmap("assets/img/victory/5.png");
                ALLEGRO_BITMAP* v6 = al_load_bitmap("assets/img/victory/6.png");
                ALLEGRO_BITMAP* v7 = al_load_bitmap("assets/img/victory/7.png");
                ALLEGRO_BITMAP* v8 = al_load_bitmap("assets/img/victory/8.png");
                ALLEGRO_BITMAP* v9 = al_load_bitmap("assets/img/victory/9.png");
                ALLEGRO_BITMAP* v10 = al_load_bitmap("assets/img/victory/10.png");
            
                al_clear_to_color(al_map_rgb(0, 0, 0));
                al_flip_display();
            
                // Controle de tempo
                double start_time;
            
                // Exibe a primeira imagem por 1 segundo
                start_time = al_get_time();
                al_draw_bitmap(v1, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.0); // Espera 1 segundo
            
                // Exibe a segunda imagem por 2 segundos
                start_time = al_get_time();
                al_draw_bitmap(v2, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 2.0); // Espera 2 segundos
            
                // Exibe a terceira imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v3, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0); // Espera 3 segundos
            
                // Exibe a quarta imagem por 2 segundos
                start_time = al_get_time();
                al_draw_bitmap(v4, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 2.0); // Espera 2 segundos
            
                // Exibe a quinta imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v5, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0); // Espera 3 segundos
            
                al_stop_sample(&violins);
                al_play_sample(sons[8], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &victory);
            
                // Exibe a sexta imagem por 1.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v6, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.6); // Espera 1.6 segundos
            
                // Exibe a sétima imagem por 0.5 segundos
                start_time = al_get_time();
                al_draw_bitmap(v7, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 0.5); // Espera 0.5 segundos
            
                // Exibe a oitava imagem por 0.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v8, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 0.6); // Espera 0.6 segundos
            
                // Exibe a nona imagem por 1.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v9, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.6); // Espera 1.6 segundos
            
                // Exibe a décima imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v10, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0); // Espera 3 segundos
            
                al_stop_sample(&victory);
            
                // Destruir as imagens
                al_destroy_bitmap(v1);
                al_destroy_bitmap(v2);
                al_destroy_bitmap(v3);
                al_destroy_bitmap(v4);
                al_destroy_bitmap(v5);
                al_destroy_bitmap(v6);
                al_destroy_bitmap(v7);
                al_destroy_bitmap(v8);
                al_destroy_bitmap(v9);
                al_destroy_bitmap(v10);
            
                game_running = false;
                break;
            }            
            destroy(fila_copia);
            destroy(sequencia);
            destroy(fila_original);
        }
    }while(window_running);

    for (int i = 0; i < 9; i++) {
        al_destroy_sample(sons[i]);
    }
    al_destroy_bitmap(prismaPadrao);
    al_destroy_bitmap(restart);
    al_destroy_bitmap(home);
    al_destroy_bitmap(play_button);
    al_destroy_bitmap(play_button_clear);
    al_destroy_bitmap(controles);
    al_destroy_bitmap(cjamais);
    al_destroy_font(font);
    al_destroy_font(font2);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(tela);

    return 0;
}