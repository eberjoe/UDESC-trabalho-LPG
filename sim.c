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
int ListaBancos(int);
int RemoveBanco(int);
int InsereProdutoParaBanco(char[], int, int, float, float, int, float);
int ConsultaProdutos(int, int, int);
int RemoveProduto(int);

/* OUTROS PROTÓTIPOS */
char* NomeBanco(int);
char* NomeProduto(int);
char* SistAm(int);

int main() {
    setlocale(LC_ALL, "Portuguese");
    char n[MAXNOME], invalido[]="\nVou fingir que não vi isso!\n\n";
    int op, s=0, sCb, sCp, idBanco, sistAmortizacao, prazoMax, filtroSist;
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
                    printf("\nSIMULAÇÃO\n\n");
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
                                    if (!ListaBancos(1))
                                        printf("\nNão há bancos cadastrados!\n\n");
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE BANCOS > INSERÇÃO\n\n");
                                    printf("Entre o nome do novo banco: ");
                                    gets(n);
                                    if (InsereBanco(n))
                                        printf("\nBanco inserido com sucesso!\n\n");
                                    else
                                        printf("\nErro ao inserir!\n\n");
                                    break;
                                case 3:
                                    printf("\nCADASTRO DE BANCOS > REMOÇÃO\n\n");
                                    if (!ListaBancos(0)) {
                                        printf("Não há nenhum banco cadastrado para remover!\n\n");
                                        break;
                                    }
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
                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                        }
                    }
                    break;
                case 3:
                    while(!sCp) {
                        printf("\n### CADASTRO DE PRODUTOS ###\n");
                        printf("[1] CONSULTA\n[2] INSERÇÃO\n[3] REMOÇÃO\n[4] VOLTA\n");
                        printf("Entre uma das opções acima: ");
                        if (scanf("%d", &op) && op >= 1 && op <= 4) { // valida a opção do menu como int dentro do intervalo de opções
                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                            switch(op) {
                                case 1:
                                    printf("\nCADASTRO DE PRODUTOS > CONSULTA\n\n");
                                    if (!ConsultaProdutos(0, 0, 0)) {
                                        printf("\nNão há produtos cadastrados!\n\n");
                                        break;
                                    }
                                    printf("\n");
                                    if (!ListaBancos(0)) {
                                        printf("\nNão há sequer bancos cadastrados, quanto menos produtos...\n\n");
                                        break;
                                    }
                                    printf("\nConsulta simples [1] ou detalhada [2] ? ");
                                    if (!scanf("%d", &op) || op != 1 && op != 2) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("\nEntre o ID do banco ou [0] para omitir filtro de banco: ");
                                    if (!scanf("%d", &idBanco)) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("\nEntre [1] para SAC, [2] para PRICE, ou [0] para omitir este filtro: ");
                                    if (!scanf("%d", &filtroSist) || filtroSist < 0 || filtroSist > 2) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("\n");
                                    if (!ConsultaProdutos(op-1, idBanco, filtroSist))
                                        printf("\nEsta consulta não gerou resultados!\n\n");
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE PRODUTOS > INSERÇÃO\n\n");
                                    if (!ListaBancos(0))
                                        printf("\nNão há bancos cadastrados! Cadastre pelo menos um banco antes de inserir um produto.\n\n");
                                    else {
                                        printf("\nEntre o ID do banco que oferece o produto: ");
                                        if (!scanf("%d", &id)) { // validação do ID do banco
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        else if (!NomeBanco(id)) {
                                            printf("Banco não encontrado! Entre um dos IDs da lista.\n\n");
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("\n%s\n", NomeBanco(id));
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre o nome do produto: ");
                                        gets(n);
                                        printf("\nEntre o sistema de amortização. [1] para SAC ou [2] para PRICE: ");
                                        if (!scanf("%d", &op) || op != 1 && op != 2) { // validação da opção de produto
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("\n%s\n", SistAm(op-1));
                                        printf("\nEntre a porcentagem máxima de financiamento: ");
                                        if (!scanf("%f", &maxPorcentFinanc) || maxPorcentFinanc <= 0 || maxPorcentFinanc > 100) { // validação
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("\n%.2f %%\n", maxPorcentFinanc);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre a taxa percentual efetiva de juros: ");
                                        if (!scanf("%f", &taxaEfetivaJuros) || taxaEfetivaJuros < 0) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("\n%.2f %%\n", taxaEfetivaJuros);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre a porcentagem máxima de comprometimento da renda: ");
                                        if (!scanf("%f", &maxPorcentRenda) || maxPorcentRenda <= 0 || maxPorcentRenda > 100) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("\n%.2f %%\n", maxPorcentRenda);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\nEntre o número máximo de parcelas ou [0] para sair sem inserir produto: ");
                                        if (!scanf("%d", &prazoMax) || prazoMax < 0) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        if (prazoMax) {
                                            printf("\n%d meses\n", prazoMax);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        }
                                        else
                                            break;
                                        if (InsereProdutoParaBanco(n, id, op-1, maxPorcentFinanc/100, taxaEfetivaJuros/100, prazoMax, maxPorcentRenda/100))
                                            printf("\nProduto inserido com sucesso!\n\n");
                                        else
                                            printf("\nErro ao inserir!\n\n");
                                    }
                                    break;
                                case 3:
                                    printf("\nCADASTRO DE PRODUTOS > REMOÇÃO\n\n");
                                    if (!ConsultaProdutos(0, 0, 0)) {
                                        printf("Não há nenhum produto cadastrado para remover!\n\n");
                                        break;
                                    }
                                    printf("\nEntre o ID do produto a ser removido: ");
                                    if (scanf("%d", &id)) { // valida o ID como int
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        if (RemoveProduto(id))
                                            printf("\n%s foi removido com sucesso!\n\n", NomeProduto(id));
                                        else
                                            printf("\nID não encontrado!\n\n");
                                    }
                                    else {
                                        printf("%s", invalido);
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    }
                                    break;
                                case 4:
                                    sCp=1;
                            }
                        }
                        else {
                            printf("%s", invalido);
                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
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
        fread(&id, sizeof(int), 1, bancos); // lê o contador de ID
        id++;
        rewind(bancos);
        fwrite(&id, sizeof(int), 1, bancos); // sobrescreve o contador de ID
    }
    else {
        id=1;
        bancos=fopen("b.bin", "wb"); // cria um arquivo do zero
        fwrite(&id, sizeof(int), 1, bancos);
    }
    fclose(bancos);
    entradaBanco.disponivel=1;
    entradaBanco.idBanco=id;
    strcpy(entradaBanco.nome, nome);
    bancos=fopen("b.bin", "rb+");
    fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
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

int ListaBancos(int modo) {
    int i=0, j, k;
    struct Banco *todosBancos=NULL, h;
    if (bancos=fopen("b.bin", "rb")) { //confere se já existe um arquivo de bancos
        fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
            if (leituraBanco.disponivel) {
                todosBancos=(struct Banco*) realloc(todosBancos, sizeof(struct Banco)*(i+1)); // realoca espaço no array de bancos a cada banco disponível encontrado
                todosBancos[i]=leituraBanco;
                i++;
            }
        }
        if (i > 1) { // ordena o array de bancos se este contiver mais de um banco
            for (j=0; j<i-1; j++) {
                for (k=j+1; k<i; k++) {
                    if (todosBancos[j].idBanco > todosBancos[k].idBanco) {
                        h=todosBancos[j];
                        todosBancos[j]=todosBancos[k];
                        todosBancos[k]=h;
                    }
                }
            }
        }
        if (i) {
            if (modo) { // impressão na tela modo elegante
                for (j=0; j<i; j++)
                    printf("ID:\t%d\nNome:\t%s\n\n", todosBancos[j].idBanco, todosBancos[j].nome);
            }
            else { // impressão na tela modo simples
                printf("ID\t| Nome do banco\n-----------------------\n");
                for (j=0; j<i; j++)
                    printf("%d\t| %s\n", todosBancos[j].idBanco, todosBancos[j].nome);
            }
        }
    }
    fclose(bancos);
    return i;
}

int RemoveBanco(int idBanco) {
    int i=0, c;
    bancos=fopen("b.bin", "rb+");
    fseek(bancos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, bancos)) {
        if (leituraBanco.idBanco == idBanco && leituraBanco.disponivel) {
            entradaBanco=leituraBanco;
            entradaBanco.disponivel=0;
            fseek(bancos, ftell(bancos)-sizeof(struct Banco), SEEK_SET); // retorna o cursor do arquivo para o início do registro a ser excluído
            fwrite(&entradaBanco, sizeof(struct Banco), 1, bancos);
            i++;
            break;
        }
    }
    fclose(bancos);
    if (produtos=fopen("p.bin", "rb+")) { // confere se já existe um arquivo de produtos
        fseek(produtos, 0, SEEK_END);
        c=ftell(produtos);
        rewind(produtos);
        fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) {
            if (leituraProduto.idBanco == idBanco && leituraProduto.disponivel) {
                entradaProduto=leituraProduto;
                entradaProduto.disponivel=0;
                fseek(produtos, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para o inicio do registro a ser excluído
                fwrite(&entradaProduto, sizeof(struct Produto), 1, produtos);
                fseek(produtos, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para evitar expansão do arquivo na próxima iteração
            }
        }
    }
    fclose(produtos);
    return i;
}

int InsereProdutoParaBanco(char nome[], int idBanco, int sist, float maxPorcentFinanc, float taxaEfetivaJuros, int prazoMax, float maxPorcentRenda) {
    int sobrescrita=0, i=0;
    if (produtos=fopen("p.bin", "rb+")) { // confere se o arquivo já existe para abri-lo sem apagar dados
        fread(&id, sizeof(int), 1, produtos); // lê o contador de ID
        id++;
        rewind(produtos);
        fwrite(&id, sizeof(int), 1, produtos); // sobrescreve o contador de ID
    }
    else {
        id=1;
        produtos=fopen("p.bin", "wb"); // cria um arquivo do zero
        fwrite(&id, sizeof(int), 1, produtos);
    }
    fclose(produtos);
    entradaProduto.disponivel=1;
    entradaProduto.idProduto=id;
    strcpy(entradaProduto.nome, nome);
    entradaProduto.idBanco=idBanco;
    entradaProduto.sistAmortizacao=sist;
    entradaProduto.maxPorcentFinanc=maxPorcentFinanc;
    entradaProduto.taxaEfetivaJuros=taxaEfetivaJuros;
    entradaProduto.prazoMax=prazoMax;
    entradaProduto.maxPorcentRenda=maxPorcentRenda;
    produtos=fopen("p.bin", "rb+");
    fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) { // percorre o arquivo
        if (!leituraProduto.disponivel) { // confere se há um registro excluído
            fseek(produtos, ftell(produtos)-sizeof(struct Produto), SEEK_SET); // retorna o cursor do arquivo para o início do registro excluído
            fwrite(&entradaProduto, sizeof(struct Produto), 1, produtos); //sobrescreve
            if (fwrite != 0)
                i++;
            sobrescrita=1;
            break;
        }
    }
    fclose(produtos);
    if (!sobrescrita) {
        produtos=fopen("p.bin", "ab"); // abre arquivo para adicionar novo registro ao final do arquivo
        fwrite(&entradaProduto, sizeof(struct Produto), 1, produtos);
        if (fwrite != 0)
            i++;
        fclose(produtos);
    }
    return i;
}

int ConsultaProdutos(int modo, int idBanco, int sist) {
    int i=0, j, k;
    struct Produto *todosProdutos=NULL, h;
    if (produtos=fopen("p.bin", "rb")) { // confere se já existe um arquivo de produtos
        fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) {
            if (leituraProduto.disponivel && (!idBanco && !sist || leituraProduto.idBanco == idBanco && leituraProduto.sistAmortizacao == sist-1 || leituraProduto.idBanco == idBanco && !sist || leituraProduto.sistAmortizacao == sist-1 && !idBanco)) {
                todosProdutos=(struct Produto*) realloc(todosProdutos, sizeof(struct Produto)*(i+1)); // realoca espaço no array de produtos a cada produto disponível encontrado
                todosProdutos[i]=leituraProduto;
                i++;
            }
        }
        if (i > 1) { // ordena o array de produtos se este contiver mais de um produto
            for (j=0; j<i-1; j++) {
                for (k=j+1; k<i; k++) {
                    if (todosProdutos[j].idProduto > todosProdutos[k].idProduto) {
                        h=todosProdutos[j];
                        todosProdutos[j]=todosProdutos[k];
                        todosProdutos[k]=h;
                    }
                }
            }
        }
        if (i) {
            if (modo) { // impressão no modo detalhado
                for (j=0; j<i; j++) {
                    printf("ID:\t\t%d\nNome:\t\t%s\nBanco:\t\t%s\nSistema:\t%s\n", todosProdutos[j].idProduto, todosProdutos[j].nome, NomeBanco(todosProdutos[j].idBanco), SistAm(todosProdutos[j].sistAmortizacao));
                    printf("Juros:\t\t%.2f %%\nMáx. fin.:\t%.2f %%\nPrazo máximo:\t%d meses\n", todosProdutos[j].taxaEfetivaJuros*100, todosProdutos[j].maxPorcentFinanc*100, todosProdutos[j].prazoMax);
                    printf("Máx. da renda:\t%.2f %%\n\n", todosProdutos[j].maxPorcentRenda*100);
                }
            }
            else { // impressão no modo resumido
                printf("ID\t| Nome do produto     | Banco\n------------------------------------------\n");
                for (j=0; j<i; j++)
                    printf("%d\t| %-20.20s| %s\n", todosProdutos[j].idProduto, todosProdutos[j].nome, NomeBanco(todosProdutos[j].idBanco));
            }
        }
    }
    fclose(produtos);
    return i;
}

int RemoveProduto(int idProduto){
    int i=0;
    produtos=fopen("p.bin", "rb+");
    fseek(produtos, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1, produtos)) {
        if (leituraProduto.idProduto == idProduto && leituraProduto.disponivel) {
            entradaProduto=leituraProduto;
            entradaProduto.disponivel=0;
            fseek(produtos, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para o inicio do registro a ser excluído
            fwrite(&entradaProduto, sizeof(struct Produto), 1, produtos);
            i++;
            break;
        }
    }
    fclose(produtos);
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
    }
    fclose(bancos);
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
    }
    fclose(produtos);
    return c;
}

char* SistAm(int s) {
    if (s)
        return "PRICE";
    return "SAC";
}