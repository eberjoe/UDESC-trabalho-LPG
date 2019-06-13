#include <stdio.h>
#include <locale.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>
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

struct Financiamento {
    int idProduto;
    int idBanco;
    int sistAmortizacao;
    float taxaNominalAnual; // taxa de juros nominal ao ano com capitalização mensal = taxaEfetivaJuros*12
    int prazo; // prazo para o financiamento estipulado pelo cliente
    float primeiraParcela; // simulação da primeira parcela
    float ultimaParcela; // simulação da última parcela
};

/* DADOS GLOBAIS */
FILE *arquivo; // ponteiro para os arquivos dos registros
struct Banco leituraBanco; // armazena o registro lido do arquivo de bancos
struct Produto leituraProduto; // armazena o registro lido do arquivo de produtos
struct Financiamento *poolFin; // armazena os resultados da prospecção e simulação

/* PROTÓTIPOS DAS FUNÇÕES CRUD */
int InsereBanco(struct Banco); // retorna êxito da inserção
int ListaBancos(int /*0: lista em tabela; 1: lista elegante*/); //retorna número de registros de bancos
int RemoveBanco(int /*ID do banco*/); // retorna êxito da remoção
void EditaBanco(struct Banco);
int InsereProdutoParaBanco(struct Produto); // retorna êxito da inserção
int ConsultaProdutos(int /*modo -- negativo: sem impressão; 0: impressão simples; 1: impressão detalhada)*/, int /*ID (se zero, os próximos parâmetros são mandatórios)*/, .../* opcionais int filtro de banco e int filtro de sistema de amort.*/); // retorna número de resultados
int RemoveProduto(int /*ID do produto*/); // retorna êxito da remoção
void EditaProduto(struct Produto);

/* OUTROS PROTÓTIPOS */
char* NomeBanco(int); // retorna nome do banco em função do ID
char* NomeProduto(int); // retorna nome do produto em função do ID
int Prospecta(float /*renda*/, float /*valor do bem*/, float /*entrada*/, int /*prazo em meses*/); //popula poolFin com planos compatíveis com o perfil do cliente
char* SistAm(int); //retorna "SAC" para 0 ou "PRICE" para 1

int main() {
    setlocale(LC_ALL, "Portuguese");
    char invalido[]="\nVou fingir que não vi isso!\n\n";
    int i, id, op, s=0, sCb, sCp, filtroIdBanco, filtroSist, prazo, tamPoolFin;
    float renda, valorBem, entrada;
    struct Banco entradaBanco;
    struct Produto entradaProduto;
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
                    printf("\n### SIMULAÇÃO ###\n");
                    if (!ConsultaProdutos(-1, 0, 0, 0)) {
                        printf("\nNão há produtos financeiros cadastrados!\n");
                        break;
                    }
                    printf("\nEntre o valor da renda bruta mensal do contraente: ");
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
                        break;
                    }
                    printf("Entre o número de parcelas: ");
                    if (scanf("%d", &prazo) && prazo > 1) { // valida o número de parcelas como inteiro maior que zero
                        if (tamPoolFin=Prospecta(renda, valorBem, entrada, prazo)) {
                            printf("\nTemos os seguintes planos para o seu caso:\n");
                            for (i=0; i<tamPoolFin; i++) {
                                printf("\nFinanciamento %s do banco %s, no sistema de amortização %s, em %d vezes e taxa nominal de %.2f %% a.a.\n", NomeProduto(poolFin[i].idProduto), NomeBanco(poolFin[i].idBanco), SistAm(poolFin[i].sistAmortizacao), prazo, poolFin[i].taxaNominalAnual);
                                printf("Primeira parcela no valor de R$ %.2f, e última parcela no valor de R$ %.2f.\n", poolFin[i].primeiraParcela, poolFin[i].ultimaParcela);
                            }
                        }
                        else
                            printf("\nAtualmente não dispomos de planos de financiamento que atendam as suas exigências.");
                        free(poolFin);
                    }
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
                                    if (!ListaBancos(1)) {
                                        printf("\nNão há bancos cadastrados!\n\n");
                                        break;
                                    }
                                    printf("\nEntre o ID de um banco para editá-lo, ou [0] para voltar: ");
                                    if (scanf("%d", &id) && id >= 0) {
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        if (!id)
                                            break;
                                        if (!NomeBanco(id) || !leituraBanco.disponivel) {
                                            printf("\nID não encontrado!\n");
                                            break;
                                        }
                                        printf("\nEntre o novo nome para %s: ", NomeBanco(id));
                                        entradaBanco=leituraBanco;
                                        fgets(entradaBanco.nome, MAXNOME+1, stdin); // fgets adiciona '\n' ao final da string
                                        strtok(entradaBanco.nome, "\n"); // segmenta a string usando '\n' como delimitador
                                        EditaBanco(entradaBanco);
                                        break;
                                    }
                                    printf("%s", invalido);
                                    while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE BANCOS > INSERÇÃO\n\n");
                                    printf("Entre o nome do novo banco: ");
                                    fgets(entradaBanco.nome, MAXNOME+1, stdin); // fgets adiciona '\n' ao final da string
                                    strtok(entradaBanco.nome, "\n"); // segmenta a string usando '\n' como delimitador
                                    if (InsereBanco(entradaBanco))
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
                                    if (!ListaBancos(0)) {
                                        printf("Não há sequer bancos cadastrados, quanto menos produtos...\n\n");
                                        break;
                                    }
                                    printf("\n");
                                    if (!ConsultaProdutos(0, 0, 0, 0)) {
                                        printf("Não há produtos cadastrados!\n\n");
                                        break;
                                    }
                                    printf("\nConsulta simples [1] ou detalhada [2] ? ");
                                    if (!scanf("%d", &op) || op != 1 && op != 2) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("Entre o ID do banco ou [0] para omitir filtro de banco: ");
                                    if (!scanf("%d", &filtroIdBanco)) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("Entre [1] para SAC, [2] para PRICE, ou [0] para omitir este filtro: ");
                                    if (!scanf("%d", &filtroSist) || filtroSist < 0 || filtroSist > 2) { // validação
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    printf("\n");
                                    if (!ConsultaProdutos(op-1, 0, filtroIdBanco, filtroSist)) { // executa a consulta com filtros
                                        printf("\nEsta consulta não gerou resultados!\n\n");
                                        break;
                                    }
                                    printf("\nEntre o ID de um produto para editá-lo, ou [0] para voltar: ");
                                    if (scanf("%d", &id) && id > 0) {
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("\n");
                                        if (!ConsultaProdutos(1,id)) { // valida a existência e a disponibilidade do produto, imprimindo seus dados detalhados em caso positivo
                                            printf("\nID não encontrado!\n");
                                            break;
                                        }
                                        do {
                                            printf("\nEntre\n[1] para editar o nome,\n[2] para editar o sistema de amortização,\n[3] para editar a taxa de juros,\n[4] para editar o máximo percentual financiável,\n[5] para editar o prazo máximo,\n[6] para editar o percentual máximo de comprometimento da renda, ou\n[0] para sair da edição\n> ");
                                            if (scanf("%d", &op) && op >= 0 && op <= 6) {
                                                while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                entradaProduto=leituraProduto;
                                                switch (op) {
                                                    case 1:
                                                        printf("\nEntre o novo nome: ");
                                                        fgets(entradaProduto.nome, MAXNOME+1, stdin); // fgets adiciona '\n' ao final da string
                                                        strtok(entradaProduto.nome, "\n"); // segmenta a string usando '\n' como delimitador
                                                        break;
                                                    case 2:
                                                        printf("\nO sistema foi mudado para %s\n", SistAm(!leituraProduto.sistAmortizacao));
                                                        entradaProduto.sistAmortizacao=!leituraProduto.sistAmortizacao;
                                                        break;
                                                    case 3:
                                                        printf("\nEntre a nova taxa mensal de juros: ");
                                                        if (scanf("%f", &entradaProduto.taxaEfetivaJuros) && entradaProduto.taxaEfetivaJuros >= 0) {
                                                            entradaProduto.taxaEfetivaJuros/=100;
                                                            break;
                                                        }
                                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                        printf("%s", invalido);
                                                        break;
                                                    case 4:
                                                        printf("\nEntre o novo percentual financiável: ");
                                                        if (scanf("%f", &entradaProduto.maxPorcentFinanc) && entradaProduto.maxPorcentFinanc > 0) {
                                                            entradaProduto.maxPorcentFinanc/=100;
                                                            break;
                                                        }
                                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                        printf("%s", invalido);
                                                        break;
                                                    case 5:
                                                        printf("\nEntre o novo prazo máximo de financiamento: ");
                                                        if (scanf("%d", &entradaProduto.prazoMax) && entradaProduto.prazoMax >0)
                                                            break;
                                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                        printf("%s", invalido);
                                                        break;
                                                    case 6:
                                                        printf("\nEntre o novo percentual máximo de comprometimento da renda: ");
                                                        if (scanf("%f", &entradaProduto.maxPorcentRenda) && entradaProduto.maxPorcentRenda >= 0 && entradaProduto.maxPorcentRenda <= 100) {
                                                            entradaProduto.maxPorcentRenda/=100;
                                                            break;
                                                        }
                                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                        printf("%s", invalido);
                                                        break;
                                                }
                                                EditaProduto(entradaProduto);
                                                printf("\n");
                                                ConsultaProdutos(1, entradaProduto.idProduto);
                                            }
                                            else {
                                                while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                                printf("%s", invalido);
                                                break;
                                            }
                                        } while (op);
                                        break;
                                    }
                                    else {
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("%s", invalido);
                                        break;
                                    }
                                    break;
                                case 2:
                                    printf("\nCADASTRO DE PRODUTOS > INSERÇÃO\n\n");
                                    if (!ListaBancos(0))
                                        printf("\nNão há bancos cadastrados! Cadastre pelo menos um banco antes de inserir um produto.\n\n");
                                    else {
                                        printf("\nEntre o ID do banco que oferece o produto: ");
                                        if (!scanf("%d", &entradaProduto.idBanco)) { // validação do ID do banco
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        else if (!NomeBanco(entradaProduto.idBanco)) {
                                            printf("\nBanco não encontrado! Entre um dos IDs da lista.\n\n");
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("%s\n", NomeBanco(entradaProduto.idBanco));
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("Entre o nome do produto: ");
                                        fgets(entradaProduto.nome, MAXNOME+1, stdin); // fgets adiciona '\n' ao final da string
                                        strtok(entradaProduto.nome, "\n"); // segmenta a string usando '\n' como delimitador
                                        printf("\nEntre o sistema de amortização. [1] para SAC ou [2] para PRICE: ");
                                        if (!scanf("%d", &op) || op != 1 && op != 2) { // validação da opção de produto
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            printf("%s", invalido);
                                            break;
                                        }
                                        printf("%s\n", SistAm(op-1));
                                        entradaProduto.sistAmortizacao=op-1;
                                        printf("Entre a porcentagem máxima de financiamento: ");
                                        if (!scanf("%f", &entradaProduto.maxPorcentFinanc) || entradaProduto.maxPorcentFinanc <= 0 || entradaProduto.maxPorcentFinanc > 100) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("%.2f %%\n", entradaProduto.maxPorcentFinanc);
                                        entradaProduto.maxPorcentFinanc/=100;
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("Entre a taxa percentual efetiva de juros: ");
                                        if (!scanf("%f", &entradaProduto.taxaEfetivaJuros) || entradaProduto.taxaEfetivaJuros < 0) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("%.2f %%\n", entradaProduto.taxaEfetivaJuros);
                                        entradaProduto.taxaEfetivaJuros/=100;
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("Entre a porcentagem máxima de comprometimento da renda: ");
                                        if (!scanf("%f", &entradaProduto.maxPorcentRenda) || entradaProduto.maxPorcentRenda <= 0 || entradaProduto.maxPorcentRenda > 100) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        printf("%.2f %%\n", entradaProduto.maxPorcentRenda);
                                        entradaProduto.maxPorcentRenda/=100;
                                        while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        printf("Entre o número máximo de parcelas ou [0] para sair sem inserir produto: ");
                                        if (!scanf("%d", &entradaProduto.prazoMax) || entradaProduto.prazoMax < 0) { // validação
                                            printf("%s", invalido);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                            break;
                                        }
                                        if (entradaProduto.prazoMax) {
                                            printf("%d meses\n", entradaProduto.prazoMax);
                                            while (getchar() != '\n'); // consome o retorno de linha em excesso da entrada do usuário
                                        }
                                        else
                                            break;
                                        if (InsereProdutoParaBanco(entradaProduto))
                                            printf("\nProduto inserido com sucesso!\n\n");
                                        else
                                            printf("\nErro ao inserir!\n\n");
                                    }
                                    break;
                                case 3:
                                    printf("\nCADASTRO DE PRODUTOS > REMOÇÃO\n\n");
                                    if (!ConsultaProdutos(0, 0, 0, 0)) {
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

int InsereBanco(struct Banco banco) {
    int sobrescrita=0, i=0, id;
    if (arquivo=fopen("b.bin", "rb+")) { // confere se o arquivo já existe para abri-lo sem apagar dados
        fread(&id, sizeof(int), 1, arquivo); // lê o contador de ID
        id++;
        rewind(arquivo);
        fwrite(&id, sizeof(int), 1, arquivo); // sobrescreve o contador de ID
    }
    else {
        id=1;
        arquivo=fopen("b.bin", "wb"); // cria um arquivo do zero
        fwrite(&id, sizeof(int), 1, arquivo);
    }
    fclose(arquivo);
    banco.disponivel=1;
    banco.idBanco=id;
    arquivo=fopen("b.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, arquivo)) { // percorre o arquivo
        if (!leituraBanco.disponivel) { // confere se há um registro excluído
            fseek(arquivo, -sizeof(struct Banco), SEEK_CUR); // retorna o cursor do arquivo para o início do registro excluído
            if (fwrite(&banco, sizeof(struct Banco), 1, arquivo)) //sobrescreve
                i++;
            sobrescrita=1;
            break;
        }
    }
    fclose(arquivo);
    if (!sobrescrita) {
        arquivo=fopen("b.bin", "ab"); // abre arquivo para adicionar novo registro ao final do arquivo
        if (fwrite(&banco, sizeof(struct Banco), 1, arquivo))
            i++;
        fclose(arquivo);
    }
    return i;
}

int ListaBancos(int modo) {
    int i=0, j, k;
    struct Banco *todosBancos=NULL, h;
    if (arquivo=fopen("b.bin", "rb")) { //confere se já existe um arquivo de bancos
        fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, arquivo)) {
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
                printf("+-----------------------------+\n");
                printf("| ID\t| Nome do banco       |\n|-----------------------------|\n");
                for (j=0; j<i; j++)
                    printf("| %d\t| %-20.20s|\n", todosBancos[j].idBanco, todosBancos[j].nome);
                printf("+-----------------------------+\n");
            }
        }
    }
    fclose(arquivo);
    free(todosBancos);
    return i;
}

int RemoveBanco(int idBanco) {
    int i=0, c;
    arquivo=fopen("b.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, arquivo)) {
        if (leituraBanco.idBanco == idBanco && leituraBanco.disponivel) {
            leituraBanco.disponivel=0;
            fseek(arquivo, -sizeof(struct Banco), SEEK_CUR); // retorna o cursor do arquivo para o início do registro a ser excluído
            if (fwrite(&leituraBanco, sizeof(struct Banco), 1, arquivo))
                i++;
            break;
        }
    }
    fclose(arquivo);
    if ((arquivo=fopen("p.bin", "rb+")) && i) { // confere se já existe um arquivo de produtos
        fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) {
            if (leituraProduto.idBanco == idBanco && leituraProduto.disponivel) {
                leituraProduto.disponivel=0;
                fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para o inicio do registro a ser excluído
                fwrite(&leituraProduto, sizeof(struct Produto), 1, arquivo);
                fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para evitar expansão do arquivo na próxima iteração
            }
        }
    }
    fclose(arquivo);
    return i;
}

void EditaBanco(struct Banco banco) {
    arquivo=fopen("b.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraBanco, sizeof(struct Banco), 1, arquivo)) {
        if (leituraBanco.idBanco == banco.idBanco) {
            fseek(arquivo, -sizeof(struct Banco), SEEK_CUR); // retorna o cursor para o inicio do registro a ser editado
            fwrite(&banco, sizeof(struct Banco), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
}

int InsereProdutoParaBanco(struct Produto produto) {
    int sobrescrita=0, i=0, id;
    if (arquivo=fopen("p.bin", "rb+")) { // confere se o arquivo já existe para abri-lo sem apagar dados
        fread(&id, sizeof(int), 1, arquivo); // lê o contador de ID
        id++;
        rewind(arquivo);
        fwrite(&id, sizeof(int), 1, arquivo); // sobrescreve o contador de ID
    }
    else {
        id=1;
        arquivo=fopen("p.bin", "wb"); // cria um arquivo do zero
        fwrite(&id, sizeof(int), 1, arquivo);
    }
    fclose(arquivo);
    produto.disponivel=1;
    produto.idProduto=id;
    arquivo=fopen("p.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) { // percorre o arquivo
        if (!leituraProduto.disponivel) { // confere se há um registro excluído
            fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor do arquivo para o início do registro excluído
            if (fwrite(&produto, sizeof(struct Produto), 1, arquivo)) //sobrescreve
                i++;
            sobrescrita=1;
            break;
        }
    }
    fclose(arquivo);
    if (!sobrescrita) {
        arquivo=fopen("p.bin", "ab"); // abre arquivo para adicionar novo registro ao final do arquivo
        if (fwrite(&produto, sizeof(struct Produto), 1, arquivo))
            i++;
        fclose(arquivo);
    }
    return i;
}

int ConsultaProdutos(int modo, int idProduto, ...) {
    int i=0, j, k, idBanco=0, sistema=0;
    struct Produto *todosProdutos=NULL, h;
    va_list intArgumentPointer;
    va_start(intArgumentPointer, idProduto);
    if (!idProduto) {
        idBanco=va_arg(intArgumentPointer, int);
        sistema=va_arg(intArgumentPointer, int);
    }
    va_end(intArgumentPointer);
    if (arquivo=fopen("p.bin", "rb")) { // confere se já existe um arquivo de produtos
        fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) {
            if (leituraProduto.disponivel && ((!idBanco || leituraProduto.idBanco == idBanco) && (!sistema || leituraProduto.sistAmortizacao == sistema-1) && (!idProduto || leituraProduto.idProduto == idProduto))) {
                todosProdutos=(struct Produto*) realloc(todosProdutos, sizeof(struct Produto)*(i+1)); // realoca espaço no array de produtos a cada produto disponível encontrado
                todosProdutos[i]=leituraProduto;
                i++;
                if (idProduto)
                    break;
            }
        }
        if (i > 1) { // ordena o array de produtos para impressão se este contiver mais de um produto
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
            if (modo > 0) { // impressão no modo detalhado
                for (j=0; j<i; j++) {
                    printf("ID:\t\t%d\nNome:\t\t%s\nBanco:\t\t%s\nSistema:\t%s\n", todosProdutos[j].idProduto, todosProdutos[j].nome, NomeBanco(todosProdutos[j].idBanco), SistAm(todosProdutos[j].sistAmortizacao));
                    printf("Juros a.m.:\t%.2f %%\nMáx. fin.:\t%.2f %%\nPrazo máximo:\t%d meses\n", todosProdutos[j].taxaEfetivaJuros*100, todosProdutos[j].maxPorcentFinanc*100, todosProdutos[j].prazoMax);
                    printf("Máx. da renda:\t%.2f %%\n\n", todosProdutos[j].maxPorcentRenda*100);
                }
            }
            else if (!modo) { // impressão no modo resumido
                printf("+----------------------------------------------------------------------------------+\n");
                printf("| ID\t| Nome do produto          | Banco               | Amort.    | Taxa %% a.m. |\n|----------------------------------------------------------------------------------|\n");
                for (j=0; j<i; j++)
                    printf("| %d\t| %-25.25s| %-20.20s| %-10.10s| %-12.2f|\n", todosProdutos[j].idProduto, todosProdutos[j].nome, NomeBanco(todosProdutos[j].idBanco), SistAm(todosProdutos[j].sistAmortizacao), todosProdutos[j].taxaEfetivaJuros*100);
                printf("+----------------------------------------------------------------------------------+\n");
            }
        }
    }
    fclose(arquivo);
    free(todosProdutos);
    return i;
}

int RemoveProduto(int idProduto){
    int i=0;
    arquivo=fopen("p.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) {
        if (leituraProduto.idProduto == idProduto && leituraProduto.disponivel) {
            leituraProduto.disponivel=0;
            fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor para o inicio do registro a ser excluído
            if (fwrite(&leituraProduto, sizeof(struct Produto), 1, arquivo))
                i++;
            break;
        }
    }
    fclose(arquivo);
    return i;
}

void EditaProduto(struct Produto produto) {
    arquivo=fopen("p.bin", "rb+");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador de ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) {
        if (leituraProduto.idProduto == produto.idProduto) {
            fseek(arquivo, -sizeof(struct Produto), SEEK_CUR); // retorna o cursor do arquivo para o início do registro a ser editado
            fwrite(&produto, sizeof(struct Produto), 1, arquivo);
            break;
        }
    }
    fclose(arquivo);
}

char* NomeBanco(int idBanco) {
    if (arquivo=fopen("b.bin", "rb")) { // confere se já existe um arquivo de bancos
        fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraBanco, sizeof(struct Banco), 1, arquivo)) {
            if (leituraBanco.idBanco == idBanco) {
                fclose(arquivo);
                return leituraBanco.nome;
            }
        }
    }
    fclose(arquivo);
    return 0;
}

char* NomeProduto(int idProduto) {
    if (arquivo=fopen("p.bin", "rb")) { // confere se já existe um arquivo de produtos
        fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
        while (fread(&leituraProduto, sizeof(struct Produto), 1, arquivo)) {
            if (leituraProduto.idProduto == idProduto) {
                fclose(arquivo);
                return leituraProduto.nome;
            }
        }
    }
    fclose(arquivo);
    return 0;
}

int Prospecta(float renda, float valor, float entrada, int prazo) {
    int i=0;
    poolFin=NULL;
    float compRenda, aSac, rPrice, emprestimo=valor-entrada, parcela1, parcela2;
    arquivo=fopen("p.bin", "rb");
    fseek(arquivo, sizeof(int), SEEK_SET); // salta o espaço reservado ao contador do ID
    while (fread(&leituraProduto, sizeof(struct Produto), 1,  arquivo)) {
        if (leituraProduto.disponivel && leituraProduto.prazoMax >= prazo && leituraProduto.maxPorcentFinanc >= 1-entrada/valor) {
            aSac=emprestimo/prazo;
            rPrice=(valor-entrada)*pow(1+leituraProduto.taxaEfetivaJuros, prazo)*leituraProduto.taxaEfetivaJuros/(pow(1+leituraProduto.taxaEfetivaJuros, prazo)-1);
            compRenda=leituraProduto.maxPorcentRenda*renda;
            parcela1 = (leituraProduto.sistAmortizacao) ? rPrice : emprestimo*leituraProduto.taxaEfetivaJuros+aSac;
            parcela2 = (leituraProduto.sistAmortizacao) ? rPrice : emprestimo*leituraProduto.taxaEfetivaJuros*(1-((float)prazo-1)/prazo)+aSac;
            if (parcela1 <= compRenda) {
                poolFin=(struct Financiamento*) realloc(poolFin, sizeof(struct Financiamento)*(i+1));
                poolFin[i].idProduto=leituraProduto.idProduto;
                poolFin[i].idBanco=leituraProduto.idBanco;
                poolFin[i].sistAmortizacao=leituraProduto.sistAmortizacao;
                poolFin[i].prazo=prazo;
                poolFin[i].taxaNominalAnual=leituraProduto.taxaEfetivaJuros*12*100;
                poolFin[i].primeiraParcela=parcela1;
                poolFin[i].ultimaParcela=parcela2;
                i++;
            }
        }
    }
    fclose(arquivo);
    return i;
}

char* SistAm(int s) {
    if (s)
        return "PRICE";
    return "SAC";
}