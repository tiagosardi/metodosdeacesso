

/*vai se tornar indice.h*/


#define PROX(N) N->prox
#define CHAVE(N)  N->chave
#include <stdio.h>
#include <stdlib.h>
#define NUM 2

typedef struct elemento {
  int     chave;
  //ponteiro para bloco FILE *reg
  short int marcador=0;	
  struct elemento *prox; //ponteiro de registro no arquivo?
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

    CHAVE(a) = 50;
    CHAVE(m)=85;

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
//Melhor colocar p receber o indice e o valor?
//Fazer inserção no final do arquivo e reodernar depois
int insere_indice (elemento **indice, elemento *a){
    if (*indice==NULL) {
        *indice = a;
        PROX(a) = NULL;
        return 1;
    }
    else
    {
        elemento *aux;
        aux=*indice;
        while(PROX(aux)!=NULL);
        PROX(aux) = a;
        return 1;

    }
}

//No arquivo nao vai ser assim
void excluir(elemento **indice, int ra){

       elemento *aux;
        aux=*indice;
        if(CHAVE((*indice))==ra){
            elemento *aux2;
            aux2=PROX((*indice));
            free(*indice);
            *indice=aux2;

            return;


    }
   while((PROX(aux)!=NULL)&&(CHAVE(PROX(aux))!=ra)) aux=PROX(aux);

        if(PROX(aux)==NULL) printf("Nao encontrado\n");
        else
            {
                elemento *aux2;
                aux2=PROX(PROX(aux));
                printf("%p\n", aux2);

                free(PROX(aux));
                printf("df\n");
                PROX(aux)= aux2;


            }
            //return NULL;

}


/*Exclusão lógica*/
//Trás blocos até encontrar e muda o marcador

/*Exclusão fisica*/
//Compactar tirando os que estão marcados
//No processo de ordenação?

//ordena tudo
//busca
//busca_binaria













