

/*vai se tornar indice.h*/


#define PROX(N) N->prox
#define CHAVE(N)  N->chave
#include <stdio.h>
#include <stdlib.h>
#define NUM 2
/*typedef struct celula {
  int  dado;
  struct celula *seguinte;
}celula;
*/

typedef struct elemento {
  int     chave;
  //ponteiro para bloco FILE *reg
  struct elemento *prox;
}elemento;


void cria_indice (elemento **indice);
int insere_indice (elemento **indice, elemento *a);
void insere_indice_ordenado (elemento **indice, elemento *a);
void imprime_indice (elemento *indice);
void excluir(elemento **indice, int ra);
//elemento* buscar(elemento **indice, int ra);

int main()
{
    elemento *indice;
    elemento *a= (elemento*)malloc(sizeof(elemento));
    elemento *m= (elemento*)malloc(sizeof(elemento));

    a->chave = 50;
    m->chave=85;

    cria_indice(&indice);


   insere_indice(&indice,a);
   insere_indice(&indice, m);
   imprime_indice(indice);




    excluir(&indice,50);

    imprime_indice(indice);

    return 0;
}

//trivial?
void cria_indice (elemento **indice){
    *indice = NULL;
}

void imprime_indice (elemento *indice){
    elemento *aux;
    aux=indice;
    printf("Elementos do indice:\n" );
    while(aux!=NULL){
        printf("%d\n",CHAVE(aux));
        aux = PROX(aux);
    }

}
//Mlehor colocar p receber o indice e o valor?
int insere_indice (elemento **indice, elemento *a){
    if (*indice==NULL) {
        *indice = a;
        (a)->prox = NULL;
        return 1;
    }
    else
    {
        elemento *aux;
        aux=*indice;
        while(aux->prox!=NULL);
        aux->prox = a;
        return 1;

    }
}


void excluir(elemento **indice, int ra){

       elemento *aux;
        aux=*indice;
        if((*indice)->chave==ra){
            elemento *aux2;
            aux2=(*indice)->prox;
            free(*indice);
            *indice=aux2;

            return;


    }
   while((aux->prox!=NULL)&&((aux->prox)->chave!=ra)) aux=aux->prox;

        if(aux->prox==NULL) printf("Nao encontrado\n");
        else
            {
                elemento *aux2;
                aux2=(aux->prox)->prox;
                printf("%p\n", aux2);

                free(aux->prox);
                printf("df\n");
                aux->prox = aux2;


            }
            //return NULL;

}

























