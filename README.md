# Requisitos
Faça seu próprio sistema de controle de dados que resolva algum problema real, empresarial ou pessoal.

Cronograma:

* [x] Definição da Equipe (duplas): até dia 20/05 (confirme a equipe até este dia usando o e-mail rafaelberri@gmail.com e no assunto informe “LPG – TF Equipe”).
* [ ] Definição do problema a ser tratado: até dia 27/05. Envie um texto descrevendo o problema que o sistema final poderia ajudar. Envie a definição principal dos dados (structs) que será utilizado na solução escolhida. No mínimo, o programa deverá trabalhar com 10 campos. (Envie até este dia usando o e-mail rafaelberri@gmail.com e o assunto “LPG – TF Dados”).
* [ ] Apresentação do sistema será dia 17 ou 24/06. Todos os integrantes deverão falar. Deverá ser apresentado o sistema/problema. Duração máxima de cada apresentação serão 12 minutos e 3 minutos para perguntas. A equipe deverá se atentar em mostrar, principalmente, os pontos de avaliação abaixo.
* [ ] Envio do código fonte (24/06). Enviar para rafaelberri@gmail.com um arquivo compactado contendo o código fonte do programa, a apresentação final, e um texto descritivo do sistema/problema atendido (com no máximo 2 páginas de texto).

## Critérios de Pontuação
|Descrição|Peso|
|-|-|
|Definição da estrutura de dados|1,0|
|Operação de cadastro/inserção de dados (validação da entrada de dados)|1,0|
|Operação de consulta de dados (possibilitar a consulta utilizando, pelo menos, dois campos)|1,0|
|Operação de exclusão de dados (deixe claro a estratégia adotada)|1,0|
|Inovação (utilização de técnicas, bibliotecas e soluções não trabalhadas na disciplina)|2,0|
|Telas do programa (boa aparência e praticidade)|1,0|
|Texto descritivo|1,0|
|Apresentação e organização|1,0|
|Presença (durante o mês de junho e principalmente na apresentação final)|1,0|

# Sistema de Cadastro de Instituições Financeiras e Simulação de Financiamento

## Introdução
O sistema permitirá o cadastro, a consulta e a remoção de dados de instituições financeiras com suas respectivas condições para financiamento. De posse de tais dados, o sistema permitirá ao usuário efetuar simulações de financiamento, apresentando na tela a progressão da amortização.

## Dados para Cadastro
O cadastro de instituições financeiras requererá as seguintes estruturas de dados:
``` C
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
```

## Dados para a Simulação
A simulação de financiamento requererá os seguintes dados do contraente:
* Renda - **float**
* Valor total do bem a ser adquirido - **float**
* Valor disponível para entrada - **float**

O sistema deverá retornar as opções disponíveis de acordo com a base de dados, ou uma mensagem indicando que não há opções para os valores fornecidos. 

## Expectativa
O sistema não se limitará à curadoria de dados, mas também será capaz de converter os dados armazenados em informação útil.

## Convenção para Nomes
Os nomes das entidades seguirão o seguinte padrão:

|Tipo|Formato|Exemplo|
|-|-|-|
|constantes|upper case|MAX|
|campos e variáveis|camel case|idProduto|
|structs e funções -- exceto **```main()```**|pascal case|ByPassModel|
|bibliotecas e arquivos de apoio -- exceto **```README.md```**|lower case|mathfunctions|

## Remoção de Registros
A remoção de registros de instituições e de produtos se dará através do zeramento do campo **```disponivel```**. Ao efetuar a inserção de um novo registro, o sistema deve primeiro procurar por registros com o campo **```disponivel```** zerado para efetuar a reescrita do mesmo, e somente adicionar novos registros ao fim do arquivo nos casos em que não houver registros com o campo **```disponivel```** zerado.

Os registros com o campo **```disponivel```** zerado não serão visíveis ao usuário seja para consulta ou para edição.
