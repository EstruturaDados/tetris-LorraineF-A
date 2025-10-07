#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAM_FILA 5   // Capacidade fixa da fila circular
#define TAM_PILHA 3  // Capacidade fixa da pilha

// -------------------- ESTRUTURAS --------------------

typedef struct {
    char nome; // Tipo da peça: 'I', 'O', 'T', 'L'
    int id;    // Identificador único da peça
} Peca;

typedef struct {
    Peca elementos[TAM_FILA];
    int frente;
    int tras;
    int tamanho;
} FilaPecas;

typedef struct {
    Peca elementos[TAM_PILHA];
    int topo;
} PilhaPecas;

// -------------------- FUNÇÕES DE INICIALIZAÇÃO --------------------

void inicializarFila(FilaPecas *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->tamanho = 0;
}

void inicializarPilha(PilhaPecas *pilha) {
    pilha->topo = -1;
}

// -------------------- FUNÇÕES DE ESTADO --------------------

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

// -------------------- GERADOR DE PEÇAS --------------------

Peca gerarPeca(int id) {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca nova;
    nova.nome = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

// -------------------- OPERAÇÕES DE FILA --------------------

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

// -------------------- OPERAÇÕES DE PILHA --------------------

void push(PilhaPecas *pilha, Peca p) {
    if (pilhaCheia(pilha)) return;
    pilha->elementos[++pilha->topo] = p;
}

Peca pop(PilhaPecas *pilha) {
    Peca removida = {'-', -1};
    if (pilhaVazia(pilha)) return removida;
    removida = pilha->elementos[pilha->topo--];
    return removida;
}

// -------------------- EXIBIÇÃO --------------------

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

void exibirEstado(FilaPecas *fila, PilhaPecas *pilha) {
    printf("\n===== ESTADO ATUAL =====\n");
    exibirFila(fila);
    exibirPilha(pilha);
    printf("=========================\n");
}

// -------------------- OPERAÇÕES ESPECIAIS --------------------

// Troca a peça da frente da fila com o topo da pilha
void trocarFrenteComTopo(FilaPecas *fila, PilhaPecas *pilha) {
    if (filaVazia(fila) || pilhaVazia(pilha)) {
        printf("\n⚠️ Não é possível trocar: fila ou pilha vazia.\n");
        return;
    }
    int frente = fila->frente;
    Peca temp = fila->elementos[frente];
    fila->elementos[frente] = pilha->elementos[pilha->topo];
    pilha->elementos[pilha->topo] = temp;

    printf("\n🔄 Troca simples realizada entre a frente da fila e o topo da pilha!\n");
}

// Troca as 3 primeiras peças da fila com as 3 da pilha
void trocaMultipla(FilaPecas *fila, PilhaPecas *pilha) {
    if (fila->tamanho < 3 || pilha->topo < 2) {
        printf("\n⚠️ Não há peças suficientes para a troca múltipla.\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        int indiceFila = (fila->frente + i) % TAM_FILA;
        Peca temp = fila->elementos[indiceFila];
        fila->elementos[indiceFila] = pilha->elementos[pilha->topo - i];
        pilha->elementos[pilha->topo - i] = temp;
    }

    printf("\n🔁 Troca múltipla realizada entre as 3 primeiras da fila e as 3 da pilha!\n");
}

// -------------------- FUNÇÃO PRINCIPAL --------------------

int main() {
    srand(time(NULL));

    FilaPecas fila;
    PilhaPecas pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    int idAtual = 0;

    // Preenche a fila inicial com 5 peças
    for (int i = 0; i < TAM_FILA; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    int opcao;

    do {
        exibirEstado(&fila, &pilha);

        printf("\nOpções disponíveis:\n");
        printf("1 - Jogar peça da frente da fila\n");
        printf("2 - Enviar peça da fila para a pilha de reserva\n");
        printf("3 - Usar peça da pilha de reserva\n");
        printf("4 - Trocar peça da frente da fila com o topo da pilha\n");
        printf("5 - Trocar os 3 primeiros da fila com as 3 peças da pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                if (!filaVazia(&fila)) {
                    Peca jogada = dequeue(&fila);
                    printf("\n🎮 Peça jogada: [%c %d]\n", jogada.nome, jogada.id);
                    enqueue(&fila, gerarPeca(idAtual++)); // mantém fila cheia
                } else {
                    printf("\n⚠️ Fila vazia!\n");
                }
                break;
            }

            case 2: {
                if (!filaVazia(&fila)) {
                    if (!pilhaCheia(&pilha)) {
                        Peca reservada = dequeue(&fila);
                        push(&pilha, reservada);
                        printf("\n📦 Peça reservada: [%c %d]\n", reservada.nome, reservada.id);
                        enqueue(&fila, gerarPeca(idAtual++));
                    } else {
                        printf("\n⚠️ Pilha cheia! Não é possível reservar mais.\n");
                    }
                }
                break;
            }

            case 3: {
                if (!pilhaVazia(&pilha)) {
                    Peca usada = pop(&pilha);
                    printf("\n🧩 Peça usada da pilha: [%c %d]\n", usada.nome, usada.id);
                } else {
                    printf("\n⚠️ Nenhuma peça reservada disponível.\n");
                }
                break;
            }

            case 4:
                trocarFrenteComTopo(&fila, &pilha);
                break;

            case 5:
                trocaMultipla(&fila, &pilha);
                break;

            case 0:
                printf("\nEncerrando o gerenciador de peças...\n");
                break;

            default:
                printf("\n❌ Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}