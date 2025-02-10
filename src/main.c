//inicializando bibliotecas próprias
#include "../lib/addons.h"
#include "../lib/jogo.h"
#include "../lib/queue.h"
#define MAX_RODADAS 30 //define o tamanho máximo de rodadas

int main() {
    //PREPARAÇÃO DO AMBIENTE
    //inicializando allegro
    if (!al_init()) {
        printf("Erro ao inicializar o Allegro.\n");
        return -1;
    }
    
    //instalando e separando canais de áudio
    al_install_audio();
    al_init_acodec_addon();
    al_reserve_samples(9);

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue(); //criando fila de eventos
    ALLEGRO_FONT* font = al_create_builtin_font(); //fonte padrão do allegro
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
    ALLEGRO_DISPLAY* tela = al_create_display(800, 800);//tela definida com tamanho de 800x800px
    ALLEGRO_SAMPLE* sons[9];//variável que guardará os sons
    inicializar_sons(sons);//inicializando os sons

    int posicoes[4][2];//posições referentes aos quadrados do prisma
    inicializar_posicoes(posicoes);//inicializando a posição dos quadrados para corresponder ao cliquee
    al_register_event_source(event_queue, al_get_display_event_source(tela));//registrando a tela na fila de eventos

    //verificando se não foi possível carregar algum dos recursos
    if (!event_queue || !font || !timer || !tela) {
        printf("Erro ao criar recursos do Allegro.\n");
        if (font) al_destroy_font(font);
        if (timer) al_destroy_timer(timer);
        if (event_queue) al_destroy_event_queue(event_queue);
        if (tela) al_destroy_display(tela);
        return -1;
    }

    //iniciando todos os addons utilizados
    iniciar_addons(tela, event_queue, font, timer);

    //inicialização das imagens utilizadas no jogo
    ALLEGRO_BITMAP* prismaPadrao = al_load_bitmap("assets/img/prisma.png");
    ALLEGRO_BITMAP* prismaClaro = al_load_bitmap("assets/img/prisma_claro.png");
    ALLEGRO_BITMAP* home = al_load_bitmap("assets/img/home.png");
    ALLEGRO_BITMAP* restart = al_load_bitmap("assets/img/restart.png");
    ALLEGRO_BITMAP* play_button = al_load_bitmap("assets/img/play.png");
    ALLEGRO_BITMAP* play_button_clear = al_load_bitmap("assets/img/play_claro.png");
    ALLEGRO_BITMAP* controles = al_load_bitmap("assets/img/controles.png");
    ALLEGRO_BITMAP* logo = al_load_bitmap("assets/img/inicio.png");
    ALLEGRO_BITMAP* icone = al_load_bitmap("assets/img/icone.png");
    ALLEGRO_BITMAP* cjamais = al_load_bitmap("assets/img/cjamais.png");
    
    //inicialização a fonte utilizada
    ALLEGRO_FONT* font2 = al_load_font("assets/fonts/LeagueSpartan-Medium.ttf", 24, 0);

    //definindo um ícone para a aplicação
    al_set_display_icon(tela, icone);
    
    //LOGO CJAMAIS - EXIBIDA APENAS QUANDO A APLICAÇÃO É ABERTA
    al_clear_to_color(al_map_rgb(255, 255, 255));//pinta o fundo de branco
    al_draw_bitmap(cjamais, 150, 150, 0);//desenha a logo da equipe
    al_flip_display();//atualiza a tela

    //carregando e tocando um som de inicialização apenas uma vez
    ALLEGRO_SAMPLE_ID cjamais_sound;
    al_play_sample(sons[6], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &cjamais_sound);

    //guarda o tempo inicial
    float start_time = al_get_time();//tempo inicial (momento em que a música começa)

    //enquanto o tempo não passar 5 segundos, continua a exibir a imagem e tocar a música
    while (al_get_time() - start_time < 5.0) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);//espera por eventos, como teclas ou clique

        //verifica se pressionaou enter e sai da exibição
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                break;
            }
        }

        //verifica o clique do mouse e sai da exibição
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.x >= 0 && event.mouse.x <= 800 &&
                event.mouse.y >= 0 && event.mouse.y <= 800) {
                break;
            }
        }
    }

    //após 5 segundos, para a música, se não receber o break antes
    al_stop_sample(&cjamais_sound);

    //definindo a tela como ativa
    bool window_running = true;

    //TELA ESTÁ RODANDO/ABERTA
    //vai repetir enquanto o usuário não fechar, mas nunca voltará pra exibição da logo da equipe
    do {
        //EXIBIÇÃO DO MENU
        al_clear_to_color(al_map_rgb(255, 255, 255));//pinta o fundo de branco
        
        //texto do menu - posição, conteúdo e fonte
        int x_inicio = (800 - al_get_bitmap_width(logo)) / 2;
        int y_inicio = (800 - al_get_bitmap_height(logo)) / 2;
        char texto_inicio[50];
        sprintf(texto_inicio, "Aperte ENTER ou clique no botão para jogar");
        int x_texto = (800 - al_get_text_width(font2, texto_inicio)) / 2;

        al_draw_bitmap(logo, x_inicio, y_inicio - 50, 0);//desenha a logo
        al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_inicio + 150, 0, texto_inicio);//desenha o texto do menu
        
        //botão de play - tamanho, posição e exibição
        int play_width = al_get_bitmap_width(play_button);
        int play_height = al_get_bitmap_height(play_button);
        int play_x = (800 - play_width) / 2;
        int play_y = y_inicio + 200;
        al_draw_bitmap(play_button, play_x, play_y, 0);
    
        al_flip_display();//atualiza a tela
    
        //toca a música tema até que o jogo comece
        ALLEGRO_SAMPLE_ID background_song;
        al_play_sample(sons[5], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, &background_song);
        
        //variáveis de controle que serão inicializadas quando o usuário apertar enter ou clicar para iniciar
        bool enter_start = false;
        bool game_running = false;
        
        //aguarda o clique no botão play ou a entrada de enter
        while (!enter_start) {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);//espera por um evento
            
            //se for o enter
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    //define variável para sair do while e entrar no jogo
                    enter_start = true;
                    game_running = true;
    
                    //efeito visual do texto e botão piscando
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

                    //mostra as opções de controles
                    al_draw_bitmap(controles, 0, 0, 0);
                    al_flip_display();
                    al_rest(2.5);
                }
            }
    
            //se o botão play for clicado
            if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                if (event.mouse.x >= play_x && event.mouse.x <= play_x + play_width &&
                    event.mouse.y >= play_y && event.mouse.y <= play_y + play_height) {
                    //define variável para sair do while e entrar no jogo
                    enter_start = true;
                    game_running = true; 

                    //efeito visual do texto e botão piscando
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
                    
                    //mostra as opções de controles
                    al_draw_bitmap(controles, 0, 0, 0);
                    al_flip_display();
                    al_rest(2.5);
                }
            }
            
            //se clicar no x, fecha a aplicação
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                window_running = false;
                break;
            }    
        }
    

        //LOOP PRINCIPAL DO JOGO
        //executado enquanto game_running = true;
        //caso game_running = false, volta pro menu
        while (game_running) {
            ALLEGRO_EVENT event;
            al_wait_for_event(event_queue, &event);//aguarda eventos

            //se clicar no x, fecha a aplicação
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

            //inicializando as 3 filas
            fila_original = init(MAX_RODADAS);
            sequencia = init(MAX_RODADAS);
            fila_copia = init(MAX_RODADAS);

            //gerando os números correspondentes às cores de forma aleatória
            srand(time(NULL));
            for (int i = 0; i < MAX_RODADAS; i++) {
                int random = (rand() % 4) + 1;
                enqueue(fila_original, random);
            }

            desenhar_prisma(prismaPadrao);//desenha o prisma "desligado"

            int rodada = 1;//inicializa a rodada como 1
            
            //roda a condição enquanto não atingir o limite de rodadas pré definido sem errar
            while (rodada <= MAX_RODADAS && !errou) {
                
                //mostrando o texto da rodada atual
                char texto_rodada[50];
                sprintf(texto_rodada, "RODADA %d", rodada);
                int y_texto = 50;
                int x_texto = (800 - al_get_text_width(font2, texto_rodada)) / 2;
                al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);

                //mostrando o texto da pontuação
                char texto_pontuacao[50];
                sprintf(texto_pontuacao, "%d pontos", pontuacao);
                int x_texto_pontuacao = (800 - al_get_text_width(font2, texto_pontuacao)) / 2; 
                int y_texto_pontuacao = 670; 
                al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);

                //aqui ocorre a lógica principal
                //o for retira da fila somente até o tamanho máximo da rodada
                //ou seja, se for rodada 1, retira só 1 número, se for 5, retira 5
                //retirar da fila significa piscar as cores na tela
                for (int i = 0; i < rodada; i++) {
                    dequeue(fila_original, &saiu);//tira da fila original e coloca na variável saiu
                    enqueue(sequencia, saiu);//coloca na fila de sequência
                    enqueue(fila_copia, saiu);//coloca na fila auxiliar

                    //exibindo as cores da rodada
                    //efeito visual de piscada: substitui o quadrado original por sua versão mais clara
                    //tocar_som: reproduz o som correspondente a cor através de uma função
                    switch (saiu) {
                        case 1:
                            //VERMELHO
                            al_draw_bitmap_region(prismaClaro, 0, 0, 200, 200, 300, 175, 0);
                            tocar_som(1,sons);
                            break;
                        case 2:
                            //AMARELO
                            al_draw_bitmap_region(prismaClaro, 200, 0, 200, 200, 300, 425, 0);
                            tocar_som(2,sons);
                            break;
                        case 3:
                            //VERDE
                            al_draw_bitmap_region(prismaClaro, 400, 0, 200, 200, 175, 300, 0);
                            tocar_som(3,sons);
                            break;
                        case 4:
                            //AZUL
                            al_draw_bitmap_region(prismaClaro, 600, 0, 200, 200, 425, 300, 0);
                            tocar_som(4,sons);
                            break;
                        default:
                            break;
                    }
                    
                    al_flip_display();//atualiza a tela
                    
                    //se clicar no x, fecha a aplicação
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

                    //volta para o prisma "desligado" após breve intervalo exibindo o mais claro
                    desenhar_prisma(prismaPadrao);
                    al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);
                    al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);
                    al_flip_display();//atualiza a tela

                    //se clicar no x, fecha a aplicação
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

                //aqui termina a lógica da fila
                //para não perder a sequência original é necessário que a fila original termine de ser desenfileirada
                //dessa forma garantimos que a fila irá se manter como no início, alocando os outros números para a fila auxiliar
                while (!is_empty(fila_original)) {
                    dequeue(fila_original, &saiu);
                    enqueue(fila_copia, saiu);
                }

                //garante que haja uma limpeza do buffer
                //liberando a entrada para o usuário apenas quando terminar de exibir as cores
                al_flush_event_queue(event_queue);

                //WHILE DA ENTRADA DO USUÁRIO
                //roda enquanto a fila sequência, que tem o tamanho da rodada, não estiver vazia
                while (!is_empty(sequencia)) {
                    //removendo da fila de sequência e colocando na variável esperado
                    //"esperado" corresponde ao número/cor que o usuário deve acertar
                    dequeue(sequencia, &esperado);

                    bool entrada_valida = false;
                    int entrada = 0;

                    //avisa que o usuário pode jogar escrevendo que é a vez dele
                    al_draw_text(font2, al_map_rgb(50, 50, 50), x_texto, y_texto, 0, texto_rodada);
                    al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto_pontuacao, 0, texto_pontuacao);
                    char texto_sua_vez[50];
                    sprintf(texto_sua_vez, "Sua vez");
                    int x_texto_sua_vez = (800 - al_get_text_width(font2, texto_sua_vez)) / 2;
                    al_draw_text(font2, al_map_rgb(77, 149, 151), x_texto_sua_vez, y_texto+50, 0, texto_sua_vez);
                    al_flip_display();

                    //roda enquanto o usuário não inserir uma entrada válida
                    //as entradas válidas são os cliques ou as teclas definidas
                    while (!entrada_valida) {
                        al_wait_for_event(event_queue, &event);//espero por eventos

                        //se clicar no x, fecha a aplicação
                        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            game_running = false;
                            window_running = false;
                            errou = 1;
                            break;
                        }
                        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                            entrada = identificar_cor_teclado(event.keyboard.keycode);//função que converte a tecla em número correspondente a cor
                            entrada_valida = (entrada > 0 && entrada <= 4);
                        }
                        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                            entrada = identificar_cor_mouse(event.mouse.x, event.mouse.y);//função que converte o clique em número correspondente a cor
                            entrada_valida = (entrada > 0 && entrada <= 4);
                        }
                    }

                    piscar_entrada(entrada, prismaClaro, prismaPadrao, sons);//faz uma animação de piscada e toca o som se a entrada do usuário for válida

                    //SE O USUÁRIO ACERTA ELE CONTINUA E SOMA 5 PONTOS
                    if (entrada == esperado) {
                        pontuacao += 5; //a cada certo +5 pontos
                        sprintf(texto_pontuacao, "%d pontos", pontuacao);//atualiza pontuação
                    } else {//SE NÃO, SIGNIFICA QUE ELE ERROU - GAME OVER
                        //se clicar em x, fecha a aplicação
                        if(event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                            game_running = false;
                            window_running = false;
                            errou = 1;
                            break;
                        }
                        //mostra que o usuário errou
                        char texto_erro[50];
                        sprintf(texto_erro, "ERROU");
                        int x_texto_erro = (800 - al_get_text_width(font2, texto_erro)) / 2;
                        al_draw_text(font2, al_map_rgb(255, 87, 51), x_texto_erro, y_texto, 0, texto_erro);

                        //exibe a pontuação final
                        sprintf(texto_pontuacao, "Você chegou até a rodada %d e terminou com %d pontos", rodada, pontuacao);
                        x_texto_pontuacao = (800 - al_get_text_width(font2, texto_pontuacao)) / 2;
                        al_draw_text(font2, al_map_rgb(100, 100, 100), x_texto_pontuacao, y_texto+50, 0, texto_pontuacao);

                        //toca o som de erro uma vez
                        ALLEGRO_SAMPLE_ID sample_id;
                        al_play_sample(sons[4], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &sample_id);
                        al_rest(0.5);
                        al_stop_sample(&sample_id);
                        
                        //desenha os ícones para recomeçar ou voltar ao menu
                        int restart_x = 450, restart_y = 620;
                        int home_x = 250, home_y = 620;
                        int restart_width = al_get_bitmap_width(restart);
                        int restart_height = al_get_bitmap_height(restart);
                        int home_width = al_get_bitmap_width(home);
                        int home_height = al_get_bitmap_height(home);
                        al_draw_bitmap(restart, restart_x, restart_y, 0);
                        al_draw_bitmap(home, home_x, home_y, 0);
                        al_flip_display();

                        bool opcao_escolhida = false;
                        //aguarda o usuário dizer se voltará ao menu ou recomeçará o jogo
                        while (!opcao_escolhida) {
                            ALLEGRO_EVENT event;
                            al_wait_for_event(event_queue, &event);//aguarda um evento
                            
                            //captura cliques do mouse
                            if(event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
                                int mouse_x = event.mouse.x;
                                int mouse_y = event.mouse.y;
                                //verifica se o clique foi na imagem "Restart"
                                if(mouse_x >= restart_x && mouse_x <= restart_x + restart_width &&
                                    mouse_y >= restart_y && mouse_y <= restart_y + restart_height){
                                    opcao_escolhida = true;
                                }
                                //verifica se o clique foi na imagem "Home"
                                if(mouse_x >= home_x && mouse_x <= home_x + home_width &&
                                    mouse_y >= home_y && mouse_y <= home_y + home_height){
                                    opcao_escolhida = true;
                                    game_running = false;//sai do jogo e volta ao menu
                                }
                            }
                            //captura teclas do teclado
                            if(event.type == ALLEGRO_EVENT_KEY_DOWN) {
                                if(event.keyboard.keycode == ALLEGRO_KEY_ENTER){
                                    opcao_escolhida = true;
                                }else if(event.keyboard.keycode == ALLEGRO_KEY_ESCAPE){
                                    opcao_escolhida = true;
                                    game_running = false;//sai do jogo e volta ao menu
                                }
                            }
                            //se clicar em x, fecha a aplicação
                            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                                opcao_escolhida = true;
                                game_running = false;
                                window_running = false;
                                errou = 1;
                                break;
                            }
                        }
                        errou = 1;//define errou como 1, outra condição de parada do jogo
                        break;
                    }
                }
                
                //devolve todos os números guardados na fila auxiliar para a fila original
                while (!is_empty(fila_copia)) {
                    dequeue(fila_copia, &saiu);
                    enqueue(fila_original, saiu);
                }
                //só passa para a próxima rodada se não errou
                if(!errou){
                    rodada++;
                }
                al_rest(0.5);
            }
            //CONDIÇÃO DE VITÓRIA
            //se o usuário passar do limite de rodadas definido significa que ele ganhou
            //aqui começa uma sequência de exibiçãoo de imagens
            //o uso de al_rest foi evitado porque ele bloqueia a entrada do usuário por muito tempo pós exibição da vitória
            if (rodada > MAX_RODADAS) {
                //inicia sons 
                ALLEGRO_SAMPLE_ID violins;
                ALLEGRO_SAMPLE_ID victory;

                //toca música de tensão somente uma vez
                al_play_sample(sons[7], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &violins);
            
                //carrega imagens
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

                al_clear_to_color(al_map_rgb(0, 0, 0));//pinta a tela de preto
                al_flip_display();
            
                //controle de tempo
                double start_time;
            
                //exibe a primeira imagem por 1 segundo
                start_time = al_get_time();
                al_draw_bitmap(v1, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.0); 
            
                //exibe a segunda imagem por 2 segundos
                start_time = al_get_time();
                al_draw_bitmap(v2, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 2.0);
            
                //exibe a terceira imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v3, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0);
            
                //exibe a quarta imagem por 2 segundos
                start_time = al_get_time();
                al_draw_bitmap(v4, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 2.0);
            
                //exibe a quinta imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v5, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0);
                
                //Finzaliza a música de tensão e toca uma música alegre
                al_stop_sample(&violins);
                al_play_sample(sons[8], 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, &victory);
                
                //o tempo se torna mais específico para sincronizar com o som e fazer um efeito de animação
                //exibe a sexta imagem por 1.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v6, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.6);
            
                //exibe a sétima imagem por 0.5 segundos
                start_time = al_get_time();
                al_draw_bitmap(v7, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 0.5);
            
                //exibe a oitava imagem por 0.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v8, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 0.6);
            
                //exibe a nona imagem por 1.6 segundos
                start_time = al_get_time();
                al_draw_bitmap(v9, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 1.6);
            
                //exibe a décima imagem por 3 segundos
                start_time = al_get_time();
                al_draw_bitmap(v10, 0, 0, 0);
                al_flip_display();
                while ((al_get_time() - start_time) < 3.0);
                
                //para a música de vitória
                al_stop_sample(&victory);
            
                //destrói as imagens
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
                
                //volta ao menu
                game_running = false;
                break;
            }
            //destrói as filas antes de voltar para o menu
            destroy(fila_copia);
            destroy(sequencia);
            destroy(fila_original);
        }

    }while(window_running);

    //destrói todos os recursos: imagem, som, fontes, timer, fila de eventos e tela que foram inicializados
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