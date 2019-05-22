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
O cadastro de uma instituição financeira requererá as seguintes estruturas de dados:
* Instituição financeira - **struct**
  * ID - **int**
  * Disponível (sim/não) - **int**
  * Nome da instituição - **char**
* Produto (pelo menos um por instituição) - **struct** 
  * ID - **int**
  * Disponível (sim/não) - **int**
  * ID da instituição - **int**
  * Nome do produto - **char**
  * Sistema de amortização (SAC ou PRICE) - **char**
  * Porcentagem máxima de financiamento - **float**
  * Taxa efetiva de juros - **float**
  * Prazo máximo em meses - **int**
  * Porcentagem máxima de comprometimento da renda - **float**

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
|structs e funções -- exceto **main()**|pascal case|ByPassModel|
|bibliotecas e arquivos de apoio -- exceto **README.md**|lower case|mathfunctions|
