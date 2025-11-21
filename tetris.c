#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// -------- STRUCT PEÇA --------
typedef struct {
    char tipo;
    int id;
} Peca;

// -------- FILA CIRCULAR --------
typedef struct {
    Peca itens[5];
    int inicio, fim, tamanho;
} Fila;

void inicializarFila(Fila *f) {
    f->inicio = 0;
    f->fim = 0;
    f->tamanho = 0;
}

int filaVazia(Fila *f) { return f->tamanho == 0; }
int filaCheia(Fila *f) { return f->tamanho == 5; }

int enqueue(Fila *f, Peca p) {
    if (filaCheia(f)) return 0;
    f->itens[f->fim] = p;
    f->fim = (f->fim + 1) % 5;
    f->tamanho++;
    return 1;
}

int dequeue(Fila *f, Peca *p) {
    if (filaVazia(f)) return 0;
    *p = f->itens[f->inicio];
    f->inicio = (f->inicio + 1) % 5;
    f->tamanho--;
    return 1;
}

void mostrarFila(Fila *f) {
    printf("Fila: ");
    if (filaVazia(f)) { printf("[vazia]\n"); return; }

    int idx = f->inicio;
    for (int i = 0; i < f->tamanho; i++) {
        printf("[ID:%d Tipo:%c] ", f->itens[idx].id, f->itens[idx].tipo);
        idx = (idx + 1) % 5;
    }
    printf("\n");
}

// -------- PILHA --------
typedef struct {
    Peca itens[5];
    int topo;
} Pilha;

void inicializarPilha(Pilha *p) { p->topo = -1; }
int pilhaVazia(Pilha *p) { return p->topo == -1; }
int pilhaCheia(Pilha *p) { return p->topo == 4; }

int push(Pilha *p, Peca elem) {
    if (pilhaCheia(p)) return 0;
    p->itens[++p->topo] = elem;
    return 1;
}

int pop(Pilha *p, Peca *elem) {
    if (pilhaVazia(p)) return 0;
    *elem = p->itens[p->topo--];
    return 1;
}

void mostrarPilha(Pilha *p) {
    printf("Pilha reserva: ");
    if (pilhaVazia(p)) { printf("[vazia]\n"); return; }

    for (int i = 0; i <= p->topo; i++)
        printf("[ID:%d Tipo:%c] ", p->itens[i].id, p->itens[i].tipo);

    printf("\n");
}

// -------- GERAR PEÇA --------
Peca gerarPeca(int id) {
    Peca p;
    char tipos[] = {'A','B','C','D'};
    p.tipo = tipos[rand() % 4];
    p.id = id;
    return p;
}

// -------- HISTÓRICO --------
typedef struct {
    int acao;   // 1=jogar, 2=reservar, 3=usar, 4=trocar
    Peca p1;    // peça afetada
    Peca p2;    // peça 2 (caso troca)
} Historico;

// --------- MAIN ---------
int main() {
    srand(time(NULL));

    Fila fila;
    Pilha pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);

    Historico h = {0};
    int idAtual = 1;

    // Preenche a fila inicial
    for (int i = 0; i < 5; i++)
        enqueue(&fila, gerarPeca(idAtual++));

    int op;
    do {
        printf("\n===== MENU =====\n");
        printf("1 - Jogar peça\n");
        printf("2 - Reservar peça\n");
        printf("3 - Usar peça reservada\n");
        printf("4 - Trocar peça do topo da pilha com a da frente\n");
        printf("5 - Desfazer última jogada\n");
        printf("6 - Inverter fila com pilha\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &op);

        Peca temp1, temp2;

        switch(op) {

        case 1: // Jogar peça
            if (dequeue(&fila, &temp1)) {
                printf("Jogou peça ID:%d Tipo:%c\n", temp1.id, temp1.tipo);
                enqueue(&fila, gerarPeca(idAtual++));
                
                h.acao = 1;
                h.p1 = temp1;
            }
            break;

        case 2: // Reservar peça
            if (!pilhaCheia(&pilha) && dequeue(&fila, &temp1)) {
                push(&pilha, temp1);
                enqueue(&fila, gerarPeca(idAtual++));

                printf("Reservou peça ID:%d Tipo:%c\n", temp1.id, temp1.tipo);

                h.acao = 2;
                h.p1 = temp1;
            }
            break;

        case 3: // Usar peça reservada
            if (pop(&pilha, &temp1)) {
                printf("Usou peça da reserva ID:%d Tipo:%c\n", temp1.id, temp1.tipo);

                h.acao = 3;
                h.p1 = temp1;
            }
            break;

        case 4: // Trocar topo da pilha com frente da fila
            if (!pilhaVazia(&pilha) && !filaVazia(&fila)) {
                temp1 = pilha.itens[pilha.topo];     // topo
                temp2 = fila.itens[fila.inicio];     // frente

                pilha.itens[pilha.topo] = temp2;
                fila.itens[fila.inicio] = temp1;

                printf("Trocou topo da pilha com frente da fila!\n");

                h.acao = 4;
                h.p1 = temp1;
                h.p2 = temp2;
            }
            break;

        case 5: // Desfazer
            printf("Desfazer ainda não implementado.\n");
            break;

        case 6: // Inverter fila com pilha
            inicializarPilha(&pilha);

            while (!filaVazia(&fila)) {
                dequeue(&fila, &temp1);
                push(&pilha, temp1);
            }
            while (!pilhaVazia(&pilha)) {
                pop(&pilha, &temp1);
                enqueue(&fila, temp1);
            }

            printf("Fila e pilha invertidas!\n");
            break;

        case 0:
            printf("Saindo...\n");
            break;

        default:
            printf("Opção inválida!\n");
        }

        mostrarFila(&fila);
        mostrarPilha(&pilha);

    } while (op != 0);

    return 0;
}
