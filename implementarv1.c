#include <stdio.h>    // printf, scanf -> Funções de entrada e saída
#include <stdlib.h>   // malloc, free, rand -> Alocação dinâmica de memória e funções utilitárias
#include <string.h>   // strcmp, strcpy -> Manipulação de strings
#include <time.h>     // clock, srand, time -> Funções relacionadas a tempo

// Definição da estrutura Produto
typedef struct {
    int codigo;        // Código identificador do produto
    char nome[50];     // Nome do produto (até 49 caracteres + '\0')
    float preco;       // Preço do produto
    int estoque;       // Quantidade em estoque
} Produto;

// Definição da estrutura Nó da árvore binária
typedef struct No {
    Produto p;         // Produto armazenado neste nó
    struct No *esq;    // Ponteiro para o filho da esquerda
    struct No *dir;    // Ponteiro para o filho da direita
} No;

// Função para criar um novo nó da árvore binária
No* criarNo(Produto p) {
    No* novo = (No*)malloc(sizeof(No)); // Aloca memória para o novo nó
    if (!novo) {                        // Verifica se a alocação falhou
        printf("Erro de memória!\n");
        exit(1);                        // Encerra o programa em caso de erro
    }
    novo->p = p;                        // Copia os dados do produto para o nó
    novo->esq = novo->dir = NULL;       // Inicializa os filhos como NULL (nó folha)
    return novo;                        // Retorna o ponteiro para o novo nó
}

int main(){
  
}
