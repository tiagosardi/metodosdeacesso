

/*vai se tornar indice.h*/


//#define POST(T) T->prox
//#define CHAVE(T)  T->chave
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
void insere_indice (elemento **indice, elemento *a);
void insere_indice_ordenado (elemento **indice, elemento *a);
void imprime_indice (elemento *indice);
void excluir(elemento **indice, int ra);
//elemento* buscar(elemento **indice, int ra);

int main()
{
    elemento *a= (elemento*)malloc(sizeof(elemento));
    elemento *indice; //=  (elemento*)malloc(sizeof(elemento));
    elemento *m= (elemento*)malloc(sizeof(elemento));
    //free(j);
    //b->chave = 14;
    printf("sdsdas\n");
    a->chave = 50;
    //elemento m;
    m->chave=85;
    //printf("%d\n",b->chave);

    cria_indice(&indice);
   printf("%p\n", indice);

   insere_indice(&indice,a);
   insere_indice(&indice, m);
    printf("2 - %p\n", indice);
    printf("/--------------------------/\n");
    imprime_indice(indice);



    printf("----------------------\n");
   // ufoi(4717,414);
    excluir(&indice,50);
   // free(&m);
    imprime_indice(indice);

    return 0;
}

void cria_indice (elemento **indice){
    *indice = NULL;
}

void imprime_indice (elemento *indice){
    elemento *aux;
    aux=indice;
    printf("Elementos do indice:\n" );
    while(aux!=NULL){

        printf("%d\n",aux->chave);
        aux = aux->prox;
    }

}
void insere_indice (elemento **indice, elemento *a){
    if (*indice==NULL) {
        *indice = a;
        (a)->prox = NULL;
    }
    else
    {
        elemento *aux;
        aux=*indice;
        while(aux->prox!=NULL);
        aux->prox = a;

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

























