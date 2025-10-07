#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5 // Tamanho fixo da fila

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;  // Índice da frente da fila
    int tras;    // Índice do final da fila
    int tamanho; // Quantidade atual de peças na fila
} FilaPecas;

// ---------- Funções utilitárias ----------

// Inicializa a fila vazia
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

// Verifica se a fila está cheia
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAM_FILA;
}

// Verifica se a fila está vazia
int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

// Gera uma peça automaticamente com nome aleatório e id incremental
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4]; // Escolhe um tipo aleatório
    nova.id = id;
    return nova;
}

// Enfileira (adiciona) uma nova peça ao final da fila
void enqueue(FilaPecas *fila, Peca nova) {
    if (filaCheia(fila)) {
        printf("\n⚠️ A fila está cheia! Não é possível adicionar nova peça.\n");
        return;
    }
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = nova;
    fila->tamanho++;
}

// Desenfileira (remove) a peça da frente da fila
void dequeue(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("\n⚠️ A fila está vazia! Nenhuma peça para jogar.\n");
        return;
    }
    Peca removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    printf("\n🎮 Peça jogada: [%c %d]\n", removida.nome, removida.id);
}

// Exibe o estado atual da fila
void exibirFila(FilaPecas *fila) {
    if (filaVazia(fila)) {
        printf("\nFila de peças: (vazia)\n");
        return;
    }
    printf("\nFila de peças:\n");
    for (int i = 0; i < fila->tamanho; i++) {
        int indice = (fila->frente + i) % TAM_FILA;
        printf("[%c %d] ", fila->elementos[indice].nome, fila->elementos[indice].id);
    }
    printf("\n");
}

// ---------- Função principal ----------
int main() {
    FilaPecas fila;
    inicializarFila(&fila);
    srand(time(NULL));

    int idAtual = 0;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    int opcao;

    do {
        exibirFila(&fila);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça (dequeue)\n");
        printf("2 - Inserir nova peça (enqueue)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                dequeue(&fila);
                break;
            case 2:
                enqueue(&fila, gerarPeca(idAtual++));
                break;
            case 0:
                printf("\nEncerrando o jogo...\n");
                break;
            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
