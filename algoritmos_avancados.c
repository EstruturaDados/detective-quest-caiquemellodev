#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_HASH 26   // hash por inicial A–Z

//-------------------------------------
// ESTRUTURA DE NÓ DA BST (pistas)
//-------------------------------------
typedef struct NoBST {
    char pista[100];
    struct NoBST *esq, *dir;
} NoBST;

//-------------------------------------
// ESTRUTURAS DA TABELA HASH
//-------------------------------------
typedef struct Ligacao {
    char pista[100];
    char suspeito[100];
    struct Ligacao *prox;
} Ligacao;

Ligacao* tabela[TAM_HASH];

//-------------------------------------
// FUNÇÃO HASH – baseada na primeira letra
//-------------------------------------
int hashFunc(char *pista) {
    char c = pista[0];
    if (c >= 'a' && c <= 'z') c -= 32;
    if (c < 'A' || c > 'Z') return 0;
    return c - 'A';
}

//-------------------------------------
// INSERIR NA TABELA HASH (pista → suspeito)
//-------------------------------------
void inserirNaHash(char *pista, char *suspeito) {
    int indice = hashFunc(pista);

    Ligacao *novo = malloc(sizeof(Ligacao));
    strcpy(novo->pista, pista);
    strcpy(novo->suspeito, suspeito);
    novo->prox = tabela[indice];
    tabela[indice] = novo;
}

//-------------------------------------
// INSERÇÃO NA BST
//-------------------------------------
NoBST* inserirBST(NoBST *raiz, char *pista) {
    if (raiz == NULL) {
        NoBST *novo = malloc(sizeof(NoBST));
        strcpy(novo->pista, pista);
        novo->esq = novo->dir = NULL;
        return novo;
    }
    if (strcmp(pista, raiz->pista) < 0)
        raiz->esq = inserirBST(raiz->esq, pista);
    else
        raiz->dir = inserirBST(raiz->dir, pista);

    return raiz;
}

//-------------------------------------
// LISTAR TODAS AS ASSOCIAÇÕES
//-------------------------------------
void mostrarHash() {
    printf("\n====== TABELA HASH (PISTA → SUSPEITO) ======\n");
    for (int i = 0; i < TAM_HASH; i++) {
        Ligacao *aux = tabela[i];
        if (aux != NULL) {
            printf("[%d] -> ", i);
            while (aux != NULL) {
                printf("(Pista: %s -> Suspeito: %s)  ", aux->pista, aux->suspeito);
                aux = aux->prox;
            }
            printf("\n");
        }
    }
}

//-------------------------------------
// CONTAR QUAL SUSPEITO MAIS APARECE
//-------------------------------------
char* suspeitoMaisRelatado() {
    // vamos considerar no máximo 100 suspeitos diferentes
    char nomes[100][100];
    int contagem[100];
    int total = 0;

    for (int i = 0; i < 100; i++) contagem[i] = 0;

    for (int i = 0; i < TAM_HASH; i++) {
        Ligacao *aux = tabela[i];
        while (aux != NULL) {

            // verifica se o suspeito já está registrado
            int achou = -1;
            for (int j = 0; j < total; j++)
                if (strcmp(nomes[j], aux->suspeito) == 0)
                    achou = j;

            // se não achou, adiciona
            if (achou == -1) {
                strcpy(nomes[total], aux->suspeito);
                contagem[total]++;
                total++;
            } else {
                contagem[achou]++;
            }

            aux = aux->prox;
        }
    }

    // Encontrar o maior
    int maior = -1;
    int indice = -1;

    for (int i = 0; i < total; i++) {
        if (contagem[i] > maior) {
            maior = contagem[i];
            indice = i;
        }
    }

    if (indice == -1) return "Nenhum suspeito registrado";

    return nomes[indice];
}

//-------------------------------------
// MAIN DE EXEMPLO
//-------------------------------------
int main() {
    NoBST *raiz = NULL;

    // EXEMPLO DE INSERÇÕES (pode trocar)
    raiz = inserirBST(raiz, "Pegada na lama");
    inserirNaHash("Pegada na lama", "Carlos");

    raiz = inserirBST(raiz, "Luvas rasgadas");
    inserirNaHash("Luvas rasgadas", "Marcos");

    raiz = inserirBST(raiz, "Vidro quebrado");
    inserirNaHash("Vidro quebrado", "Carlos");

    raiz = inserirBST(raiz, "Cabelo ruivo encontrado");
    inserirNaHash("Cabelo ruivo encontrado", "Julia");

    inserirNaHash("Pegada na lama", "Carlos");  // Carlos aparece 2x

    mostrarHash();

    printf("\n🔎 Suspeito mais relatado: %s\n", suspeitoMaisRelatado());

    return 0;
}