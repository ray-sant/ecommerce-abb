#include <stdio.h>    // printf, scanf
#include <stdlib.h>   // malloc, free, rand
#include <string.h>   // strcmp, strcpy
#include <time.h>     // clock (para medir tempo), srand, time

typedef struct {
    int codigo;
    char nome[50];
    float preco;
    int estoque;
} Produto;

typedef struct No {
    Produto p;         // produto armazenado no nó
    struct No *esq;    // ponteiro para filho da esquerda
    struct No *dir;    // ponteiro para filho da direita
} No;

int main(){
    
}