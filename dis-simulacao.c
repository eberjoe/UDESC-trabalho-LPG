#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
#include "validacao.h"
#define MAXNOME 100

/* ESTRUTURAS */
struct Banco {
    int idBanco; // identificador da instituição financeira
    int disponivel; // flag indicando se a instituição financeira está disponível ou não
    char nome[MAXNOME]; // nome da instituição financeira
};

struct Produto {
    int idProduto; // identificador do produto
    int disponivel; // flag indicando se o produto está disponível ou não
    int idBanco; // identificador da instituição financeira à qual o produto pertence (chave externa)
    char nome[MAXNOME]; // nome do produto
    char sistAmortizacao; // caracter indicando o sistema de amortização que pode ser SAC ('S') ou PRICE ('P')
    float maxPorcentFinanc; // número entre 0 e 1 indicando a máxima porção financiável do preço total
    float taxaEfetivaJuros; // número entre 0 e 1 indicando a taxa efetiva de juros
    int prazoMax; // número inteiro indicando a máxima quantidade de meses permitida para o financiamento
    float maxPorcentRenda; // número entre 0 e 1 indicando o máximo comprometimento da renda permitido
};

/* GLOBAIS (EXCETO ATORES, JORNALISTAS E APRESENTADORES) */
FILE *bancos, *produtos;
int id;
struct Banco leituraBanco, entradaBanco;
struct Produto leituraProduto, entradaProduto;

/* FUNÇÕES CRUD */
void InsereBanco(char nome[MAXNOME]) {
    int sobrescrita=0;
    if (bancos=fopen("b.bin", "rb+")) { // confere se o arquivo já existe para abri-lo sem apagar dados
        fread(&id, sizeof(int), 1, bancos);
        id++;
        rewind(bancos);
        fwrite(&id, sizeof(int), 1, bancos);
        fclose(bancos);
    }
    else {
        id=1;
        bancos=fopen("b.bin", "wb");
        fwrite(&id, sizeof(int), 1, bancos);
        fclose(bancos);
    }
    entradaBanco.disponivel=1;
    entradaBanco.idBanco=id;
    strcpy(entradaBanco.nome, nome);
    bancos=fopen("b.bin", "rb+");
    fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) { // percorre o arquivo
        if (!leituraBanco.disponivel) { // confere se há um registro excluído
            fseek(bancos, ftell(bancos)-sizeof(struct Banco), SEEK_SET); // retorna o cursor do arquivo para o início do registro excluído
            fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos); //sobrescreve
            if (fwrite != 0)
                printf("\nBanco inserido com sucesso!\n\n");
            else
                printf("\nErro ao inserir!\n\n");
            sobrescrita=1;
            break;
        }
    }
    fclose(bancos);
    if (!sobrescrita) {
        bancos=fopen("b.bin", "ab+");
        fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos);
        if (fwrite != 0)
            printf("\nBanco inserido com sucesso!\n\n");
        else
            printf("\nErro ao inserir!\n\n");
        fclose(bancos);
    }
    return;
}

int main() {
    char n[MAXNOME];
    int op, id, s=0;
    float renda, valorBem, entrada;
    while (1) {
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
                while (1) {
                    printf("Entre o valor do bem a ser adquirido: ");
                    if (ValidaFloat(&valorBem)) {
                        printf("%.2f\n", valorBem);
                        break;
                    }
                    printf("\nEntrada inválida!\n\n");
                }
                while (1) {
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
                        bancos=fopen("b.bin", "rb");
                        if (bancos == NULL) {
                            fprintf(stderr, "\nErro tentando abrir o arquivo!\n\n");
                            exit(1);
                        }
                        fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao ID
                        while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
                            if (leituraBanco.disponivel)
                                printf ("ID: %d\nNome: %s\n\n", leituraBanco.idBanco, leituraBanco.nome);
                        }
                        fclose (bancos);
                        break;
                    case 2:
                        printf("\nCADASTRO DE BANCO > INSERÇÃO\n");
                        printf("Entre o nome do novo banco: ");
                        gets(n);
                        InsereBanco(n);
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
