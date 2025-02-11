# 🎮 Memento - Jogo da memória com Allegro 5
O seguinte projeto implementa o clássico jogo Genius/Simon utilizando a biblioteca Allegro 5 e conceitos aprendidos na disciplina de Estrutura de Dados. O jogo desafia o jogador a memorizar e repetir sequências de cores e sons, aumentando a dificuldade conforme o progresso.

Assista o vídeo no <a href="https://youtu.be/zmrpd9r-1Vw?si=CnUpuZlumaJ7TPWx">YouTube</a>. Se quiser ver apenas a execução do jogo, avance para 17:35.

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

### Troca de telas
O jogo é dividido entre camadas e seções de forma a exibir o conteúdo correspondente. Por exemplo, a logo da equipe só é exibida uma vez quando o jogo é inicializado, mas o menu pode ser acessado novamente quando o usuário erra a sequência da rodada. Foram utilizadas estruturas como `while` e `do-while`, além de algumas flags para garantir a "navegação".
![image](https://github.com/user-attachments/assets/3e846d8c-cf57-4f9c-be9d-1bda23711224)

## ♟️Estratégia de implementação e metodologia
Antes da implementação com o Allegro, rascunhamos e desenvolvemos separadamente a lógica base do jogo (pasta `logica_genius`), realizando testes para verificar se o TAD fila foi implementado da melhor maneira.

![image](https://github.com/user-attachments/assets/609020ac-9970-408e-8f55-e958b6e0eff9)

A partir disso, dividimos as responsabilidades: cada um ficou encarregado de aprender e compartilhar conhecimentos sobre diferentes recursos da biblioteca, como eventos de clique e teclado, troca de telas, animação e saída de áudio. Essa divisão teve como objetivo aprofundar o conhecimento na biblioteca e torná-la mais familiar. Com as peças prontas, bastou encaixá-las. O processo demorou um pouco, mas reuniões para compartilhar e discutir a implementação de certos recursos ajudaram a resolver os desafios.

A princípio, utilizamos a IDE Visual Studio para trabalhar, devido à facilidade de integração com a biblioteca Allegro. No entanto, isso logo se tornou um empecilho, tanto pela falta de familiaridade quanto pela incompatibilidade com os sistemas operacionais dos integrantes. A migração para o Visual Studio Code foi essencial, e o uso do controle de versão Git, junto com o repositório no GitHub, nos permitiu acompanhar o andamento do desenvolvimento do jogo de forma organizada.

## 🖼️ Telas
<img src="https://github.com/user-attachments/assets/d1f656e7-b75f-47c8-9108-d19917a7333a" width="200">
<img src="https://github.com/user-attachments/assets/6b01eb4e-f22c-4905-b4f3-55081eafd88b" width="200">
<img src="https://github.com/user-attachments/assets/eb61382e-8d82-4f2c-bf30-87bc75c5b68f" width="200"><br>
<img src="https://github.com/user-attachments/assets/b8104a3c-b558-4f76-9642-709d5b7fe4bc" width="200">
<img src="https://github.com/user-attachments/assets/89c81291-0144-4e38-a3e3-354df0638f86" width="200">
<img src="https://github.com/user-attachments/assets/2ea5c568-2de4-4714-bcb4-513eafafe2f5" width="200"><br>
<img src="https://github.com/user-attachments/assets/e290931e-05ee-4cf7-9bfa-8870901e605a" width="200">
<img src="https://github.com/user-attachments/assets/33885bc4-f155-4c2d-a2dd-0ba27eceefe8" width="200">
<img src="https://github.com/user-attachments/assets/cacdcdd9-33cc-40d4-ba69-58dd08815b7c" width="200">

## 📥 Instalação
### Windows 🪟
1. Certifique-se de ter o **gcc** e o **Make** instalados
2. Clone este repositório:
   
   ```bash
   https://github.com/cjamais/memento.git
4. Acesse o terminal, digite `make run`, aperte enter e espere o jogo carregar.

#### Alternativa sem Make
Ao invés de utilizar o make no terminal, digite
1. `gcc -I allegro\include -c src/*.c`
2. `gcc -I allegro\include *.o -o Memento.exe liballegro_monolith.dll.a`<br>
3. Rode o executável gerado com `.\Memento.exe`

#### Outra alternativa
Ao clonar o repositório, um arquivo executável (Memento.exe) será baixado. Você pode jogar através dessa última versão compilada. 

### Linux 🐧
1. Certifique-se de ter o **gcc** e o **Make** instalados
2. Clone este repositório:
   
   ```bash
   https://github.com/cjamais/memento.git
4. Acesse o terminal, digite `make run`, aperte enter e espere o jogo carregar.

### macOS 🍎
1. Certifique-se de ter o **gcc** e o **Make** instalados
2. Clone este repositório:
   
   ```bash
   https://github.com/cjamais/memento.git
4. Acesse o terminal, digite `make run`, aperte enter e espere o jogo carregar.

##  📚 Sobre o Allegro 5
O Allegro 5 é uma biblioteca gráfica poderosa para desenvolvimento de jogos em C/C++. Suporta gráficos, áudio, entrada de usuário e manipulação de eventos, tornando o desenvolvimento mais simples e eficiente.

## 👨‍💻 Autores
- [Gabriel Albino](https://www.linkedin.com/in/gabrielalbino05/)
- [Gabrielle Ulisses](https://www.linkedin.com/in/gabrielle-ulisses/)
- [Otávio Baroni](https://www.linkedin.com/in/otaviobaroni/)
- [Rafael Matias](https://www.linkedin.com/in/rafael-matias-dev/)

