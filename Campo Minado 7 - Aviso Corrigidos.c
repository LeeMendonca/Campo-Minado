#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#define REVELADA -2 //?
#define MINA -1 //?
#define MARCADA -3  // Marca a célula como possível mina
#define MAX 20

int LINHAS, COLUNAS, MINAS;
int tabuleiro[MAX][MAX];
int visualizacao[MAX][MAX];
int modoMarcar = 0;  // 0 para revelar, 1 para marcar

void limparTela() {
    system("clear");
}

void imprimirRegras() {
    printf("\033[1;35m!!! BEM-VINDO(A) AO CAMPO MINADO !!!\033[0m\n\n");
    printf("\033[1;34mObjetivo:\033[0m Revelar todas as células sem minas.\n\n\n");
    printf("\033[1;34mInstruções:\033[0m\n\n");
    printf("\033[1;35m1.\033[0m Escolha o tamanho do tabuleiro e a quantidade de minas.\n");
    printf("\033[1;35m2.\033[0m Digite as coordenadas (linha e coluna) para revelar ou marcar uma mina.\n");
    printf("\033[1;35m3.\033[0m Se revelar uma mina, o jogo acaba! Se não, um número indicará quantas minas estão próximas.\n\n\n");
    printf("\033[1;34mBoa sorte!\033[0m\n");
    printf("\033[1;35m----------------------------------------------------------------------------------------------------\033[0m\n");
}

void inicializarTabuleiro() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            tabuleiro[i][j] = 0;
            visualizacao[i][j] = 0;
        }
    }

    int minasColocadas = 0;
    while (minasColocadas < MINAS) {
        int linha = rand() % LINHAS;
        int coluna = rand() % COLUNAS;

        if (tabuleiro[linha][coluna] != MINA) {
            tabuleiro[linha][coluna] = MINA;
            minasColocadas++;

            for (int i = linha - 1; i <= linha + 1; i++) {
                for (int j = coluna - 1; j <= coluna + 1; j++) {
                    if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS && tabuleiro[i][j] != MINA) {
                        tabuleiro[i][j]++;
                    }
                }
            }
        }
    }
}

void imprimirTabuleiro(int revelarTudo) {
    printf("\n     ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("%2d  ", j + 1);
    }
    printf("\n");

    printf("   ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("\033[1;30m----\033[0m");
    }
    printf("\033[1;30m---\033[0m\n");

    for (int i = 0; i < LINHAS; i++) {
        printf("%2d \033[1;30m|\033[0m", i + 1);

        for (int j = 0; j < COLUNAS; j++) {
            if (revelarTudo) {
                if (tabuleiro[i][j] == MINA && visualizacao[i][j] == MARCADA) {
                    printf("  \033[1;31mX\033[0m ");
                } else if (visualizacao[i][j] == MARCADA && tabuleiro[i][j] != MINA) {
                    printf("  \033[1;34m!\033[0m ");
                } else if (tabuleiro[i][j] == MINA) {
                    printf("  \033[1;31mX\033[0m ");
                } else {
                    printf(" %2d ", tabuleiro[i][j]);  // Exibe número de minas adjacentes
                }
            } else {
                if (visualizacao[i][j] == MARCADA) {
                    printf("  🚩 ");
                } else if (visualizacao[i][j] == REVELADA) {
                    if (tabuleiro[i][j] == MINA) {
                        printf("  \033[1;31mX\033[0m ");
                    } else {
                        printf(" %2d ", tabuleiro[i][j]);
                    }
                } else {
                    printf("  \033[1;30m?\033[0m ");
                }
            }
        }
        printf("\033[1;30m |\033[0m\n");
    }

    printf("   ");
    for (int j = 0; j < COLUNAS; j++) {
        printf("\033[1;30m----\033[0m");
    }
    printf("\033[1;30m---\033[0m\n");
}

int revelar(int linha, int coluna) {
    if (linha < 0 || linha >= LINHAS || coluna < 0 || coluna >= COLUNAS) {
        printf("Coordenadas inválidas!\n");
        return 0;
    }

    if (visualizacao[linha][coluna] == REVELADA) {
        printf("Essa posição já foi revelada!\n");
        return 0;
    }

    visualizacao[linha][coluna] = REVELADA;

    if (tabuleiro[linha][coluna] == MINA) {
        return -1;
    }

    if (tabuleiro[linha][coluna] == 0) {
        for (int i = linha - 1; i <= linha + 1; i++) {
            for (int j = coluna - 1; j <= coluna + 1; j++) {
                if (i >= 0 && i < LINHAS && j >= 0 && j < COLUNAS && visualizacao[i][j] != REVELADA) {
                    revelar(i, j);
                }
            }
        }
    }
    return 1;
}

int checarVitoria() {
    for (int i = 0; i < LINHAS; i++) {
        for (int j = 0; j < COLUNAS; j++) {
            if (tabuleiro[i][j] != MINA && visualizacao[i][j] != REVELADA) {
                return 0;
            }
        }
    }
    return 1;
}

int main() {
    setlocale(LC_ALL, "pt_BR.UTF-8");
    srand(time(NULL));

    imprimirRegras();
    printf("Pressione Enter para continuar...");
    getchar();
    limparTela();

    printf("Digite o \033[1;34mtamanho\033[0m do tabuleiro desejado (máximo %d): ", MAX);
    scanf("%d", &LINHAS);
    COLUNAS = LINHAS;

    printf("Digite quantas \033[1;35mminas\033[0m deseja esconder: ");
    scanf("%d", &MINAS);

    inicializarTabuleiro();
    limparTela();

    int linha, coluna, resultado;
    char comando;

    while (1) {

        printf("\n");
        printf("Modo: %s\n", modoMarcar ? "\033[1;34mMarcar\033[0m" : "\033[1;35mRevelar\033[0m");
        imprimirTabuleiro(0);
        printf("\n");
        printf("Para marcar uma possível mina, digite 'B'.\nDigite as coordenadas (linha e coluna): ");
        scanf(" %c", &comando); // O espaço antes de %c ignora qualquer caractere de nova linha
        limparTela();

        if (comando == 'B' || comando == 'b') {
            modoMarcar = 1;
            limparTela();
            printf("Modo de marcação ativado. Digite as coordenadas para marcar uma possível mina com uma bandeira.\n");
        } else {

            ungetc(comando, stdin);  // Volta o caractere para o fluxo de entrada (?)
            if (scanf("%d %d", &linha, &coluna) != 2) {
                limparTela();
                printf("Coordenadas inválidas!\n");
                getchar();  // Espera o usuário pressionar Enter para continuar
                continue;   // Retorna ao início do loop
            }

            linha--; coluna--;  // Ajusta para o índice correto

            if (modoMarcar) {
                if (visualizacao[linha][coluna] == REVELADA) {
                    printf("Essa posição já foi revelada, não pode ser marcada!\n");
                } else {
                    visualizacao[linha][coluna] = MARCADA;
                    modoMarcar = 0;
                }
            } else {
                resultado = revelar(linha, coluna);

                if (resultado == -1) {
                    limparTela();
                    imprimirTabuleiro(1);  // Exibe o tabuleiro final com todas as minas
                    printf("\033[1;31mVocê revelou uma mina! Game Over!\033[0m\n\n");
                    break;
                } else if (checarVitoria()) {
                    limparTela();
                    imprimirTabuleiro(1);
                    printf("\033[1;33mPARABÉNS! Você revelou todas as células sem minas!\033[0m\n");
                    break;
                }
            }
        }

    }

    return 0;
}
