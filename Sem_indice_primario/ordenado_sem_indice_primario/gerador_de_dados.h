/**
Criação de extraiNome e extraiSobrenome;

novas funções: geraNome e geraSobrenome
O aleatorio agora faz para nome e sobrenome;
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX 200 // EDITE AQUI ***************************

int* extraiRA();
char* extraiNome();
char* extraiSobrenome();

int* criaVetor1(){
  int i;

  int *vetor1=(int*)malloc((2*TAM_MAX)*sizeof(int));
  //int vetor1[2*TAM_MAX];  //vai pegar de uma fonte com mais valores

  for(i=0; i<(2*TAM_MAX) ; i++){
    vetor1[i] = i+1;
  }

  return vetor1;

}

//Cria um RA. É usado na função adicionarAluno, para gerar um RA distinto.
int criaRA(){	//consertar... nao ta saindo aleatorio

  int i;
  int* vetor2 = (int*)malloc(TAM_MAX *sizeof(int*)); 
  vetor2= extraiRA();
  

  srand( (unsigned)time(NULL) );
  int novoRA = 1 + ( rand() % TAM_MAX);
  //printf("\ncomecando com: %d", novoRA);
  for(i=0 ; i<TAM_MAX ; i++){
    if(novoRA == vetor2[i]){
      novoRA= 1 + ( rand() % 2*TAM_MAX);
      i=-1;
      //printf("\nRA atual: %d", novoRA);
     
    }
  }
  return novoRA;
}

char* extraiNome(){
	FILE *arq;
	arq = fopen ("dados.txt", "r");

	char linha[200], *ra_aux;
	char* nome;

	int ra, i=0;

	//char* nome=(char*)malloc(TAM_MAX *(sizeof(char*)));

	while((i<TAM_MAX)&&(fgets(linha, sizeof(linha), arq))!=NULL ){
		ra_aux = (char*)strtok(linha, "&");
		nome = (char*)strtok(NULL, "&");
		//printf("\n%s", nome);
	}

	return nome;
}

//Pega os RAs do arquivo e coloca num vetor
int* extraiRA(){	

  FILE *arq;
  arq = fopen("dados.txt", "r");

  char linha[200], *ra_aux;
  int ra, i=0;
  int* vetor2=(int*)malloc(TAM_MAX *sizeof(int*));
  

  while((i<TAM_MAX)&&(fgets(linha, sizeof(linha), arq))!=NULL ){
    ra_aux = (char*)strtok(linha, "&");
    vetor2[i] = atoi(ra_aux);
    i++;            
  }

  /*for(i=0; i<TAM_MAX ; i++){
    printf("\nmeus RAs lindos caramba: %lu", sizeof(vetor2[i]));
  }*/

  return vetor2;
}

char* geraNomes(){

	char nomes[][10]={"Mateus", "Marcos", "Maria","Lucas", "Joao", "Felipe", "Elias", "Zacarias", "Timoteo", "Leandro", "Ester", "Astrovaldo", "Fernando", "Guilherme", "Tyson", "James", "Gabriel", "Leticia", "Jesse", "Hamurabi", "Taylor", "Amanda", "Luana", "Beatriz", "Elber", "Tamires", "Leonardo", "Camila", "Victoria", "Vandiscley", "Daniel", "Katherine" , "Vanusa", "Danilo", "Davi", "Mariana", "Paula", "Heloisa"};
	int i;
	char *nomes2=(char*)malloc(TAM_MAX * sizeof(char*));

	int posicao= 0 + (rand()% 38);
	strcpy(nomes2 , nomes[posicao]);

	return nomes2;
}

char* geraSobrenomes(){

	char sobrenomes[][10]={"Souza", "Tavares", "Buarque", "Mendonca", "Herzog", "Teixeira", "Brumatti", "Duarte", "Silva", "Alves", "Oliveira", "Favalessa", "Gomes", "Novaes", "Marques", "Cortez", "Goncalves", "Lopes"};
	int i;
	char *sobrenomes2=(char*)malloc(TAM_MAX * sizeof(char*));

	int posicao= 0 + (rand()% 18);
	strcpy(sobrenomes2 , sobrenomes[posicao]);

	return sobrenomes2;
}

float geraNotas(){
	float nota;
	int i;
	//float x;
	//srand ((unsigned)time(NULL));
	
	nota= rand() % 1000;
	nota=nota/100;
	//printf("\n\nnota: %.2f\n\n", nota);
	return nota;
}

int* geraRA(int *vetor1){

  int* vetor2=(int*)malloc(TAM_MAX * sizeof(int));
  
  int j=0;
  int posicao;

  int i;
  

  srand( (unsigned)time(NULL) );
  while(j<TAM_MAX){

    posicao= 0 + ( rand() % TAM_MAX);

  


    if(vetor1[posicao]!=-1){
      vetor2[j]=vetor1[posicao];
      vetor1[posicao]=-1;
      j++;
    }
  }
  return vetor2;
}

void geraArquivo(){

  time_t inicio,fim;
  inicio=time(NULL);

  FILE *arq;
  arq = fopen("dados.txt", "w");

   //mudar pra lista depois
  int i;

  int* vetor1=criaVetor1();

  int* vetor2=geraRA(vetor1);
  char* nome= geraNomes();

  //geraNomes();

  //printf("");
  srand( (unsigned)time(NULL) );
  for(i=0 ; i<TAM_MAX ; i++){ 
    fprintf(arq, "%d&%s&%s&%.2f&%.2f&%.2f\n",vetor2[i], geraNomes(), geraSobrenomes(), geraNotas(), geraNotas(), geraNotas());
    //printf("%d&%s&sobrenome&10&08&5\n",vetor2[i], geraNomes());
  }

  fim = time(NULL);
  //printf("\nTempo de execucao: %.0f segundos\n", difftime(fim, inicio));
  printf("\nArquivo gerado com sucesso!\n");


  fclose(arq);
}
