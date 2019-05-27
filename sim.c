#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
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
    int sistAmortizacao; // indica o sistema de amortização que pode ser SAC (0) ou PRICE (!=0)
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

/* PROTÓTIPOS DAS FUNÇÕES CRUD */
int InsereBanco(char[]);
int ListaBancos();
int RemoveBanco(int);
int InsereProdutoParaBanco(char[], int, float, float, int, float); //implementar
int ListaProdutos(); // implementar
int RemoveProduto(int); // implementar

/* OUTROS PROTÓTIPOS */
char* NomeBanco(int);
char* NomeProduto(int);

int main() {
    char n[MAXNOME];
    int op, s=0;
    float renda=0, valorBem=0, entrada=0;
    while (1) {
        printf("\n\nMENU PRINCIPAL\n");
        printf("[1] SIMULAÇÃO\n[2] CADASTRO DE BANCO\n[3] CADASTRO DE PRODUTO FINANCEIRO\n[4] SAIR\n");
        printf("Entre uma das opções acima: ");
        if (scanf("%d", &op) && op >= 1 && op <= 4) { // valida a opção do menu como int dentro do intervalo de opções
            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
            switch(op) {
                case 1:
                    printf("\nSIMULAÇÃO\n");
                    printf("Entre o valor da renda bruta mensal do contraente: ");
                    if (scanf("%f", &renda) && renda > 0) // valida o valor como float maior que zero
                        printf("R$ %.2f\n", renda);
                    else {
                        printf("\nEntrada inválida!\n\n");
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        break;
                    }
                    printf("Entre o valor do bem a ser adquirido: ");
                    if (scanf("%f", &valorBem) && valorBem > 0) // valida o valor como float maior que zero
                        printf("R$ %.2f\n", valorBem);
                    else {
                        printf("\nEntrada inválida!\n\n");
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        break;
                    }
                    printf("Entre o valor da entrada: ");
                    if (scanf("%f", &entrada) && entrada > 0 && entrada < valorBem) // valida o valor como float maior que zero e menor que o preço total
                        printf("R$ %.2f\n", entrada);
                    else {
                        printf("\nEntrada inválida!\n\n");
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                    }
                    break;
                case 2:
                    printf("\nCADASTRO DE BANCO\n");
                    printf("[1] CONSULTA\n[2] INSERÇÃO\n[3] REMOÇÃO\n");
                    printf("Entre uma das opções acima: ");
                    if (scanf("%d", &op) && op >= 1 && op <= 3) { // valida a opção do menu como int dentro do intervalo de opções
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        switch(op) {
                            case 1:
                                printf("\nCADASTRO DE BANCO > CONSULTA\n");
                                if (!ListaBancos())
                                    printf("\nNão há bancos cadastrados!\n\n");
                                break;
                            case 2:
                                printf("\nCADASTRO DE BANCO > INSERÇÃO\n");
                                printf("Entre o nome do novo banco: ");
                                gets(n);
                                if (InsereBanco(n))
                                    printf("\nBanco inserido com sucesso!\n\n");
                                else
                                    printf("\nErro ao inserir!\n\n");
                                break;
                            case 3:
                                printf("\nCADASTRO DE BANCO > REMOÇÃO\n");
                                printf("\nATENÇÃO! A remoção de um banco remove também todos os seus produtos!\n\n");
                                printf("Entre o ID do banco a ser removido: ");
                                if (scanf("%d", &id)) { // valida o ID como int
                                    while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    if (RemoveBanco(id))
                                        printf("\nO banco %s foi removido com sucesso!\n\n", NomeBanco(id));
                                    else
                                        printf("\nID não encontrado!\n\n");
                                }
                                else {
                                    printf("\nEntrada inválida!\n\n");
                                    while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                }
                        }
                    }
                    else {
                        printf("\nOPÇÃO INVÁLIDA!\n\n");
                        while (getchar() != '\n');
                    }
                    break;
                case 3:
                    printf("\nCADASTRO DE PRODUTO\n");
                    break;
                case 4:
                    printf("\nOBRIGADO!\n");
                    s=1;
            }
        }
        else {
            printf("\nOPÇÃO INVÁLIDA!\n\n");
            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
        }
        if (s) break;
    }
    return 0;
}

int InsereBanco(char nome[]) {
    int sobrescrita=0, i=0;
    if (bancos=fopen("b.bin", "rb+")) { // confere se o arquivo já existe para abri-lo sem apagar dados
        fread(&id, sizeof(int), 1, bancos);
        id++;
        rewind(bancos);
        fwrite(&id, sizeof(int), 1, bancos);
        fclose(bancos);
    }
    else {
        id=1;
        bancos=fopen("b.bin", "wb"); // cria um arquivo do zero
        fwrite(&id, sizeof(int), 1, bancos);
        fclose(bancos);
    }
    entradaBanco.disponivel=1;
    entradaBanco.idBanco=id;
    strcpy(entradaBanco.nome, nome);
    bancos=fopen("b.bin", "rb+");
    fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) { // percorre o arquivo
        if (!leituraBanco.disponivel) { // confere se há um registro excluído
            fseek(bancos, ftell(bancos)-sizeof(struct Banco), SEEK_SET); // retorna o cursor do arquivo para o início do registro excluído
            fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos); //sobrescreve
            if (fwrite != 0)
                i++;
            sobrescrita=1;
            break;
        }
    }
    fclose(bancos);
    if (!sobrescrita) {
        bancos=fopen("b.bin", "ab"); // abre arquivo para adicionar novo registro ao final do arquivo
        fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos);
        if (fwrite != 0)
            i++;
        fclose(bancos);
    }
    return i;
}

int ListaBancos() {
    int i=0;
    if (bancos=fopen("b.bin", "rb")) { //confere se já existe um arquivo de bancos
        if (bancos == NULL) {
            fprintf(stderr, "\nErro ao abrir o arquivo!\n\n");
            exit(1);
        }
        fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
            if (leituraBanco.disponivel) {
                printf("ID:\t%d\nNome:\t%s\n\n", leituraBanco.idBanco, leituraBanco.nome);
            }
        }
        fclose(bancos);
        i++;
    }
    return i;
}

int RemoveBanco(int idBanco) {
    int i=0;
    bancos=fopen("b.bin", "rb+");
    if (bancos == NULL) {
        fprintf(stderr, "\nErro ao abrir o arquivo!\n\n");
        exit(1);
    }
    fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
        if (leituraBanco.idBanco == idBanco && leituraBanco.disponivel) {
            entradaBanco=leituraBanco;
            entradaBanco.disponivel=0;
            fseek(bancos, ftell(bancos)-sizeof(struct Banco), SEEK_SET); // retorna o cursor do arquivo para o início do registro a ser excluído
            fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos);
            if (fwrite != 0)
                i++;
            else
                printf("\nErro ao remover banco!\n\n");
            break;
        }
    }
    fclose(bancos);
    if (produtos=fopen("p.bin", "rb+")) { // confere se já existe um arquivo de produtos
        fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) {
            if (leituraProduto.idBanco == idBanco && leituraProduto.disponivel) {
                entradaProduto=leituraProduto;
                entradaProduto.disponivel=0;
                fseek(produtos, ftell(produtos)-sizeof(struct Produto), SEEK_SET); // retorna o cursor para o inicio do registro a ser excluído
                fwrite(&entradaProduto, sizeof(struct Produto), 1, produtos);
            }
        }
        fclose(produtos);
    }
    return i;
}

char* NomeBanco(int idBanco) {
    char* c=NULL;
    if (bancos=fopen("b.bin", "rb")) { // confere se já existe um arquivo de bancos
        fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
            if (leituraBanco.idBanco == idBanco) {
                c=(char*) malloc(MAXNOME*sizeof(char));
                strcpy(c, leituraBanco.nome);
                break;
            }
        }
        fclose(bancos);
    }
    return c;
}

char* NomeProduto(int idProduto) {
    char* c=NULL;
    if (produtos=fopen("p.bin", "rb")) { // confere se já existe um arquivo de produtos
        fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) {
            if (leituraProduto.idProduto == idProduto) {
                c=(char*) malloc(MAXNOME*sizeof(char));
                strcpy(c, leituraProduto.nome);
                break;
            }
        }
        fclose(produtos);
    }
    return c;
}