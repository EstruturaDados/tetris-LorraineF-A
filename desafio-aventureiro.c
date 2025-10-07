#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // Capacidade da fila de peças futuras
#define TAM_PILHA 3  // Capacidade da pilha de peças reservadas

// Estrutura que representa uma peça do Tetris
typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único
} Peca;

// Estrutura da fila circular
typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaPecas;

// Estrutura da pilha
typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} PilhaPecas;

// ---------- Funções de inicialização ----------
void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

void inicializarPilha(PilhaPecas *pilha) {
    pilha->topo = -1;
}

// ---------- Funções de estado ----------
int filaCheia(FilaPecas *fila) {
    return fila->tamanho == TAM_FILA;
}

int filaVazia(FilaPecas *fila) {
    return fila->tamanho == 0;
}

int pilhaCheia(PilhaPecas *pilha) {
    return pilha->topo == TAM_PILHA - 1;
}

int pilhaVazia(PilhaPecas *pilha) {
    return pilha->topo == -1;
}

// ---------- Função de geração de peça ----------
Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// ---------- Operações da fila ----------
void enqueue(FilaPecas *fila, Peca nova) {
    if (filaCheia(fila)) return;
    fila->tras = (fila->tras + 1) % TAM_FILA;
    fila->elementos[fila->tras] = nova;
    fila->tamanho++;
}

Peca dequeue(FilaPecas *fila) {
    Peca removida = {'-', -1};
    if (filaVazia(fila)) return removida;
    removida = fila->elementos[fila->frente];
    fila->frente = (fila->frente + 1) % TAM_FILA;
    fila->tamanho--;
    return removida;
}

// ---------- Operações da pilha ----------
void push(PilhaPecas *pilha, Peca p) {
    if (pilhaCheia(pilha)) {
        printf("\n⚠️ A pilha de reserva está cheia! Não é possível reservar mais peças.\n");
        return;
    }
    pilha->topo++;
    pilha->elementos[pilha->topo] = p;
}

Peca pop(PilhaPecas *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) {
        printf("\n⚠️ A pilha de reserva está vazia! Nenhuma peça para usar.\n");
        return removida;
    }
    removida = pilha->elementos[pilha->topo];
    pilha->topo--;
    return removida;
}

// ---------- Funções de exibição ----------
void exibirFila(FilaPecas *fila) {
    printf("Fila de peças:\t");
    if (filaVazia(fila)) {
        printf("(vazia)");
    } else {
        for (int i = 0; i < fila->tamanho; i++) {
            int indice = (fila->frente + i) % TAM_FILA;
            printf("[%c %d] ", fila->elementos[indice].nome, fila->elementos[indice].id);
        }
    }
    printf("\n");
}

void exibirPilha(PilhaPecas *pilha) {
    printf("Pilha de reserva (Topo -> Base): ");
    if (pilhaVazia(pilha)) {
        printf("(vazia)");
    } else {
        for (int i = pilha->topo; i >= 0; i--) {
            printf("[%c %d] ", pilha->elementos[i].nome, pilha->elementos[i].id);
        }
    }
    printf("\n");
}

// ---------- Função principal ----------
int main() {
    srand(time(NULL));

    FilaPecas fila;
    PilhaPecas pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idAtual = 0;
    int opcao;

    // Inicializa a fila com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    do {
        printf("\n===== ESTADO ATUAL =====\n");
        exibirFila(&fila);
        exibirPilha(&pilha);

        printf("\nOpções de ação:\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(idAtual++)); // Mantém a fila cheia
                } else {
                    printf("\n⚠️ A fila está vazia! Nenhuma peça para jogar.\n");
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca reservada = dequeue(&fila);
                        push(&pilha, reservada);
                        printf("\n📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                        enqueue(&fila, gerarPeca(idAtual++)); // Nova peça entra na fila
                    } else {
                        printf("\n⚠️ A pilha de reserva está cheia!\n");
                    }
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\n🧩 Peça usada da reserva: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\n⚠️ Nenhuma peça reservada disponível.\n");
                }
                break;
            }

            case 0:
                printf("\nEncerrando o jogo...\n");
                break;

            default:
                printf("\n❌ Opção inválida! Tente novamente.\n");
        }

    } while (opcao != 0);

    return 0;
}
