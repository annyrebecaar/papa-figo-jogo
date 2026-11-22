// a fuga do papa-figo: jogo de terminal com tema nordestino
// o papa-figo é uma lenda do nordeste: um velho que pega criança na rua.
// voce precisa chegar em casa (C) antes que ele te alcance.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINHAS 11
#define COLUNAS 20

// guarda uma posição no mapa (linha e coluna)
struct Coordenada {
    int linha;
    int coluna;
};

// coragem e quantos passos o jogador deu
struct Status {
    int coragem;
    int passos;
};

// o jogador e o papa-figo usam essa struct
// aqui tem struct dentro de struct (aninhada): Coordenada e Status
struct Personagem {
    char nome[30];
    struct Coordenada posicao;
    struct Status status;
};

// o mapa do jogo (matriz de caracteres)
char mapa[LINHAS][COLUNAS + 2];

// limpa a tela (funciona no windows e no mac/linux)
void limparTela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

// lê o mapa do arquivo mapa.txt
// também acha onde começam o jogador (J) e o papa-figo (P)
int carregarMapa(struct Personagem *jogador, struct Personagem *papaFigo) {
    FILE *arquivo = fopen("mapa.txt", "r");
    if (arquivo == NULL) {
        printf("nao achei o arquivo mapa.txt!\n");
        return 0;
    }

    for (int i = 0; i < LINHAS; i++) {
        fgets(mapa[i], COLUNAS + 2, arquivo);
        mapa[i][COLUNAS] = '\0'; // tira o enter do fim da linha

        for (int j = 0; j < COLUNAS; j++) {
            if (mapa[i][j] == 'J') {
                jogador->posicao.linha = i;
                jogador->posicao.coluna = j;
                mapa[i][j] = '.';
            }
            if (mapa[i][j] == 'P') {
                papaFigo->posicao.linha = i;
                papaFigo->posicao.coluna = j;
                mapa[i][j] = '.';
            }
        }
    }

    fclose(arquivo);
    return 1;
}

// troca cada letra do mapa por um emoji na hora de mostrar
// (no mapa.txt continuam letras, porque a matriz é de char)
void desenharCasa(char c) {
    if (c == '#') printf("⛔️");
    else if (c == 'R') printf("🍯");
    else if (c == 'C') printf("🏠");
    else printf("  "); // rua: dois espaços, que é a largura de um emoji
}

// desenha o mapa com o jogador e o papa-figo por cima
void desenharMapa(struct Personagem jogador, struct Personagem papaFigo) {
    limparTela();
    printf("=== A FUGA DO PAPA-FIGO ===\n\n");

    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (i == jogador.posicao.linha && j == jogador.posicao.coluna) {
                printf("🏃");
            } else if (i == papaFigo.posicao.linha && j == papaFigo.posicao.coluna) {
                printf("👹");
            } else {
                desenharCasa(mapa[i][j]);
            }
        }
        printf("\n");
    }

    printf("\n%s | coragem: %d | passos: %d\n", jogador.nome, jogador.status.coragem, jogador.status.passos);
    printf("🏃 voce   👹 papa-figo   🍯 rapadura (+5 coragem)   🏠 casa   ⛔️ parede\n");
}

// diz se dá pra andar nessa posição (não é parede)
int podeAndar(int linha, int coluna) {
    if (mapa[linha][coluna] == '#') {
        return 0;
    }
    return 1;
}

// move o jogador conforme a tecla (w, a, s, d)
void moverJogador(struct Personagem *jogador, char tecla) {
    int novaLinha = jogador->posicao.linha;
    int novaColuna = jogador->posicao.coluna;

    if (tecla == 'w') novaLinha--;
    if (tecla == 's') novaLinha++;
    if (tecla == 'a') novaColuna--;
    if (tecla == 'd') novaColuna++;

    if (podeAndar(novaLinha, novaColuna)) {
        jogador->posicao.linha = novaLinha;
        jogador->posicao.coluna = novaColuna;
        jogador->status.passos++;
        jogador->status.coragem--; // cada passo dá medo
    }
}

// se pisou numa rapadura, ganha coragem e a rapadura some
void pegarRapadura(struct Personagem *jogador) {
    int l = jogador->posicao.linha;
    int c = jogador->posicao.coluna;

    if (mapa[l][c] == 'R') {
        jogador->status.coragem += 5;
        mapa[l][c] = '.';
    }
}

// o papa-figo anda 1 casa na direção do jogador
void moverPapaFigo(struct Personagem *papaFigo, struct Personagem jogador) {
    int l = papaFigo->posicao.linha;
    int c = papaFigo->posicao.coluna;

    // primeiro tenta chegar mais perto na linha
    if (jogador.posicao.linha < l && podeAndar(l - 1, c)) {
        papaFigo->posicao.linha--;
    } else if (jogador.posicao.linha > l && podeAndar(l + 1, c)) {
        papaFigo->posicao.linha++;
    }
    // se não deu, tenta pela coluna
    else if (jogador.posicao.coluna < c && podeAndar(l, c - 1)) {
        papaFigo->posicao.coluna--;
    } else if (jogador.posicao.coluna > c && podeAndar(l, c + 1)) {
        papaFigo->posicao.coluna++;
    }
}

// devolve 1 se os dois estão no mesmo lugar
int foiPego(struct Personagem jogador, struct Personagem papaFigo) {
    return jogador.posicao.linha == papaFigo.posicao.linha &&
           jogador.posicao.coluna == papaFigo.posicao.coluna;
}

// salva o nome e os passos no ranking.txt (no final do arquivo)
void salvarRanking(struct Personagem jogador) {
    FILE *arquivo = fopen("ranking.txt", "a");
    if (arquivo == NULL) {
        printf("nao deu pra salvar o ranking.\n");
        return;
    }
    fprintf(arquivo, "%s %d\n", jogador.nome, jogador.status.passos);
    fclose(arquivo);
}

// lê o ranking.txt e mostra quem fugiu
void mostrarRanking() {
    FILE *arquivo = fopen("ranking.txt", "r");
    char nome[30];
    int passos;

    limparTela();
    printf("=== RANKING (quem chegou em casa) ===\n\n");

    if (arquivo == NULL) {
        printf("ninguem fugiu ainda. seja o primeiro!\n");
        return;
    }

    while (fscanf(arquivo, "%29s %d", nome, &passos) == 2) {
        printf("%-15s %d passos\n", nome, passos);
    }
    fclose(arquivo);
}

// mostra as regras
void comoJogar() {
    limparTela();
    printf("=== COMO JOGAR ===\n\n");
    printf("voce esta voltando pra casa de noite e o papa-figo esta na rua!\n");
    printf("- ande com w (cima), a (esquerda), s (baixo), d (direita) e aperte enter\n");
    printf("- cada passo gasta 1 de coragem. sem coragem, voce trava de medo\n");
    printf("- rapadura (🍯) da +5 de coragem\n");
    printf("- chegue em casa (🏠) sem ser pego pelo papa-figo (👹)\n");
    printf("- digite q para desistir\n");
}

// uma partida inteira
void jogar() {
    struct Personagem jogador;
    struct Personagem papaFigo;
    char tecla;

    printf("qual o seu nome (sem espaco)? ");
    scanf("%29s", jogador.nome);

    jogador.status.coragem = 30;
    jogador.status.passos = 0;
    strcpy(papaFigo.nome, "Papa-Figo");

    if (!carregarMapa(&jogador, &papaFigo)) {
        return;
    }

    while (1) {
        desenharMapa(jogador, papaFigo);
        printf("\nsua jogada: ");
        if (scanf(" %c", &tecla) != 1) {
            return; // acabou a entrada
        }

        if (tecla == 'q') {
            printf("voce desistiu...\n");
            return;
        }

        moverJogador(&jogador, tecla);
        pegarRapadura(&jogador);

        // o papa-figo anda a cada 3 passos do jogador (se for mais rapido fica impossivel)
        if (jogador.status.passos % 3 == 0) {
            moverPapaFigo(&papaFigo, jogador);
        }

        if (foiPego(jogador, papaFigo)) {
            desenharMapa(jogador, papaFigo);
            printf("\nO PAPA-FIGO TE PEGOU! fim de jogo.\n");
            return;
        }

        if (jogador.status.coragem <= 0) {
            desenharMapa(jogador, papaFigo);
            printf("\nvoce travou de medo no meio da rua... fim de jogo.\n");
            return;
        }

        if (mapa[jogador.posicao.linha][jogador.posicao.coluna] == 'C') {
            desenharMapa(jogador, papaFigo);
            printf("\nVOCE CHEGOU EM CASA! fugiu em %d passos.\n", jogador.status.passos);
            salvarRanking(jogador);
            return;
        }
    }
}

// o main só mostra o menu e chama as funções
int main() {
    int opcao;

    do {
        printf("\n=== A FUGA DO PAPA-FIGO ===\n");
        printf("1 - jogar\n");
        printf("2 - ver ranking\n");
        printf("3 - como jogar\n");
        printf("0 - sair\n");
        printf("opcao: ");

        // se digitar letra em vez de numero, limpa o que foi digitado
        if (scanf("%d", &opcao) != 1) {
            if (feof(stdin)) break; // acabou a entrada, sai do jogo
            while (getchar() != '\n');
            opcao = -1;
        }

        if (opcao == 1) jogar();
        else if (opcao == 2) mostrarRanking();
        else if (opcao == 3) comoJogar();
        else if (opcao != 0) printf("opcao invalida.\n");

    } while (opcao != 0);

    printf("ate mais, e cuidado com o papa-figo!\n");
    return 0;
}
