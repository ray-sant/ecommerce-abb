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

typedef struct NoCarrinho {
    Produto p;
    int quantidade;
    struct NoCarrinho *esq;
    struct NoCarrinho *dir;
} NoCarrinho;

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
    return raiz;
}

NoCarrinho* criarNoCarrinho(Produto p, int quantidade) {
    NoCarrinho* novo = (NoCarrinho*)malloc(sizeof(NoCarrinho));
    if (!novo) {
        printf("Erro de memória!\n");
        exit(1);
    }
    novo->p = p;
    novo->quantidade = quantidade;
    novo->esq = novo->dir = NULL;
    return novo;
}

NoCarrinho* inserirNoCarrinho(NoCarrinho* raiz, Produto p, int quantidade) {
    if (raiz == NULL) return criarNoCarrinho(p, quantidade);
    
    if (p.codigo < raiz->p.codigo)
        raiz->esq = inserirNoCarrinho(raiz->esq, p, quantidade);
    else if (p.codigo > raiz->p.codigo)
        raiz->dir = inserirNoCarrinho(raiz->dir, p, quantidade);
    else {
        raiz->quantidade += quantidade;
    }
    return raiz;
}

No* buscarProdutoPorCodigo(No* raiz, int codigo) {
    if (raiz == NULL || raiz->p.codigo == codigo) {
        return raiz;
    }
    if (codigo < raiz->p.codigo) {
        return buscarProdutoPorCodigo(raiz->esq, codigo);
    }
    return buscarProdutoPorCodigo(raiz->dir, codigo);
}

void imprimirCarrinho(NoCarrinho* raiz) {
    if (raiz != NULL) {
        imprimirCarrinho(raiz->esq);
        printf("  - Cód: %d | Nome: %s | Qtd: %d | Preço Unit.: R$%.2f | Subtotal: R$%.2f\n",
               raiz->p.codigo, raiz->p.nome, raiz->quantidade, raiz->p.preco, raiz->p.preco * raiz->quantidade);
        imprimirCarrinho(raiz->dir);
    }
}

float calcularTotalCarrinho(NoCarrinho* raiz) {
    if (raiz == NULL) {
        return 0.0;
    }
    float total = (raiz->p.preco * raiz->quantidade);
    total += calcularTotalCarrinho(raiz->esq);
    total += calcularTotalCarrinho(raiz->dir);
    return total;
}

NoCarrinho* encontrarMinimo(NoCarrinho* no) {
    NoCarrinho* atual = no;
    while (atual && atual->esq != NULL) {
        atual = atual->esq;
    }
    return atual;
}

NoCarrinho* removerDoCarrinho(NoCarrinho* raiz, int codigo, No* arvoreCodigo) {
    if (raiz == NULL) return raiz;

    if (codigo < raiz->p.codigo) {
        raiz->esq = removerDoCarrinho(raiz->esq, codigo, arvoreCodigo);
    } else if (codigo > raiz->p.codigo) {
        raiz->dir = removerDoCarrinho(raiz->dir, codigo, arvoreCodigo);
    } else {
        No* produtoCatalogo = buscarProdutoPorCodigo(arvoreCodigo, codigo);
        if (produtoCatalogo) {
            produtoCatalogo->p.estoque += raiz->quantidade;
            printf("Devolução de %d unidade(s) do produto '%s' realizada. Estoque atualizado para: %d\n", 
                   raiz->quantidade, raiz->p.nome, produtoCatalogo->p.estoque);
        }

        if (raiz->esq == NULL) {
            NoCarrinho* temp = raiz->dir;
            free(raiz);
            return temp;
        } else if (raiz->dir == NULL) {
            NoCarrinho* temp = raiz->esq;
            free(raiz);
            return temp;
        }

        NoCarrinho* temp = encontrarMinimo(raiz->dir);
        raiz->p = temp->p;
        raiz->quantidade = temp->quantidade;
        raiz->dir = removerDoCarrinho(raiz->dir, temp->p.codigo, NULL);
    }
    return raiz;
}

void carregarCSV(const char* nomeArquivo, No** arvoreCodigo, No** arvorePreco, No** arvoreNome) {
    FILE* f = fopen(nomeArquivo, "r");
    if (!f) {
        printf("Erro ao abrir arquivo %s\n", nomeArquivo);
        exit(1);
    }

    char linha[256];
    fgets(linha, sizeof(linha), f); 

    while (fgets(linha, sizeof(linha), f)) {
        Produto p;
        if (sscanf(linha, "%d,%99[^,],%f,%d", &p.codigo, p.nome, &p.preco, &p.estoque) == 4) {
            *arvoreCodigo = inserirPorCodigo(*arvoreCodigo, p);
            *arvorePreco  = inserirPorPreco(*arvorePreco, p);
            *arvoreNome   = inserirPorNome(*arvoreNome, p);
        }
    }
    fclose(f);
}

void liberarArvore(No* raiz) {
    if (raiz == NULL) return;
    liberarArvore(raiz->esq);
    liberarArvore(raiz->dir);
    free(raiz);
}

void liberarCarrinho(NoCarrinho* raiz) {
    if (raiz == NULL) return;
    liberarCarrinho(raiz->esq);
    liberarCarrinho(raiz->dir);
    free(raiz);
}

int main() {
    No* arvoreCodigo = NULL;
    No* arvorePreco  = NULL;
    No* arvoreNome   = NULL;
    NoCarrinho* carrinho = NULL;

    carregarCSV("produtos.csv", &arvoreCodigo, &arvorePreco, &arvoreNome);
    printf("Catálogo de produtos carregado com sucesso!\n");

    int opcao, codigo, quantidade;

    do {
        printf("\n======= E-COMMERCE MENU =======\n");
        printf("1. Adicionar produto ao carrinho\n");
        printf("2. Visualizar carrinho de compras\n");
        printf("3. Devolver produto (remover do carrinho)\n");
        printf("4. Buscar produto por código\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o código do produto: ");
                scanf("%d", &codigo);
                printf("Digite a quantidade: ");
                scanf("%d", &quantidade);

                No* produtoEncontrado = buscarProdutoPorCodigo(arvoreCodigo, codigo);
                if (produtoEncontrado == NULL) {
                    printf("ERRO: Produto com código %d não encontrado.\n", codigo);
                } else if (produtoEncontrado->p.estoque < quantidade) {
                    printf("ERRO: Estoque insuficiente. Disponível: %d\n", produtoEncontrado->p.estoque);
                } else {
                    produtoEncontrado->p.estoque -= quantidade;
                    carrinho = inserirNoCarrinho(carrinho, produtoEncontrado->p, quantidade);
                    printf("Produto '%s' adicionado ao carrinho com sucesso!\n", produtoEncontrado->p.nome);
                }
                break;

            case 2:
                if (carrinho == NULL) {
                    printf("\nO carrinho de compras está vazio.\n");
                } else {
                    printf("\n--- SEU CARRINHO DE COMPRAS ---\n");
                    imprimirCarrinho(carrinho);
                    printf("----------------------------------\n");
                    printf("TOTAL DO PEDIDO: R$%.2f\n", calcularTotalCarrinho(carrinho));
                }
                break;
            
            case 3:
                 if (carrinho == NULL) {
                    printf("\nO carrinho de compras já está vazio.\n");
                } else {
                    printf("Digite o código do produto para devolver: ");
                    scanf("%d", &codigo);
                    carrinho = removerDoCarrinho(carrinho, codigo, arvoreCodigo);
                }
                break;
            
            case 4:
                printf("Digite o código do produto a buscar: ");
                scanf("%d", &codigo);
                No* produtoBusca = buscarProdutoPorCodigo(arvoreCodigo, codigo);
                 if (produtoBusca != NULL) {
                    printf("\n--- Detalhes do Produto ---\n");
                    printf("Código: %d\nNome: %s\nPreço: R$%.2f\nEstoque: %d\n",
                           produtoBusca->p.codigo, produtoBusca->p.nome, produtoBusca->p.preco, produtoBusca->p.estoque);
                } else {
                    printf("Produto com código %d não encontrado no catálogo.\n", codigo);
                }
                break;

            case 0:
                printf("Saindo do sistema...\n");
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);

    liberarArvore(arvoreCodigo);
    liberarArvore(arvorePreco);
    liberarArvore(arvoreNome);
    liberarCarrinho(carrinho);

    return 0;
}
