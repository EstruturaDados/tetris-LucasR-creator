#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// ---------- STRUCTS ----------
typedef struct {
    char tipo;
    int id;
} Peca;

typedef struct {
    Peca itens[5];
    int inicio;
    int fim;
    int tamanho;
} Fila;

// ---------- FUNÇÕES ----------
void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int filaCheia(Fila *f) {
    return f->tamanho == 5;
}

int filaVazia(Fila *f) {
    return f->tamanho == 0;
}

Peca gerarPeca(int id) {
    Peca nova;
    char tipos[] = {'A', 'B', 'C', 'D'};
    nova.tipo = tipos[rand() % 4];
    nova.id = id;
    return nova;
}

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;

    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % 5;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *removida) {
    if (filaVazia(f)) return 0;

    *removida = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % 5;
    f->tamanho--;
    return 1;
}

void mostrarFila(Fila *f) {
    if (filaVazia(f)) {
        printf("Fila vazia.\n");
        return;
    }

    printf("Fila: ");
    int idx = f->inicio;
    for (int i = 0; i < f->tamanho; i++) {
        printf("[ID:%d | Tipo:%c] ", f->itens[idx].id, f->itens[idx].tipo);
        idx = (idx + 1) % 5;
    }
    printf("\n");
}

// ---------- MAIN ----------
int main() {
    Fila fila;
    inicializarFila(&fila);

    srand(time(NULL));

    int idAtual = 1;

    // Preencher a fila no início
    for (int i = 0; i < 5; i++) {
        enqueue(&fila, gerarPeca(idAtual++));
    }

    int opcao;

    do {
        printf("\n===== MENU =====\n");
        printf("1 - Jogar peça (remover da frente)\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        switch (opcao) {
        case 1: {
            Peca removida;
            if (dequeue(&fila, &removida)) {
                printf("Peça jogada -> ID:%d Tipo:%c\n", removida.id, removida.tipo);

                enqueue(&fila, gerarPeca(idAtual++));
            }
            mostrarFila(&fila);
            break;
        }

        case 0:
            printf("Encerrando...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

    } while (opcao != 0);

    return 0;
}
