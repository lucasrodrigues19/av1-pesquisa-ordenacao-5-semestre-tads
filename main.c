/*
 * main.c
 *
 *  Created on: 17 de jun de 2020
 *      Author: lucas.rodrigues
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct DataPedido {
	int dia;
	int mes;
	int ano;
} DataPedido;

typedef struct Caixinha {
	struct Caixinha *ant;
	char desc_prod[50];
	int quantidade;
	float valor_produto;
	struct Caixinha *prox;
} Caixinha;

typedef struct ListaDuplaEncadeada {
	Caixinha *inicio;
} ListaDuplaEncadeada;

typedef struct nodo {
	struct nodo *esq;
	char nome_cliente[70];
	DataPedido *dataPedido;
	float valor_total;
	ListaDuplaEncadeada *produtos;
	struct nodo *dir;
} tipo_abb;

void criarArvore(tipo_abb **arvore);
void criarLista(ListaDuplaEncadeada *lista);
void inserir(tipo_abb **t, char tNome_cliente[70]);
tipo_abb* pesquisar(tipo_abb **t, char tNome_cliente[70]);
void printArvore(tipo_abb **arv_encontrada);
void emOrdem(tipo_abb **No);
void excluirNo(tipo_abb **arvore, char tNome_cliente[70]); //
tipo_abb* menor_valor(tipo_abb *t); //
void inserirProdutos(ListaDuplaEncadeada *produtos, char tDesc[50], int TQtd,float tValor_prod); //
void exibirNaOrdemProdutos(ListaDuplaEncadeada *produtos);
ListaDuplaEncadeada* inserirDadosProduto();
DataPedido* inserirDadosData();
void inserirDataPedido(DataPedido **tDataPedido, int tDia, int tMes, int tAno);
void exibirDataPedido(DataPedido **tDataPedido);
float valorTotalProdutos(ListaDuplaEncadeada *produtos);
void exibirDataValorPedido(tipo_abb **arv_encontrada);
void exibirPedidosComValorAcima(tipo_abb **arv_encontrada, float valorAcima);


int main() {
	tipo_abb *arv_encontrada;
	tipo_abb *arvore;
	char tNome_cliente[70], tDesc_prod;
	int i;
	float valorAcima;
	criarArvore(&arvore);

	for (i = 0; i < 3; i++) {
		printf("\n---DADOS DO CLIENTE ----\n");
		strcpy(tNome_cliente, "");
		printf("\nNome do cliente: ");
		gets(tNome_cliente);
		if (strcmp(tNome_cliente, "") == 0)
			gets(tNome_cliente);

		inserir(&arvore, tNome_cliente);
	}
	printf("\n");
	printf("\n ENCONTRE UM CLIENTE \n");
	strcpy(tNome_cliente, "");
	printf("\nNome: ");
	gets(tNome_cliente);
	if (strcmp(tNome_cliente, "") == 0)
		gets(tNome_cliente);
	arv_encontrada = pesquisar(&arvore, tNome_cliente);
	if (arv_encontrada != NULL)
		exibirDataValorPedido(&arv_encontrada);

	printf("\n");
	printf("\nREMOVA UM PEDIDO\n");
	strcpy(tNome_cliente, "");
	printf("\nNome do cliente: ");
	gets(tNome_cliente);
	if (strcmp(tNome_cliente, "") == 0)
		gets(tNome_cliente);

	excluirNo(&arvore, tNome_cliente);


	printf("\n\n--------Em ordem por nome do cliente: ------------\n\n");
	emOrdem(&arvore);

	printf("\n\nExibir Pedidos acima de: ");
		scanf("%f",&valorAcima);
		printf("\n------PEDIDOS ACIMA DE %.2f------\n", valorAcima);
		exibirPedidosComValorAcima(&arvore,valorAcima);

	return 0;
}
void criarArvore(tipo_abb **arvore) {
	*arvore = NULL;
}
void criarLista(ListaDuplaEncadeada *lista) {
	lista->inicio = NULL;
}
void inserir(tipo_abb **t, char tNome_cliente[70]) {
	if (*t == NULL) {
		*t = (tipo_abb*) malloc(sizeof(tipo_abb));
		(*t)->produtos = inserirDadosProduto();
		(*t)->dataPedido = inserirDadosData();
		strcpy((*t)->nome_cliente, tNome_cliente);
		(*t)->valor_total = valorTotalProdutos(&(*t)->produtos);
		(*t)->esq = NULL;
		(*t)->dir = NULL;
	} else if (strcmp(tNome_cliente, (*t)->nome_cliente) > 0)
		inserir(&((*t)->dir), tNome_cliente);
	else
		inserir(&((*t)->esq), tNome_cliente);
}
tipo_abb* pesquisar(tipo_abb **t, char tNome_cliente[70]) {
	if (t != NULL) {
		if (strcmp((*t)->nome_cliente, tNome_cliente) == 0)
			return *t;
		else if (strcmp(tNome_cliente, (*t)->nome_cliente) > 0)
			pesquisar(&(*t)->dir, tNome_cliente);
		else if (strcmp(tNome_cliente, (*t)->nome_cliente) < 0)
			pesquisar(&(*t)->esq, tNome_cliente);
		else
			return NULL;
	} else
		return NULL;
}
void printArvore(tipo_abb **arv_encontrada) {
	if (*arv_encontrada != NULL) {
		printf("\nNome: %s", (*arv_encontrada)->nome_cliente);
		printf("\nValor total: %.2f", (*arv_encontrada)->valor_total);
		exibirNaOrdemProdutos(&(*arv_encontrada)->produtos);
	}
}
void emOrdem(tipo_abb **No) {
	if (*No != NULL) {
		emOrdem(&((*No)->esq));
		printf("\n---------CLIENTE-----------\n");
		printf("\nNome do cliente: ");
		puts((*No)->nome_cliente);
		printf("Data do pedido: ");
		exibirDataPedido(&((*No)->dataPedido));
		printf("\n\n");
		exibirNaOrdemProdutos(&(*No)->produtos);
		printf("\n");
		printf("Valor total do Pedido: ");
		printf("%.2f", (*No)->valor_total);
		printf("\n");
		emOrdem(&(*No)->dir);

	}
}
void excluirNo(tipo_abb **arvore, char tNome_cliente[70]) { //
	tipo_abb *aux = (tipo_abb*) malloc(sizeof(tipo_abb)); // variável para auxiliar a busca pelo menor valor da subárvore direita
	if (strcmp((*arvore)->nome_cliente, "") == 0)
		return;

	if (*arvore != NULL) {
		if (strcmp(tNome_cliente, (*arvore)->nome_cliente) > 0)
			excluirNo(&(*arvore)->dir, tNome_cliente);
		else if (strcmp(tNome_cliente, (*arvore)->nome_cliente) < 0)
			excluirNo(&(*arvore)->esq, tNome_cliente);
		else if (((*arvore)->esq != NULL) && ((*arvore)->dir != NULL)) {
			aux = menor_valor((*arvore)->dir); // variavel aux recebe o endereço do menor valor na subárvore direita
			strcpy((*arvore)->nome_cliente, (aux->nome_cliente)); // o nó a ser retirado recebe o valor do nó que contem o menor valor da subárvore direita
			(*arvore)->valor_total = (aux->valor_total);
			(*arvore)->produtos = (aux->produtos);
			(*arvore)->dataPedido = (aux->dataPedido);
			excluirNo(&(*arvore)->dir, (*arvore)->nome_cliente);
		} else { // se o nó só tem um filho ou nenhum
			aux = *arvore; // variável aux aponta para o nó a ser retirado
			if ((*arvore)->esq == NULL) // se o nó não tem filho esquerdo
				*arvore = (*arvore)->dir; // o filho direito assume o lugar do pai
			else
				// se o nó nó tem filho direito
				*arvore = (*arvore)->esq; // o filho esquerdo assume o lugar do pai

			free(aux->dataPedido);
			free(aux->produtos);
			free(aux); // libera o nó desejado
			printf("Valor retirado da arvore.");
		}
	} else
		printf("Não existe esse valor na arvore");

}

tipo_abb* menor_valor(tipo_abb *t) { // t aponta para o nó raiz da árvore ou raiz da subárvore
	if (t == NULL) // se a árvore estiver vazia
		return NULL;
	else if (t->esq == NULL) // se o nó não tem filho esquerdo, este é o menor valor da subárvore direita
		return t; // retorna o endereço do nó que contem o menor valor da subárvore direita
	else
		// se o nó tem filho esquerdo
		return menor_valor(t->esq); // chamada recursiva para continuar a procura do menor valor
}

void inserirProdutos(ListaDuplaEncadeada *produtos, char tDesc[50], int TQtd,
		float tValor_prod) { //
	Caixinha *nova;
	nova = (Caixinha*) malloc(sizeof(Caixinha));
	strcpy(nova->desc_prod, tDesc);
	nova->valor_produto = tValor_prod;
	nova->quantidade = TQtd;

	if (produtos->inicio == NULL) {
		nova->ant = nova;
		nova->prox = nova;
		produtos->inicio = nova;
	} else {
		Caixinha *aux;
		aux = produtos->inicio; //aponta para o inicio da lista
		while (aux->prox != produtos->inicio) //pecorre ate achar o ultimo elemento
			aux = aux->prox;

		nova->ant = aux; //aux agora sera o anterior
		nova->prox = produtos->inicio; //o proximo do ultimo elemento sera inicio
		aux->prox = nova; //o ultimo elemento agora sera nova
		produtos->inicio->ant = nova; //caso não esteja vazia o anterior do primeiro elemento, aponta para o ultimo
	}
}
void exibirNaOrdemProdutos(ListaDuplaEncadeada *produtos) { //
	Caixinha *aux;
	aux = produtos->inicio; //aponta para o inicio da lista
	if (aux != NULL) {
		do {
			if (aux->desc_prod != NULL)
				printf("Descrição do produto:  %s , ", aux->desc_prod);
			if (aux->quantidade != NULL)
				printf("Quantidade: %d, ", aux->quantidade);
			if (aux->valor_produto != 0)
				printf("Valor do produto:  %.2f\n", aux->valor_produto);
			aux = aux->prox;
		} while (aux != produtos->inicio);
	}
}
ListaDuplaEncadeada* inserirDadosProduto() { //
	int j, rsp, tQtd, maxP;
	float tValor_Prod;
	char tDesc[50];
	ListaDuplaEncadeada *lista_dep;
	criarLista(&lista_dep);
	printf("Quantos produtos? ");
	scanf("%d", &maxP);
	for (j = 0; j < maxP; j++) {
		printf("\n----DADOS DO PRODUTO-----\n");
		strcpy(tDesc, "");
		printf("\nDescricao do produto: ");
		gets(tDesc);
		if (strcmp(tDesc, "") == 0)
			gets(tDesc);

		printf("Valor do produto: ");
		scanf("%f", &tValor_Prod);

		printf("Quantidade: ");
		scanf("%d", &tQtd);

		inserirProdutos(&lista_dep, tDesc, tQtd, tValor_Prod);
	}

	return lista_dep;
}
DataPedido* inserirDadosData() { //
	int tDia, tMes, tAno;
	DataPedido *tDataPedido;
	printf("\n-----DATA DO PEDIDO-----\n");
	printf("\nDia: ");
	scanf("%d", &tDia);
	printf("Mes: ");
	scanf("%d", &tMes);
	printf("Ano: ");
	scanf("%d", &tAno);
	inserirDataPedido(&tDataPedido, tDia, tMes, tAno);
	return tDataPedido;
}
void inserirDataPedido(DataPedido **tDataPedido, int tDia, int tMes, int tAno) { //
	*tDataPedido = (DataPedido*) malloc(sizeof(DataPedido));
	(*tDataPedido)->dia = tDia;
	(*tDataPedido)->mes = tMes;
	(*tDataPedido)->ano = tAno;
}
void exibirDataPedido(DataPedido **tDataPedido) { //
	if ((*tDataPedido)->dia != NULL)
		printf("%d/", (*tDataPedido)->dia);
	if ((*tDataPedido)->mes != NULL)
		printf("%d", (*tDataPedido)->mes);
	if ((*tDataPedido)->ano != NULL)
		printf("/%d", (*tDataPedido)->ano);
}
float valorTotalProdutos(ListaDuplaEncadeada *produtos) { //
	Caixinha *aux;
	aux = produtos->inicio; //aponta para o inicio da lista
	float soma = 0;
	float valorP;
	int qtd;

	if (aux != NULL) {

		do {
			valorP = 0;
			valorP += aux->valor_produto;
			valorP *= aux->quantidade;
			soma += valorP;
			aux = aux->prox;
		} while (aux != produtos->inicio);
	}
	return soma;
}
void exibirDataValorPedido(tipo_abb **arv_encontrada) {
	printf("\n------DADOS DO PEDIDO------\n");
	printf("Data do pedido: ");
	exibirDataPedido(&(*arv_encontrada)->dataPedido);
	printf("\nValor total do Pedido: ");
	printf("%.2f", (*arv_encontrada)->valor_total);
	printf("\n");
	exibirNaOrdemProdutos(&(*arv_encontrada)->produtos);
}
void exibirPedidosComValorAcima(tipo_abb **arv_encontrada, float valorAcima) {
	if (*arv_encontrada != NULL) {
		exibirPedidosComValorAcima(&((*arv_encontrada)->esq),valorAcima);
		if ((*arv_encontrada)->valor_total > valorAcima) {
			printf("\n--------------\n");
			printf("Data do pedido: ");
			exibirDataPedido(&(*arv_encontrada)->dataPedido);
			printf("\nValor total do Pedido: ");
			printf("%.2f", (*arv_encontrada)->valor_total);
			printf("\n");
			exibirNaOrdemProdutos(&(*arv_encontrada)->produtos);
		}
		exibirPedidosComValorAcima(&(*arv_encontrada)->dir,valorAcima);
	}
}
