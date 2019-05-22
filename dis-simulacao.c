#include <stdio.h>
//#include <stdlib.h>
#include "validacao.h"

#define MAXNOME 100

struct Banco {
    int idBanco; // identificador da instituição financeira
    int disponivel; // flag indicando se a instituição financeira está disponível
    char nome[MAXNOME]; // nome da instituição financeira
};

struct Produto {
    int idProduto; // identificador do produto
    int disponivel; // flag indicando se o produto está disponível ou não
    int idBanco; // identificador da instituição financeira à qual o produto pertence (chave externa)
    char nome[MAXNOME]; // nome do produto
    char sistAmortizacao; // caracter indicando o sistema de amortização que pode ser SAC ('S') ou PRICE ('P')
    float maxPorcentFinanc; // número entre 0 e 1 indicando a máxima porção financiável de um valor
    float taxaEfetivaJuros; // número entre 0 e 1 indicando a taxa efetiva de juros
    int prazoMax; // número inteiro indicando a máxima quantidade de meses permitida para o financiamento
    float maxPorcentRenda; // número entre 0 e 1 indicando o máximo comprometimento da renda permitido
};

int main() {
    FILE *b, *p;
    int op, id, s=0;
    float renda, valorBem, entrada;
    struct Banco entradaBanco;
    struct Produto entradaProduto;
    while(1) {
        printf("MENU PRINCIPAL\n");
        printf("[1] SIMULAÇÃO\n[2] CADASTRO DE BANCO\n[3] CADASTRO DE PRODUTO FINANCEIRO\n[4] SAIR\n");
        printf("Entre uma das opções acima: ");
        scanf("%d", &op);
        while (getchar() != '\n');
        switch(op) {
            case 1:
                printf("\nSIMULAÇÃO\n");
                while(1) {
                    printf("Entre o valor da renda bruta mensal do contraente: ");
                    if (ValidaFloat(&renda)) {
                        printf("%.2f\n", renda);
                        break;
                    }
                    printf("\nEntrada inválida!\n\n");
                }
                while(1) {
                    printf("Entre o valor do bem a ser adquirido: ");
                    if (ValidaFloat(&valorBem)) {
                        printf("%.2f\n", valorBem);
                        break;
                    }
                    printf("\nEntrada inválida!\n\n");
                }
                while(1) {
                    printf("Entre o valor da entrada: ");
                    if (ValidaFloat(&entrada) && entrada<valorBem) {
                        printf("%.2f\n", entrada);
                        break;
                    }
                    printf("\nEntrada inválida!\n\n");
                }
                break;
            case 2:
                printf("\nCADASTRO DE BANCO\n");
                printf("[1] CONSULTA\n[2] INSERÇÃO\n[3] REMOÇÃO\n");
                printf("Entre uma das opções acima: ");
                scanf("%d", &op);
                while (getchar() != '\n');
                switch(op) {
                    case 1:
                        printf("\nCADASTRO DE BANCO > CONSULTA\n");
                        b=fopen("b.dat", "r");
                        if (b == NULL) {
                            fprintf(stderr, "\nErro tentando abrir o arquivo!\n");
                            exit(1);
                        }
                        while(fread(&entradaBanco, sizeof(struct Banco), 1, b))
                            printf ("ID: %d\nNome: %s\n\n", entradaBanco.idBanco, entradaBanco.nome);
                        fclose (b);
                        break;
                    case 2:
                        printf("\nCADASTRO DE BANCO > INSERÇÃO\n");
                        printf("Entre o nome do novo banco: ");
                        b=fopen("b.dat", "w");
                        if (b == NULL) {
                            fprintf(stderr, "\nErro tentando abrir o arquivo!\n");
                            exit(1);
                        }
                        struct Banco entradaBanco={1, 1, gets(entradaBanco.nome)};
                        fwrite(&entradaBanco, sizeof(struct Banco), 1, b);
                        if (fwrite != 0)
                            printf("Banco inserido com sucesso!\n");
                        else
                            printf("Erro ao tentar inserir!\n");
                        fclose(b);
                        break;
                    case 3:
                        printf("\nCADASTRO DE BANCO > REMOÇÃO\n");
                        break;
                    default:
                        printf("\nOPÇÃO INVÁLIDA!\n\n");
                }
                break;
            case 3:
                printf("\nCADASTRO DE PRODUTO\n");
                break;
            case 4:
                printf("\nOBRIGADO!\n");
                s=1;
                break;
            /* VALIDAÇÃO DO MENU PRINCIPAL */
            default:
                printf("\nOPÇÃO INVÁLIDA!\n\n");
        }
        if (s) break;
    }
    return 0;
}
