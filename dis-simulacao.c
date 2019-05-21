#include<stdio.h>
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
    return 0;
}