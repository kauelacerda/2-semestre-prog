#include <stdio.h>
#include <stdlib.h>

/* --- Definição da Estrutura do Nó --- */
typedef struct No {
    int valor;
    struct No *esquerda;
    struct No *direita;
} No;

/* --- Protótipos das Funções --- */
No* criarNo(int valor);
No* inserir(No* raiz, int valor);
No* buscar(No* raiz, int valor);
No* encontrarMinimo(No* raiz);
No* remover(No* raiz, int valor);
void preOrdem(No* raiz);
void emOrdem(No* raiz);
void posOrdem(No* raiz);
void liberarArvore(No* raiz);
void menuPercorrer(No* raiz);

/* --- Função Principal --- */
int main() {
    No* raiz = NULL;
    int opcao, valor;
    No* busca;

    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1 - Inserir valor\n");
        printf("2 - Buscar valor\n");
        printf("3 - Remover valor\n");
        printf("4 - Percorrer arvore\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("Digite o valor para inserir: ");
                scanf("%d", &valor);
                raiz = inserir(raiz, valor);
                printf("Valor %d inserido (ou ja existente).\n", valor);
                break;

            case 2:
                printf("Digite o valor para buscar: ");
                scanf("%d", &valor);
                busca = buscar(raiz, valor);
                if (busca != NULL) {
                    printf("Valor %d ENCONTRADO na arvore.\n", valor);
                } else {
                    printf("Valor %d NAO encontrado.\n", valor);
                }
                break;

            case 3:
                printf("Digite o valor para remover: ");
                scanf("%d", &valor);
                // Verifica se existe antes de tentar remover para dar feedback melhor
                if (buscar(raiz, valor) != NULL) {
                    raiz = remover(raiz, valor);
                    printf("Valor %d removido com sucesso.\n", valor);
                } else {
                    printf("Valor nao esta na arvore.\n");
                }
                break;

            case 4:
                menuPercorrer(raiz);
                break;

            case 0:
                printf("Liberando memoria e saindo...\n");
                liberarArvore(raiz);
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 0);

    return 0;
}

/* --- Implementação das Funções --- */

// Cria um novo nó com alocação dinâmica
No* criarNo(int valor) {
    No* novo = (No*)malloc(sizeof(No));
    if (novo == NULL) {
        printf("Erro de alocacao de memoria!\n");
        exit(1);
    }
    novo->valor = valor;
    novo->esquerda = NULL;
    novo->direita = NULL;
    return novo;
}

// 1. Inserção
No* inserir(No* raiz, int valor) {
    // Caso base: árvore vazia ou chegou numa folha
    if (raiz == NULL) {
        return criarNo(valor);
    }

    // Regra da BST: menores à esquerda, maiores à direita
    if (valor < raiz->valor) {
        raiz->esquerda = inserir(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = inserir(raiz->direita, valor);
    } else {
        // Valor igual: não inserimos duplicatas (conforme documentação)
        // Se quisesse permitir, bastaria decidir um lado (ex: >= vai para direita)
        printf(" [Aviso: Valor duplicado ignorado] ");
    }
    
    return raiz;
}

// 2. Busca
No* buscar(No* raiz, int valor) {
    if (raiz == NULL || raiz->valor == valor) {
        return raiz;
    }

    if (valor < raiz->valor) {
        return buscar(raiz->esquerda, valor);
    } else {
        return buscar(raiz->direita, valor);
    }
}

// Função auxiliar para encontrar o menor valor (usado na remoção)
No* encontrarMinimo(No* raiz) {
    No* atual = raiz;
    // O menor valor em uma BST é o nó mais à esquerda
    while (atual && atual->esquerda != NULL) {
        atual = atual->esquerda;
    }
    return atual;
}

// 3. Remoção (Implementa os 3 casos)
No* remover(No* raiz, int valor) {
    if (raiz == NULL) return raiz;

    // Procura o nó a ser removido
    if (valor < raiz->valor) {
        raiz->esquerda = remover(raiz->esquerda, valor);
    } else if (valor > raiz->valor) {
        raiz->direita = remover(raiz->direita, valor);
    } else {
        // Encontrou o nó a ser removido (raiz->valor == valor)

        // CASO 1: Nó folha (sem filhos)
        if (raiz->esquerda == NULL && raiz->direita == NULL) {
            free(raiz);
            return NULL;
        }

        // CASO 2: Nó com apenas um filho
        else if (raiz->esquerda == NULL) {
            No* temp = raiz->direita;
            free(raiz);
            return temp;
        } else if (raiz->direita == NULL) {
            No* temp = raiz->esquerda;
            free(raiz);
            return temp;
        }

        // CASO 3: Nó com dois filhos
        // Estratégia: Encontrar o sucessor in-order (menor valor da subárvore direita)
        else {
            No* temp = encontrarMinimo(raiz->direita);
            
            // Copia o valor do sucessor para o nó atual
            raiz->valor = temp->valor;
            
            // Remove o sucessor da subárvore direita (agora que o valor foi copiado)
            // Isso recairá no Caso 1 ou 2 para o nó 'temp' original
            raiz->direita = remover(raiz->direita, temp->valor);
        }
    }
    return raiz;
}

// 4. Percursos
void preOrdem(No* raiz) {
    if (raiz != NULL) {
        printf("%d ", raiz->valor); // Raiz
        preOrdem(raiz->esquerda);   // Esquerda
        preOrdem(raiz->direita);    // Direita
    }
}

void emOrdem(No* raiz) {
    if (raiz != NULL) {
        emOrdem(raiz->esquerda);    // Esquerda
        printf("%d ", raiz->valor); // Raiz
        emOrdem(raiz->direita);     // Direita
    }
}

void posOrdem(No* raiz) {
    if (raiz != NULL) {
        posOrdem(raiz->esquerda);   // Esquerda
        posOrdem(raiz->direita);    // Direita
        printf("%d ", raiz->valor); // Raiz
    }
}

// Função para liberar memória (Usa lógica Pós-Ordem para não perder referências)
void liberarArvore(No* raiz) {
    if (raiz != NULL) {
        liberarArvore(raiz->esquerda);
        liberarArvore(raiz->direita);
        free(raiz);
    }
}

// Submenu para percursos
void menuPercorrer(No* raiz) {
    int subOpcao;
    
    if (raiz == NULL) {
        printf("A arvore esta vazia.\n");
        return;
    }

    printf("\n   --- SUBMENU PERCURSO ---\n");
    printf("   1 - Pre-ordem\n");
    printf("   2 - Em ordem\n");
    printf("   3 - Pos-ordem\n");
    printf("   Escolha: ");
    scanf("%d", &subOpcao);

    printf("   Resultado: [ ");
    switch (subOpcao) {
        case 1:
            preOrdem(raiz);
            break;
        case 2:
            emOrdem(raiz);
            break;
        case 3:
            posOrdem(raiz);
            break;
        default:
            printf("Opcao invalida!");
    }
    printf("]\n");
}