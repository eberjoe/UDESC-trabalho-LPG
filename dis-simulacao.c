#include <stdio.h>

#define MAXNOME 100

struct Banco {
    int idBanco; // identificador da instituição financeira
    char nome[MAXNOME]; // nome da instituição financeira
};

struct Produto {
    int idProduto; // identificador do produto
    int idBanco; // identificador da instituição financeira à qual o produto pertence (chave externa)
    char nomeProduto[MAXNOME]; // nome do produto
    int disponivel; // flag indicando se o produto está disponível ou não
    char sistAmortizacao; // caracter indicando o sistema de amortização que pode ser SAC ('S') ou PRICE ('P')
    float maxPorcentFinanc; // número entre 0 e 1 indicando a máxima porção financiável de um valor
    float taxaEfetivaJuros; // número entre 0 e 1 indicando a taxa efetiva de juros
    int prazoMax; // número inteiro indicando a máxima quantidade de meses permitida para o financiamento
    float maxPorcentRenda; // número entre 0 e 1 indicando o máximo comprometimento da renda permitido
};

int main() {
    int op, s=0, ch;
    while(1) {
        printf("MENU PRINCIPAL\n");
        printf("[1] SIMULAÇÃO\n[2] CADASTRO DE INSTITUIÇÃO FINANCEIRA\n[3] CADASTRO DE PRODUTO FINANCEIRO\n[4] SAIR\n");
        printf("Entre uma das opções acima: ");
        scanf("%d", &op);
        switch(op) {
            case 1:
                printf("\nSIMULAÇÃO\n");
                break;
            case 2:
                printf("\nCADASTRO DE BANCO\n");
                break;
            case 3:
                printf("\nCADASTRO DE PRODUTO\n");
                break;
            case 4:
                printf("\nOBRIGADO!\n");
                s=1;
                break;
            default:
                printf("\nOPÇÃO INVÁLIDA!\n\n");
                /* Limpa o buffer do \n que vem com scanf() */
                while ((ch=getchar()) != '\n' && ch != EOF) ;
                printf("\n\nPressione ENTER para continuar.");
                while ((ch=getchar()) != '\n' && ch != EOF)
                break;
        }
        if (s) break; else printf("\nOPÇÃO EM DESENVOLVIMENTO...\n\n");
    }
    return 0;
}
