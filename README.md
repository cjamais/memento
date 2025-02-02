# 🎮 Jogo Genius com Allegro 5

## 📌 Descrição

Este projeto implementa o clássico jogo Genius utilizando a biblioteca Allegro 5. O jogo desafia o jogador a memorizar e repetir sequências de cores e sons, aumentando a dificuldade conforme o progresso.

## 🛠️ Tecnologias Utilizadas

- **Linguagem:** C
- **Biblioteca:** Allegro 5
- **Estruturas de Dados:** Fila (Queue) para armazenar sequências

## 📜 Funcionalidades

- Exibição gráfica das cores do jogo
- Reprodução de sons para cada cor
- Captura de entrada via teclado e mouse
- Validação de sequência digitada pelo jogador
- Geração de novas sequências aleatórias conforme o progresso

## 📥 Instalação

1. Certifique-se de ter o Allegro 5 instalado em seu sistema.
2. Clone este repositório:
   ```sh
   git clone https://github.com/seu-usuario/genius-allegro5.git
   cd genius-allegro5
3. Compile o código com:
  gcc -o genius main.c queue.c genius.c -lallegro -lallegro_primitives -lallegro_audio -lallegro_acodec
4. Execute o jogo:
./genius


##  📚 Sobre o Allegro 5
O Allegro 5 é uma biblioteca gráfica poderosa para desenvolvimento de jogos em C/C++. Suporta gráficos, áudio, entrada de usuário e manipulação de eventos, tornando o desenvolvimento mais simples e eficiente.