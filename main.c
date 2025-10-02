#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int codigo;
    char nome[100];
    float preco;
    int estoque;
} Produto;

typedef struct No {
    Produto p;
    struct No *esq;
    struct No *dir;
} No;

No* criarNo(Produto p) {
    No* novo = (No*)malloc(sizeof(No));
    if (!novo) {
        printf("Erro de memória!\n");
        exit(1);
    }
    novo->p = p;
    novo->esq = novo->dir = NULL;
    return novo;
}

No* inserirPorCodigo(No* raiz, Produto p) {
    if (raiz == NULL) return criarNo(p);
    if (p.codigo < raiz->p.codigo)
        raiz->esq = inserirPorCodigo(raiz->esq, p);
    else if (p.codigo > raiz->p.codigo)
        raiz->dir = inserirPorCodigo(raiz->dir, p);
    else
        printf("Produto com código %d já existe!\n", p.codigo);
    return raiz;
}

No* inserirPorPreco(No* raiz, Produto p) {
    if (raiz == NULL) return criarNo(p);
    if (p.preco < raiz->p.preco)
        raiz->esq = inserirPorPreco(raiz->esq, p);
    else
        raiz->dir = inserirPorPreco(raiz->dir, p);
    return raiz;
}

No* inserirPorNome(No* raiz, Produto p) {
    if (raiz == NULL) return criarNo(p);
    int cmp = strcmp(p.nome, raiz->p.nome);
    if (cmp < 0)
        raiz->esq = inserirPorNome(raiz->esq, p);
    else if (cmp > 0)
        raiz->dir = inserirPorNome(raiz->dir, p);
    else
        printf("Produto com nome \"%s\" já existe!\n", p.nome);
    return raiz;
}

void imprimirEmOrdem(No* raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esq);
        printf("Código: %d | Nome: %s | Preço: %.2f | Estoque: %d\n",
               raiz->p.codigo, raiz->p.nome, raiz->p.preco, raiz->p.estoque);
        imprimirEmOrdem(raiz->dir);
    }
}

void carregarCSV(const char* nomeArquivo, No** arvoreCodigo, No** arvorePreco, No** arvoreNome) {
    FILE* f = fopen(nomeArquivo, "r");
    if (!f) {
        printf("Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }

    char linha[256];
    int linhaCount = 0;

    while (fgets(linha, sizeof(linha), f)) {
        if (linhaCount == 0) {
            linhaCount++;
            continue;
        }

        Produto p;
        if (sscanf(linha, "%d,%99[^,],%f,%d", &p.codigo, p.nome, &p.preco, &p.estoque) == 4) {
            *arvoreCodigo = inserirPorCodigo(*arvoreCodigo, p);
            *arvorePreco  = inserirPorPreco(*arvorePreco, p);
            *arvoreNome   = inserirPorNome(*arvoreNome, p);
        } else {
            printf("Linha inválida ignorada: %s\n", linha);
        }
        linhaCount++;
    }

    fclose(f);
}

int main() {
    No* arvoreCodigo = NULL;
    No* arvorePreco  = NULL;
    No* arvoreNome   = NULL;

    carregarCSV("produtos.csv", &arvoreCodigo, &arvorePreco, &arvoreNome);

    printf("\nProdutos ordenados por CÓDIGO:\n");
    imprimirEmOrdem(arvoreCodigo);

    printf("\nProdutos ordenados por PREÇO:\n");
    imprimirEmOrdem(arvorePreco);

    printf("\nProdutos ordenados por NOME:\n");
    imprimirEmOrdem(arvoreNome);

    return 0;
}
