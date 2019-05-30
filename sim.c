#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
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
int InsereProdutoParaBanco(char[], int, float, float, int, float); // implementar
int ListaProdutos(); // implementar
int RemoveProduto(int); // implementar
int ListaProdPorBanco(int); // implementar
int ListaProdPorTipo(int); // implementar

/* OUTROS PROTÓTIPOS */
char* NomeBanco(int);
char* NomeProduto(int);
int IdBanco(char[]); // implementar

int main() {
    setlocale(LC_ALL, "Portuguese");
    char n[MAXNOME], invalido[35]="\nVou fingir que não vi isso!\n\n";
    int op, s=0, sCb, sCp, idBanco, sistAmortizacao, prazoMax;
    float renda, valorBem, entrada, maxPorcentFinanc, taxaEfetivaJuros, maxPorcentRenda;
    while (!s) {
        sCb=0;
        sCp=0;
        printf("\n\n### MENU PRINCIPAL ###\n");
        printf("[1] SIMULAÇÃO\n[2] CADASTRO DE BANCOS\n[3] CADASTRO DE PRODUTOS FINANCEIROS\n[4] SAIR\n");
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
                        printf("%s", invalido);
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        break;
                    }
                    printf("Entre o valor do bem a ser adquirido: ");
                    if (scanf("%f", &valorBem) && valorBem > 0) // valida o valor como float maior que zero
                        printf("R$ %.2f\n", valorBem);
                    else {
                        printf("%s", invalido);
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        break;
                    }
                    printf("Entre o valor da entrada: ");
                    if (scanf("%f", &entrada) && entrada > 0 && entrada < valorBem) // valida o valor como float maior que zero e menor que o preço total
                        printf("R$ %.2f\n", entrada);
                    else {
                        printf("%s", invalido);
                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                    }
                    break;
                case 2:
                    while(!sCb) {
                        printf("\n### CADASTRO DE BANCOS ###\n");
                        printf("[1] LISTA\n[2] INSERÇÃO\n[3] REMOÇÃO\n[4] VOLTA\n");
                        printf("Entre uma das opções acima: ");
                        if (scanf("%d", &op) && op >= 1 && op <= 4) { // valida a opção do menu como int dentro do intervalo de opções
                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                            switch(op) {
                                case 1:
                                    printf("\nCADASTRO DE BANCOS > LISTA\n\n");
                                    if (!ListaBancos())
                                        printf("\nNão há bancos cadastrados!\n\n");
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE BANCOS > INSERÇÃO\n");
                                    printf("\nEntre o nome do novo banco: ");
                                    gets(n);
                                    if (InsereBanco(n))
                                        printf("\nBanco inserido com sucesso!\n\n");
                                    else
                                        printf("\nErro ao inserir!\n\n");
                                    break;
                                case 3:
                                    printf("\nCADASTRO DE BANCOS > REMOÇÃO\n");
                                    printf("\nATENÇÃO! A remoção de um banco remove também todos os seus produtos!\n\n");
                                    printf("Entre o ID do banco a ser removido: ");
                                    if (scanf("%d", &id)) { // valida o ID como int
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        if (RemoveBanco(id))
                                            printf("\n%s foi removido com sucesso!\n\n", NomeBanco(id));
                                        else
                                            printf("\nID não encontrado!\n\n");
                                    }
                                    else {
                                        printf("%s", invalido);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    }
                                    break;
                                case 4:
                                    sCb=1;
                            }
                        }
                        else {
                            printf("%s", invalido);
                            while (getchar() != '\n');
                        }
                    }
                    break;
                case 3:
                    while(!sCp) {
                        printf("\n### CADASTRO DE PRODUTOS ###\n");
                        printf("[1] LISTA\n[2] CONSULTA POR BANCO\n[3] CONSULTA POR TIPO\n[4] INSERÇÃO\n[5] EDIÇÃO\n[6] REMOÇÃO\n[7] VOLTA\n");
                        printf("Entre uma das opções acima: ");
                        if (scanf("%d", &op) && op >= 1 && op <= 7) { // valida a opção do menu como int dentro do intervalo de opções
                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                            switch(op) {
                                case 1:
                                    printf("\nCADASTRO DE PRODUTOS > LISTA\n");
                                    if (!ListaProdutos())
                                        printf("\nNão há produtos cadastrados!\n\n");
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE PRODUTOS > CONSULTA POR BANCO\n");
                                    printf("\nEntre o ID do banco: ");
                                    if (scanf("%d", &id)) { // valida ID como int
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        if (!ListaProdPorBanco(id))
                                            printf("\nBanco não encontrado!\n\n");
                                    }
                                    else {
                                        printf("%s", invalido);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    }
                                    break;
                                case 3:
                                    printf("\nCADASTRO DE PRODUTOS > CONSULTA POR TIPO\n");
                                    printf("\nEntre [1] para SAC ou [2] para PRICE: ");
                                    if (scanf("%d", &op) && (op == 1 || op == 2)) { // valida opção como int entre 1 e 2
                                        if (!ListaProdPorTipo(op))
                                            printf("\nNão há produtos deste tipo!\n\n");
                                    }
                                    else {
                                        printf("%s", invalido);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    }
                                    break;
                                case 4:
                                    printf("\nCADASTRO DE PRODUTOS > INSERÇÃO\n");
                                    if (!ListaBancos())
                                        printf("\nNão há bancos cadastrados! Cadastre pelo menos um banco antes de inserir um produto.\n\n");
                                    else {
                                        printf("\nEntre o ID do banco que oferece o produto: ");
                                        if (!scanf("%d", &id) || !NomeBanco(id)) { // validação do ID do banco
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("\n%s\n", NomeBanco(id));
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre o nome do produto: ");
                                        gets(n);
                                        printf("\nEntre o sistema de amortização. [1] para SAC ou [2] para PRICE: ");
                                        if (!scanf("%d", &op) || op != 1 && op != 2) { // validação da opção de produto
                                            printf("%s", invalido);
                                            break;
                                        }
                                        if (op == 1)
                                            printf("\nSAC\n");
                                        else
                                            printf("\nPRICE\n");
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre a porcentagem máxima de financiamento: ");
                                        if (!scanf("%f", &maxPorcentFinanc) || maxPorcentFinanc <= 0 || maxPorcentFinanc > 100) { // validação
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("\n%.2f %%\n", maxPorcentFinanc);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre a taxa percentual efetiva de juros: ");
                                        if (!scanf("%f", &taxaEfetivaJuros) || taxaEfetivaJuros < 0) { // validação
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("\n%.2f %%\n", taxaEfetivaJuros);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre a porcentagem máxima de comprometimento da renda: ");
                                        if (!scanf("%f", &maxPorcentRenda) || maxPorcentRenda <= 0 || maxPorcentRenda > 100) { // validação
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("\n%.2f %%\n", maxPorcentRenda);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre o número máximo de parcelas ou [0] para sair sem inserir produto: ");
                                        if (!scanf("%d", &prazoMax) || prazoMax < 0) { // validação
                                            printf("%s", invalido);
                                            break;
                                        }
                                        if (prazoMax) {
                                            printf("\n%d meses\n", prazoMax);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        }
                                        else
                                            break;
                                        if (InsereProdutoParaBanco(n, id, maxPorcentFinanc, taxaEfetivaJuros, prazoMax, maxPorcentRenda))
                                            printf("\nProduto inserido com sucesso!\n\n");
                                        else
                                            printf("\nErro ao inserir!\n\n");
                                    }
                                    break;
                                case 5:
                                    printf("\nCADASTRO DE PRODUTOS > EDIÇÃO\n"); // COMPLETAR
                                    break;
                                case 6:
                                    printf("\nCADASTRO DE PRODUTOS > REMOÇÃO\n"); //COMPLETAR
                                    break;
                                case 7:
                                    sCp=1;
                            }
                        }
                        else {
                            printf("%s", invalido);
                            while (getchar() != '\n');
                        }
                    }
                    break;
                case 4:
                    printf("\nOBRIGADO!\n");
                    s=1;
            }
        }
        else {
            printf("%s", invalido);
            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
        }
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
    int i=0, j;
    struct Banco *todosBancos=NULL, h;
    if (bancos=fopen("b.bin", "rb")) { //confere se já existe um arquivo de bancos
        fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
            if (leituraBanco.disponivel) {
                todosBancos=(struct Banco*) realloc(todosBancos, sizeof(struct Banco)*(i+1)); // realoca espaço no array de bancos a cada banco disponível encontrado
                todosBancos[i]=leituraBanco;
                i++;
            }
        }
        fclose(bancos);
        if (i > 1) { // ordena o array de bancos se este contiver mais de um banco
            for (j=0; j<i-1; j++) {
                if (todosBancos[j].idBanco > todosBancos[j+1].idBanco) {
                    h=todosBancos[j];
                    todosBancos[j]=todosBancos[j+1];
                    todosBancos[j+1]=h;
                }
            }
        }
        if (i) {
            for (j=0; j<i; j++) { // impressão na tela
                printf("ID:\t%d\nNome:\t%s\n\n", todosBancos[j].idBanco, todosBancos[j].nome);
            }
        }
    }
    return i;
}

int RemoveBanco(int idBanco) {
    int i=0;
    bancos=fopen("b.bin", "rb+");
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

int InsereProdutoParaBanco(char nome[], int idBanco, float maxPorcentFinanc, float taxaEfetivaJuros, int prazoMax, float maxPorcentRenda) {
    return 0;
}

int ListaProdutos() {
    return 0;
}

int RemoveProduto(int idProduto){
    return 0;
}

int ListaProdPorBanco(int idBanco) {
    return 0;
}

int ListaProdPorTipo(int tipo) {
    return 0;
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

int idBanco(char nome[]) {
    return 0;
}