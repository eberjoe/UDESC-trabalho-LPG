#include<stdio.h>
#define MAXNOME 100

struct Banco {
    int idBanco;
    char nome[MAXNOME];
};

struct Produto {
    int idProduto;
    int idBanco;
    char nomeProduto[MAXNOME];
    int disponivel;
    char sistAmortizacao;
    float maxPorcentFinanc;
    float taxaEfetivaJuros;
    int prazoMax;
    float maxPorcentRenda;
};

int main() {
    return 0;
}