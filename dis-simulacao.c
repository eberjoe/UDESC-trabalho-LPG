#include<stdio.h>

struct Banco {
    int idBanco;
    char nome[100];
};

struct Produto {
    int idProduto;
    int idBanco;
    char nomeProduto[100];
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