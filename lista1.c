#include <stdio.h>
#include <stdlib.h>

// Estrutura do nó da lista duplamente encadeada
typedef struct No {
    int valor;
    struct No* anterior;
    struct No* proximo;
} No;

// Estrutura da lista
typedef struct {
    No* inicio;
    No* fim;
    int tamanho;
} ListaDupla;

// Protótipos das funções
ListaDupla* criar_lista();
void destruir_lista(ListaDupla* lista);
void inserir_inicio(ListaDupla* lista, int valor);
void inserir_posicao(ListaDupla* lista, int valor, int posicao);
void inserir_final(ListaDupla* lista, int valor);
int remover_posicao(ListaDupla* lista, int posicao);
int buscar_valor(ListaDupla* lista, int valor);
void listar_elementos(ListaDupla* lista);
void menu();

// Função para criar uma nova lista
ListaDupla* criar_lista() {
    ListaDupla* lista = (ListaDupla*)malloc(sizeof(ListaDupla));
    if (lista == NULL) {
        printf("Erro ao alocar memória para a lista.\n");
        exit(1);
    }
    lista->inicio = NULL;
    lista->fim = NULL;
    lista->tamanho = 0;
    return lista;
}

// Função para destruir a lista e liberar memória
void destruir_lista(ListaDupla* lista) {
    No* atual = lista->inicio;
    while (atual != NULL) {
        No* temp = atual;
        atual = atual->proximo;
        free(temp);
    }
    free(lista);
}

// Função para inserir no início da lista
void inserir_inicio(ListaDupla* lista, int valor) {
    No* novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return;
    }
    
    novo_no->valor = valor;
    novo_no->anterior = NULL;
    novo_no->proximo = lista->inicio;
    
    if (lista->inicio == NULL) {
        // Lista vazia
        lista->inicio = novo_no;
        lista->fim = novo_no;
    } else {
        lista->inicio->anterior = novo_no;
        lista->inicio = novo_no;
    }
    
    lista->tamanho++;
    printf("Valor %d inserido no início da lista.\n", valor);
}

// Função para inserir em uma posição específica
void inserir_posicao(ListaDupla* lista, int valor, int posicao) {
    if (posicao < 1 || posicao > lista->tamanho + 1) {
        printf("Posição inválida! A lista tem %d elementos.\n", lista->tamanho);
        return;
    }
    
    if (posicao == 1) {
        inserir_inicio(lista, valor);
        return;
    }
    
    if (posicao == lista->tamanho + 1) {
        inserir_final(lista, valor);
        return;
    }
    
    No* novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return;
    }
    
    novo_no->valor = valor;
    
    // Encontrar a posição onde inserir
    No* atual = lista->inicio;
    for (int i = 1; i < posicao; i++) {
        atual = atual->proximo;
    }
    
    // Inserir antes do nó atual
    novo_no->anterior = atual->anterior;
    novo_no->proximo = atual;
    atual->anterior->proximo = novo_no;
    atual->anterior = novo_no;
    
    lista->tamanho++;
    printf("Valor %d inserido na posição %d.\n", valor, posicao);
}

// Função para inserir no final da lista
void inserir_final(ListaDupla* lista, int valor) {
    No* novo_no = (No*)malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("Erro ao alocar memória para o nó.\n");
        return;
    }
    
    novo_no->valor = valor;
    novo_no->proximo = NULL;
    novo_no->anterior = lista->fim;
    
    if (lista->fim == NULL) {
        // Lista vazia
        lista->inicio = novo_no;
        lista->fim = novo_no;
    } else {
        lista->fim->proximo = novo_no;
        lista->fim = novo_no;
    }
    
    lista->tamanho++;
    printf("Valor %d inserido no final da lista.\n", valor);
}

// Função para remover de uma posição específica
int remover_posicao(ListaDupla* lista, int posicao) {
    if (posicao < 1 || posicao > lista->tamanho) {
        printf("Posição inválida! A lista tem %d elementos.\n", lista->tamanho);
        return -1;
    }
    
    No* atual;
    int valor_removido;
    
    if (posicao == 1) {
        // Remover do início
        atual = lista->inicio;
        lista->inicio = atual->proximo;
        
        if (lista->inicio != NULL) {
            lista->inicio->anterior = NULL;
        } else {
            // Lista ficou vazia
            lista->fim = NULL;
        }
    } else if (posicao == lista->tamanho) {
        // Remover do final
        atual = lista->fim;
        lista->fim = atual->anterior;
        lista->fim->proximo = NULL;
    } else {
        // Remover do meio
        atual = lista->inicio;
        for (int i = 1; i < posicao; i++) {
            atual = atual->proximo;
        }
        
        atual->anterior->proximo = atual->proximo;
        atual->proximo->anterior = atual->anterior;
    }
    
    valor_removido = atual->valor;
    free(atual);
    lista->tamanho--;
    
    printf("Valor %d removido da posição %d.\n", valor_removido, posicao);
    return valor_removido;
}

// Função para buscar um valor na lista
int buscar_valor(ListaDupla* lista, int valor) {
    No* atual = lista->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
        if (atual->valor == valor) {
            printf("Valor %d encontrado na posição %d.\n", valor, posicao);
            return posicao;
        }
        atual = atual->proximo;
        posicao++;
    }
    
    printf("Valor %d não encontrado na lista.\n", valor);
    return -1;
}

// Função para listar todos os elementos
void listar_elementos(ListaDupla* lista) {
    if (lista->inicio == NULL) {
        printf("Lista vazia!\n");
        return;
    }
    
    printf("\n=== ELEMENTOS DA LISTA ===\n");
    printf("Tamanho da lista: %d\n", lista->tamanho);
    printf("Pos | Valor | Anterior | Próximo\n");
    printf("----|-------|----------|--------\n");
    
    No* atual = lista->inicio;
    int posicao = 1;
    
    while (atual != NULL) {
        printf("%3d | %5d | ", posicao, atual->valor);
        
        if (atual->anterior == NULL) {
            printf("   NULL   | ");
        } else {
            printf("  %5d  | ", atual->anterior->valor);
        }
        
        if (atual->proximo == NULL) {
            printf("  NULL\n");
        } else {
            printf(" %5d\n", atual->proximo->valor);
        }
        
        atual = atual->proximo;
        posicao++;
    }
    printf("==========================\n\n");
}

// Menu interativo
void menu() {
    ListaDupla* lista = criar_lista();
    int opcao, valor, posicao;
    
    do {
        printf("\n=== LISTA DUPLAMENTE ENCADEADA ===\n");
        printf("1. Inserir no início\n");
        printf("2. Inserir em posição específica\n");
        printf("3. Inserir no final\n");
        printf("4. Remover de posição específica\n");
        printf("5. Buscar valor\n");
        printf("6. Listar elementos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserir_inicio(lista, valor);
                break;
                
            case 2:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                printf("Digite a posição: ");
                scanf("%d", &posicao);
                inserir_posicao(lista, valor, posicao);
                break;
                
            case 3:
                printf("Digite o valor a ser inserido: ");
                scanf("%d", &valor);
                inserir_final(lista, valor);
                break;
                
            case 4:
                printf("Digite a posição a ser removida: ");
                scanf("%d", &posicao);
                remover_posicao(lista, posicao);
                break;
                
            case 5:
                printf("Digite o valor a ser buscado: ");
                scanf("%d", &valor);
                buscar_valor(lista, valor);
                break;
                
            case 6:
                listar_elementos(lista);
                break;
                
            case 0:
                printf("Encerrando programa...\n");
                break;
                
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 0);
    
    destruir_lista(lista);
}

// Função principal
int main() {
    printf("=== IMPLEMENTAÇÃO DE LISTA DUPLAMENTE ENCADEADA ===\n");
    menu();
    return 0;
}