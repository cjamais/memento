# 🎮 Memento - Jogo da memória com Allegro 5
O seguinte projeto implementa o clássico jogo Genius/Simon utilizando a biblioteca Allegro 5 e conceitos aprendidos na disciplina de Estrutura de Dados. O jogo desafia o jogador a memorizar e repetir sequências de cores e sons, aumentando a dificuldade conforme o progresso.

## 🛠️ Tecnologias e ferramentas

- Estrutura de dados: Fila (Queue) 🛒➡️
- Linguagem de programação: ©️
- Biblioteca: Allegro 5 🐊
- Ambiente de desenvolvimento: Visual Studio e Visual Studio Code 💻

## 📜 Funcionalidades

- Geração de sequências de cores
- Exibição gráfica das cores do jogo
- Reprodução de sons para cada cor
- Captura de entrada via teclado e mouse
- Validação de sequência digitada pelo jogador

## ⚙️ Mecânica do jogo
### Filas
**Filas** (*Queues*) são estruturas de dados lineares que seguem o princípio FIFO (First In, First Out), onde o primeiro elemento a ser inserido é o primeiro a ser removido. Elas são amplamente usadas para organizar e controlar o fluxo de dados em sistemas onde a ordem de processamento precisa ser preservada. O jogo utiliza três filas distintas:

*Fila Original* - Armazena a sequência principal, contendo números aleatórios de 1 a 4, que representam as cores. Seu tamanho é definido pelo valor máximo determinado pelo programa.

*Fila de Sequência* - Esta fila armazena a quantidade de elementos correspondente à rodada em andamento. O número de elementos nesta fila aumenta a cada rodada, ou seja, na primeira rodada há 1 elemento, na segunda rodada há 2, na terceira rodada há 3, e assim por diante. A fila de rodadas é inicializada com o mesmo tamanho máximo da fila principal, mas seu conteúdo é modificado a cada nova rodada, conforme o número de elementos a ser apresentado ao jogador.

*Fila Auxiliar* - A função dessa fila é auxiliar no processo de preservação da fila principal durante o jogo, garantindo que os dados da fila original sejam mantidos intactos.
### Fluxo de execução
O jogo começa com um contador de rodadas inicializado em 1, e o processo se repete até que o número máximo de rodadas, determinado pelo tamanho máximo da fila, seja atingido. A cada rodada, o programa remove (desenfileira) o primeiro elemento da fila principal, que representa a cor da sequência a ser exibida. Esse número é então armazenado em uma variável chamada "saiu" e adicionado tanto na fila de sequência quanto na fila auxiliar. Enquanto isso, a cor correspondente ao número é exibida na tela, acompanhada de um som relacionado à cor.

Após a exibição da sequência, a fila principal termina de ser esvaziada, mas sem perder os dados originais: os elementos restantes da fila principal são movidos para a fila auxiliar, de forma que a fila original fique vazia, mas a sequência completa continue intacta na fila auxiliar.

O programa então libera a entrada do jogador. O jogador pode utilizar o mouse ou as setas do teclado, entradas essas que são convertidas em números correspondentes às cores. O número que o jogador deve acertar em cada rodada é retirado da fila de sequência e ele é armazenado em uma variável chamada "esperada". A cada entrada do jogador, o sistema verifica se o número corresponde ao que foi apresentado. Se o jogador acertar o número esperado, ele ganha 5 pontos para cada acerto, e ao terminar a rodada uma nova é iniciada. Caso o jogador erre, o jogo é encerrado imediatamente.

Ao final de cada rodada, a fila auxiliar é desenfileirada e seus elementos são reinseridos na fila principal, restaurando a sequência para que o jogo continue de onde parou. O contador de rodadas é incrementado em um, e o processo se repete até o final do jogo, seja pela vitória do jogador, ao completar todas as rodadas sem erros, ou pela derrota, caso ele cometa algum erro.

## 📥 Instalação

<!--- 1. Certifique-se de ter o Allegro 5 instalado em seu sistema.
2. Clone este repositório:
   ```https://github.com/cjamais/genius.git```
3. Compile o código com:
  gcc -o genius main.c queue.c genius.c -lallegro -lallegro_primitives -lallegro_audio -lallegro_acodec
4. Execute o jogo --->

### Windows 🪟
1. Certifique-se de ter o **gcc** e o **Make** instalados
2. Clone este repositório:
   ```bash
   https://github.com/cjamais/memento.git
3. Acesse o terminal, digite `make win` e aperte enter
4. Escreva `make play` e aperte enter
5. Um executável será criado, acesse-o em sua pasta ou pelo terminal escrevendo `.\Memento.exe`

#### Alternativa
Ao clonar o repositório, um arquivo executável (Memento.exe) será baixado. Você pode jogar através dessa última versão compilada. 

##  📚 Sobre o Allegro 5
O Allegro 5 é uma biblioteca gráfica poderosa para desenvolvimento de jogos em C/C++. Suporta gráficos, áudio, entrada de usuário e manipulação de eventos, tornando o desenvolvimento mais simples e eficiente.

## 👨‍💻 Autores
- [Gabriel Albino](https://www.linkedin.com/in/gabrielalbino05/)
- [Gabrielle Ulisses](https://www.linkedin.com/in/gabrielle-ulisses/)
- [Otávio Baroni](https://www.linkedin.com/in/otaviobaroni/)
- [Rafael Matias](https://www.linkedin.com/in/rafael-matias-dev/)

