/*
ATUALIZAÇÃO: (Commit)

Antigo:
1. A funcao adicionarAluno possui adição de RA aleatório (funcional mal)

Novo:
1.

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gerador_de_dados.h"

#define BSIZE 100

typedef struct _aluno {
  int RA;
  char primeiro_nome[20];
  char ultimo_nome[20];
  float nota[3];
} Aluno;

void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2);
int buscarAluno(char *identificador , char *chave);
Aluno* recupera_registro(int deslocamento);
void remove_logic(int ra);
void imprime_dados();
void reorganizacao_dados();
void tempoAdicionarAluno();
void tempoBuscarAluno();
void tempoRemoverAluno();
void le_script();
/*Variaveis globais*/
 int   pos1,pos2,r;
  FILE *fp;
   char buffer[100];

int main()
{


   // printf("%d",n);

	clock_t t0, tf;
  	double tempo_gasto;
  	t0 = clock();


      int   comp_reg;
      int num_reg= TAM_MAX ;/*editar na biblioteca*/
      Aluno a[num_reg], b; //cria o vetor do tamanho de numero de registros


	char linha[200];//Escolher tamanho adequado p linha /PS: nao funciona com ponteiro pois na linha 50 usa-se sizeof(linha)
	char *nome, *ult_nome, *nota1, *nota2, *nota3, *ra_aux;
	FILE *arq;

	int num=0;

    geraArquivo();

      /*Captura dados no txt*/
	arq = fopen("dados.txt", "r");

	if(arq == NULL)
			printf("Erro, nao foi possivel abrir o arquivo\n");
	else
		while((num<num_reg)&&(fgets(linha, sizeof(linha), arq))!=NULL )
        {

            /*Separa dados do txt pelo separador &*/
            ra_aux = (char*)strtok(linha, "&");

            nome = (char*)strtok(NULL, "&");

            ult_nome=(char*)strtok(NULL, "&");
            nota1 =(char*)strtok(NULL, "&");

            nota2 =(char*)strtok(NULL, "&");
            nota3 =(char*)strtok(NULL, "&");
            //printf("%s\n%s\n%s\n%s\n%s", nome, ult_nome, nota1, nota2, nota3);

            /*Converte e copia dados para o vetor de alunos*/
            a[num].RA = atoi(ra_aux);
            strcpy(a[num].primeiro_nome,nome);
            strcpy(a[num].ultimo_nome,ult_nome);
            a[num].nota[0]=atof(nota1);
            a[num].nota[1]=atof(nota2);
            a[num].nota[2]=atof(nota3); //Converte notas de string para float

            //printf("\n--\n%s\n%s\n%.2f\n%.2f\n%.2f", a[num].primeiro_nome, a[num].ultimo_nome, a[num].nota[0], a[num].nota[1], a[num].nota[2]);

            num++; //Atualiza contador
        }

	fclose(arq);

/***************************************************************************/

 /*Criação do arquivo binario*/

  fp = fopen("dados.bin","wb");

  pos1 = 0; //Numero de bytes ocupados no buffer

  memcpy(&(buffer[pos1]),&num_reg,sizeof(int)); //Copia numero de registros para primeira posição do buffer
  //printf("num_reg %d %s\n",num_reg,&(buffer[pos1])); //buffer[pos] varia pq???
  pos1 += sizeof(int); //esta sendo ocuoado o numero de bytes de um inteiro no buffer

  for (r=0; r < num_reg; r++) {
    //printf("num_reg %d %d\n",num_reg,(buffer[pos1]));
    comp_reg =  //calcula quantos bytes serao ocupados no buffer
      sizeof(int) + // ra
      strlen(a[r].primeiro_nome)*(sizeof(char)) + // nome1
      1*sizeof(char) + // #
      strlen(a[r].ultimo_nome)*(sizeof(char)) + // nome2
      1*sizeof(char) + // #
      sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3
   // printf("--NOVO REG: %d\n",comp_reg);

/* Montando buffer*/

    if (pos1+sizeof(int)>=BSIZE) {         //se a escrita de um int ultrapassar o tamanho do buffer.
      fwrite(buffer,sizeof(char),pos1,fp); //escreve o buffer em memoria e
      pos1 = 0;                            //esvazia buffer
    }
    memcpy(&(buffer[pos1]),&comp_reg,sizeof(int)); //escreve quantos bytes o registro ocupa no buffer
    //printf("comp_reg %s\n",&(buffer[pos1]));
    pos1+=sizeof(int);                             //atualiza numero de bytes ocupados no buffer

    // ra
    if (pos1+sizeof(int)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].RA,sizeof(int));
    //printf("ra %s\n",&(buffer[pos1]));
    pos1 += sizeof(int);

    // nome1 + #
    if (pos1+sizeof(char)*(strlen(a[r].primeiro_nome)+2)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),a[r].primeiro_nome,sizeof(char)*strlen(a[r].primeiro_nome));
    pos1 += sizeof(char)*strlen(a[r].primeiro_nome);
    buffer[pos1] = '#';
    //printf("nome1 %s\n",&(buffer[pos1-sizeof(char)*strlen(a[r].primeiro_nome)]));
    pos1 += 1*sizeof(char);

    // nome2 + #
    if (pos1+sizeof(char)*(strlen(a[r].ultimo_nome)+2)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),a[r].ultimo_nome,sizeof(char)*strlen(a[r].ultimo_nome));
    pos1 += sizeof(char)*strlen(a[r].ultimo_nome);
    buffer[pos1] = '#';
    //printf("nome2 %s\n",&(buffer[pos1-sizeof(char)*strlen(a[r].ultimo_nome)]));
    pos1 += 1*sizeof(char);

    // nota1
    if (pos1+sizeof(float)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].nota[0],sizeof(float));
    //printf("nota1 %s\n",&(buffer[pos1]));
    pos1 += sizeof(float);

    // nota2
    if (pos1+sizeof(float)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].nota[1],sizeof(float));
    //printf("nota2 %s\n",&(buffer[pos1]));
    pos1 += sizeof(float);

    // nota3
    if (pos1+sizeof(float)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].nota[2],sizeof(float));
    //printf("nota3 %s\n",&(buffer[pos1]));
    pos1 += sizeof(float);

  } // end for

  if (pos1>0) {
    //printf("final %s %d %d\n",buffer,strlen(buffer),pos1);
    fwrite(buffer,sizeof(char),pos1,fp);
  }

  fclose(fp);

/*************************************************************************/

/*
float no[3]={10,17,20};
inserir(07, "leandro", "nogueira", no);

num_reg++;

*/

/* Simulando a leitura do arquivo*/



  fp = fopen("dados.bin","r+");

  pos1 = 0;
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("---->num_reg %d\n",num_reg);


	char* nomesAqui = geraNomes();
  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));
    //printf("NOVO REG: %d\n",comp_reg);

    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);
    }

    /* copia demais campos do registro*/

    // ra
    memcpy(&b.RA,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);

    // nome1
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.primeiro_nome,&(buffer[pos1]),pos2-pos1);
    b.primeiro_nome[pos2-pos1] = '\0';
    //printf("nome1 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.primeiro_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);

    // nome2
    pos1 = pos2 + sizeof(char);
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.ultimo_nome,&(buffer[pos1]),pos2-pos1);
    b.ultimo_nome[pos2-pos1] = '\0';
    //printf("nome2 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.ultimo_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);
    pos1 = pos2 + sizeof(char);

    // nota1
    memcpy(&b.nota[0],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[0]);

    // nota2
    memcpy(&b.nota[1],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[1]);

    // nota3
    memcpy(&b.nota[2],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[2]);

    /* imprime o registro lido*/
    //printf("%d %6d %s %s %.2f %.2f %.2f\n",
//	   comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
  }

  fclose(fp);


/***********************************************************************************************************/

	
//  tempoAdicionarAluno();
//  tempoBuscarAluno();
//  tempoRemoverAluno();

//  adicionarAluno("Nome", "Sobrenome", 10, 9.5, 8.73);
//  buscarAluno("RA", "4");
//  remove_logic(2);
//  reorganizacao_dados();
//  le_script();
//  imprime_dados();

//	tf = clock();
//	tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
//	printf("Tempo gasto: %lf s\n", tempo_gasto);


  	return(0);
    //FIM DA MAIN
}

void tempoAdicionarAluno(){

	
	clock_t t0, tf;
  	double tempo_gasto, somatorio;
  	//t0 = clock();
  	int i;
  	for(i = 0 ; i<50 ; i ++){

  		t0 = clock();
  		adicionarAluno(geraNomes(), geraSobrenomes(), geraNotas(), geraNotas(), geraNotas());	
  		tf = clock();
		tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
		somatorio += tempo_gasto;
		//printf("%lf\n", tempo_gasto);
  	}
  printf("%.7lf\n", (somatorio/i)*100);

	


}

void tempoBuscarAluno(){

		
	clock_t t0, tf;
  	double tempo_gasto, somatorio;
  	//t0 = clock();
  	int i;
  	char i_char[10];
  	for(i = 0 ; i<50 ; i ++){
  		sprintf(i_char , "%d" , i+1);
  		t0 = clock();
  		buscarAluno("RA", i_char);	
  		

  		tf = clock();
		tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
		somatorio += tempo_gasto;
		//printf("%lf\n", tempo_gasto);
  	}

	printf("%.7lf\n", (somatorio/i)*100);

}

void tempoRemoverAluno(){

	clock_t t0, tf;
  	double tempo_gasto, somatorio;
  	//t0 = clock();
  	int i, j;
  	char i_char[10];
  		
  		
	  	for(i = 0 ; i<10 ; i ++){
	  		sprintf(i_char , "%d" , i+1);
  			t0 = clock();
  			remove_logic(i+1);
	  		
  			tf = clock();
			tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
      //printf("\ntempo gasto: %.7f\n", tempo_gasto);
			somatorio += tempo_gasto;
			//printf("%lf\n", tempo_gasto);
  		}

  	//printf("\ni: %d\n", i);
	printf("deu %.7lf\n", (somatorio/i)*100);
}

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
int buscarAluno(char *identificador , char *chave){ 
/*-----------------------------------------------------------------------------*/
	int comp_reg, comp_reg_total = 0;
	//chave=(char*)malloc(sizeof(char));
	Aluno b;
  int num_reg = TAM_MAX;
  char charRA[20], charNota1[5], charNota2[5], charNota3[5], nome_e_sobrenome[50];
   /* Simulando a leitura do arquivo */

  fp = fopen("dados.bin","r+");

  pos1 = 0;

  
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer */
  

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("\nnum_reg:: %d\n",num_reg);
  //printf("\npos1: %d", pos1);	// 4
  //printf("\nbuffer[pos1]: %c", buffer[pos1]); // 31

  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));
//    printf("\nNOVO REG: %d\n",comp_reg);
    comp_reg_total += comp_reg + sizeof(int);
//    printf("\ncomp_reg_total: %d", comp_reg_total);
    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);
    }

    /* copia demais campos do registro */

    // ra
    memcpy(&b.RA,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);

    // nome1
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.primeiro_nome,&(buffer[pos1]),pos2-pos1);
    b.primeiro_nome[pos2-pos1] = '\0';
    //printf("nome1 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.primeiro_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);

    // nome2
    pos1 = pos2 + sizeof(char);
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.ultimo_nome,&(buffer[pos1]),pos2-pos1);
    b.ultimo_nome[pos2-pos1] = '\0';
    //printf("nome2 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.ultimo_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);
    pos1 = pos2 + sizeof(char);

    // nota1
    memcpy(&b.nota[0],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[0]);

    // nota2
    memcpy(&b.nota[1],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[1]);

    // nota3
    memcpy(&b.nota[2],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[2]);

    /* imprime o registro lido */
    
//    printf("%d %6d %s %s %.2f %.2f %.2f\n",
//              comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);

    //busca por ra
	if ( (!strcmp(identificador, "RA")) || (!strcmp(identificador, "ra")) ){ 
		//converte o b.RA de int para char
		sprintf(charRA , "%d" , b.RA);
		
		if(!strcmp(chave, charRA)){

			comp_reg_total -= comp_reg; //voltar 1 para ficar no inicio do registro
			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
			//printf("\nconta aeee: %d", comp_reg_total);

			return comp_reg_total;
		}

	}

	// pesquisa pelo primeiro nome
	else if( (!strcmp(identificador, "nome")) || (!strcmp(identificador, "NOME")) || (!strcmp(identificador, "Nome")) ){ 
		
		if(!strcmp(chave, b.primeiro_nome)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
			
		}
	}

	// pesquisa pelo ultimo nome
	else if( (!strcmp(identificador, "sobrenome")) || (!strcmp(identificador, "SOBRENOME")) || (!strcmp(identificador, "Sobrenome")) ){ 
		
		if(!strcmp(chave, b.ultimo_nome)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);

		}
	}
	// pesquisa pelo ultimo nome
	else if( (!strcmp(identificador, "Nome+Sobrenome")) || (!strcmp(identificador, "NOME+SOBRENOME")) || (!strcmp(identificador, "nome+sobrenome")) ){

		//concatenar nome e sobrenome (separa-los por espaço)
		strcat(nome_e_sobrenome, b.primeiro_nome);
		strcat( nome_e_sobrenome , " ");
		strcat( nome_e_sobrenome , b.ultimo_nome);

		if(!strcmp(chave, nome_e_sobrenome)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);

		}
		strcpy(nome_e_sobrenome, "");
	}

	// pesquisa pela nota 1
	else if( (!strcmp(identificador, "Nota1")) || (!strcmp(identificador, "NOTA1")) || (!strcmp(identificador, "nota1")) ){	
		//converter o b.nota[0] de float para string
		sprintf(charNota1, "%.2f", b.nota[0]);
		//printf("\nnota1: %s", charNota1);
		if(!strcmp(chave, charNota1)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
		}
	}

	// pesquisa pela nota 2
	else if( (!strcmp(identificador, "nota2")) || (!strcmp(identificador, "NOTA2")) || (!strcmp(identificador, "Nota 2")) ){	
		//converter o b.nota[1] de float para string
		sprintf(charNota2, "%.2f", b.nota[1]);
		if(!strcmp(chave, charNota2)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
		}
	}

	// pesquisa pela nota 3
	else if( (!strcmp(identificador, "nota3")) || (!strcmp(identificador, "NOTA3")) || (!strcmp(identificador, "Nota3")) ){	
		//converter o b.nota[2] de float para string
		sprintf(charNota3, "%.2f", b.nota[2]);
		if(!strcmp(chave, charNota3)){

			printf("%d %6d %s %s %.2f %.2f %.2f\n",
       			comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
		}
	}

	else if( (!strcmp(identificador, "removerRegistros")) || (!strcmp(identificador, "REMOVERREGISTROS")) || (!strcmp(identificador, "REMOVER_REGISTROS")) ){

    	//concatenar nome e sobrenome (separa-los por espaço)
    	strcat(nome_e_sobrenome, b.primeiro_nome);
    	strcat( nome_e_sobrenome , " ");
    	strcat( nome_e_sobrenome , b.ultimo_nome);

    	if(!strcmp(chave, nome_e_sobrenome)){

      	printf("%d %6d %s %s %.2f %.2f %.2f\n",
            	comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);

    	}
    	strcpy(nome_e_sobrenome, "");
  	}

  } //end do for
  printf("\nNAO ENCONTROU\n");
  fclose(fp);
  //comp_reg_total -= comp_reg;
  //printf("\nvolta 1: %d", comp_reg_total);

  //return comp_reg_total;
  return 1;

}

void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2){
	int comp_reg, num_reg, pos1=0,r;
	Aluno a;
	char buffer[BSIZE];
  int ra;

  // funcao que cria um RA para adicionar ao aluno. Falta dar um jeito de atualizar o num_reg
  ra = criaRA();


	// cria 1o registro
  	a.RA = ra;
  	strcpy(a.primeiro_nome,primeiro_nome);
  	strcpy(a.ultimo_nome,ultimo_nome);
  	a.nota[0]=nota0;
  	a.nota[1]=nota1;
  	a.nota[2]=nota2;

	//FILE *fp;
	fp = fopen("dados.bin","rb");

	//Leitura arquivo
  	fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     // le BSIZE bytes do arquivo para buffer
	memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg

	close(fp);

	//fp = fopen("dados.bin","");

	num_reg++;										//Adiciona +1 registro que sera adicionado
//	printf("\n----->Numero de registros: %d\n", num_reg);
	memcpy(&(buffer[pos1]),&num_reg,sizeof(int));
	pos1 += sizeof(int);

   // fwrite(buffer,sizeof(int),pos1,fp);

	comp_reg =  //calcula quantos bytes serao ocupados no buffer
    sizeof(int) + // ra
    strlen(a.primeiro_nome)*(sizeof(char)) + // nome1
    1*sizeof(char) + // #
    strlen(a.ultimo_nome)*(sizeof(char)) + // nome2
    1*sizeof(char) + // #
    sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3 */


   

	//tamanho do registro no buffer
	memcpy(&(buffer[pos1]), &comp_reg, sizeof(int));
	pos1 += sizeof(int);
	//RA
	memcpy(&(buffer[pos1]),&a.RA,sizeof(int));
	pos1 += sizeof(int);
	//Primeiro nome + #
	memcpy(&(buffer[pos1]),a.primeiro_nome,sizeof(char)*strlen(a.primeiro_nome));
	pos1 += sizeof(char)*strlen(a.primeiro_nome);
	buffer[pos1] = '#';
	pos1 += sizeof(char);
	//Ultimo nome + #
 	memcpy(&(buffer[pos1]),a.ultimo_nome,sizeof(char)*strlen(a.ultimo_nome));
    pos1 += sizeof(char)*strlen(a.ultimo_nome);
    buffer[pos1] = '#';
    pos1 += 1*sizeof(char);
	//nota1
    memcpy(&(buffer[pos1]),&a.nota[0],sizeof(float));
    pos1 += sizeof(float);
	//nota2
    memcpy(&(buffer[pos1]),&a.nota[1],sizeof(float));
    pos1 += sizeof(float);
	//nota3
   	memcpy(&(buffer[pos1]),&a.nota[2],sizeof(float));
    pos1 += sizeof(float);

	//Salvar no Arquivo Bin

    fp = fopen("dados.bin","ab");
    fseek(fp, 0, SEEK_END);
    fwrite(buffer,sizeof(char),pos1,fp);

    fclose(fp);

	//Salvar no Arquivo no txt

	fp = fopen("dados.txt","ab");
	fprintf(fp, "%d&%s&%s&%.2f&%.2f&%.2f\n",a.RA, a.primeiro_nome, a.ultimo_nome, a.nota[0], a.nota[1], a.nota[2]);
  printf("\n%d&%s&%s&%.2f&%.2f&%.2f\n",a.RA, a.primeiro_nome, a.ultimo_nome, a.nota[0], a.nota[1], a.nota[2]);
	fclose(fp);
}

Aluno* recupera_registro(int deslocamento){

  Aluno aluno;
  char buffer2[100];
  int pos1=0, pos2;
  FILE *fp = fopen("dados.bin","r+");
  fseek(fp, deslocamento, SEEK_SET);
  fread(buffer2,sizeof(char),BSIZE,fp);
/*Guarda no arquivo-> num_reg  + cada reg-> [com_reg,ra,nome1,nome2,notas*3]*/



    // comp_reg
    int comp_reg =0;
    memcpy(&comp_reg,&(buffer2[pos1]),sizeof(int));
    pos1 += sizeof(int);

    // ra
    memcpy(&aluno.RA,&(buffer2[pos1]),sizeof(int));
    pos1 += sizeof(int);


    // nome1
    pos2 = pos1;
    while(buffer2[pos2]!='#') pos2 += sizeof(char);
    memcpy(aluno.primeiro_nome,&(buffer2[pos1]),pos2-pos1);
    aluno.primeiro_nome[pos2-pos1] = '\0';
    //printf("nome1 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.primeiro_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);

    // nome2
    pos1 = pos2 + sizeof(char);
    pos2 = pos1;
    while(buffer2[pos2]!='#') pos2 += sizeof(char);
    memcpy(aluno.ultimo_nome,&(buffer2[pos1]),pos2-pos1);
    aluno.ultimo_nome[pos2-pos1] = '\0';
    //printf("nome2 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.ultimo_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);
    pos1 = pos2 + sizeof(char);

    // nota1
    memcpy(&aluno.nota[0],&(buffer2[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[0]);

    // nota2
    memcpy(&aluno.nota[1],&(buffer2[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[1]);

    // nota3
    memcpy(&aluno.nota[2],&(buffer2[pos1]),sizeof(float));
    pos1 += sizeof(float);

    Aluno *m = (Aluno*)malloc(sizeof(Aluno));


   m->RA = aluno.RA;
   strcpy(m->primeiro_nome,aluno.primeiro_nome);
   strcpy(m->ultimo_nome,aluno.ultimo_nome);
   m->nota[0] = aluno.nota[0];
   m->nota[1] = aluno.nota[1];
   m->nota[2] = aluno.nota[2];

 //  printf(" \nRegistro recuperado >:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
   return m;

}


void imprime_dados(){

    int num_reg=0;
    int comp_reg = 0;
    Aluno b;
/* Simulando a leitura do arquivo*/
    FILE *fp;
    fp = fopen("dados.bin","r+");

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
 //   printf("---->num_reg %d\n",num_reg);

    int chave;

  char* nomesAqui = geraNomes();
  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));
    //printf("NOVO REG: %d\n",comp_reg);


 //   buscarAluno(1,chave, comp_reg); //**************************************************************************************************************

    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);
    }

    /* copia demais campos do registro*/

    // ra
    memcpy(&b.RA,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);

    // nome1
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.primeiro_nome,&(buffer[pos1]),pos2-pos1);
    b.primeiro_nome[pos2-pos1] = '\0';
    //printf("nome1 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.primeiro_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);

    // nome2
    pos1 = pos2 + sizeof(char);
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.ultimo_nome,&(buffer[pos1]),pos2-pos1);
    b.ultimo_nome[pos2-pos1] = '\0';
    //printf("nome2 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.ultimo_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);
    pos1 = pos2 + sizeof(char);

    // nota1
    memcpy(&b.nota[0],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[0]);

    // nota2
    memcpy(&b.nota[1],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[1]);

    // nota3
    memcpy(&b.nota[2],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[2]);

    /* imprime o registro lido*/
    printf("%d %6d %s %s %.2f %.2f %.2f\n",
     comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
  }
  fclose(fp);
}


void remove_logic(int ra){

	char charRA[20];
	sprintf(charRA , "%d" , ra);	
 	Aluno p;
	int deslocamento = buscarAluno("RA", charRA);
   Aluno *x = recupera_registro(deslocamento);
//   printf("\naqui x->ra: %d \nx->primeiro_nome: %s\nx->ultimo_nome: %s\n",x->RA,x->primeiro_nome , x->ultimo_nome);


 


   FILE *fp=fopen("dados.bin","rb+");

 //  printf("x->ra %d\n",x->RA);
   x->RA = -1;

 //  printf("\no que eh isso: %d %s\n",x->RA,x->primeiro_nome );
 	
 	int comp_reg =
      sizeof(int) + // ra
      strlen(x->primeiro_nome)*(sizeof(char)) + // nome1
      1*sizeof(char) + // #
      strlen(x->ultimo_nome)*(sizeof(char)) + // nome2
      1*sizeof(char) + // #
      sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3
      //int j = -1;
 //     printf("++%d++\n", deslocamento);

	char buffer_aux[100];
	int pos_aux=0;
	//comp_reg
	memcpy(&(buffer_aux[pos_aux]),&comp_reg,sizeof(int));
	pos_aux+=sizeof(int);
	//ra
	memcpy(&(buffer_aux[pos_aux]),&(x->RA),sizeof(int));
	pos_aux+=sizeof(int);

	//nome1+#
	memcpy(&(buffer_aux[pos_aux]),x->primeiro_nome,sizeof(char)*strlen(x->primeiro_nome));
    pos_aux += sizeof(char)*strlen(x->primeiro_nome);
    buffer_aux[pos_aux] = '#';
    pos_aux += 1*sizeof(char);
    //nome2+#
	memcpy(&(buffer_aux[pos_aux]),x->ultimo_nome,sizeof(char)*strlen(x->ultimo_nome));
    pos_aux += sizeof(char)*strlen(x->ultimo_nome);
    buffer_aux[pos_aux] = '#';
    pos_aux += 1*sizeof(char);
//nota1
    memcpy(&(buffer_aux[pos_aux]),&(x->nota[0]),sizeof(float));
    pos_aux += sizeof(float);
    //nota2
    memcpy(&(buffer_aux[pos_aux]),&(x->nota[1]),sizeof(float));
    pos_aux += sizeof(float);
    //nota3
    memcpy(&(buffer_aux[pos_aux]),&(x->nota[2]),sizeof(float));
    pos_aux += sizeof(float);
	x->RA= -1;
	fseek(fp, deslocamento,SEEK_SET);
	fwrite(buffer_aux,comp_reg,1,fp);
//	printf("Removido %s, %s %d\n",x->primeiro_nome,x->ultimo_nome, comp_reg);
//	printf("\n----------------------\n");
	fclose(fp);

//	reorganizacao_dados();

}

void reorganizacao_dados(){

    int num_reg=0;
    int comp_reg = 0;
    Aluno b;
/* Simulando a leitura do arquivo*/
    FILE *fp;
    fp = fopen("dados.bin","rb+");

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
  //  printf("---->num_reg000 %d\n",num_reg);


    int chave;
    char buffer3[100];
   //to sobrescrevendo num reg

  FILE *fp_novo = fopen("dados2.bin","wb");
  fwrite(&(buffer[0]),sizeof(char),sizeof(int),fp_novo);
  fclose(fp_novo);

  char* nomesAqui = geraNomes();

  fp_novo = fopen("dados2.bin","rb+");
  //fseek(fp_novo,sizeof(int),SEEK_SET);
  int pos3=0;
  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);
    }

    /* copia demais campos do registro*/

    // ra
    memcpy(&b.RA,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);

    // nome1
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.primeiro_nome,&(buffer[pos1]),pos2-pos1);
    b.primeiro_nome[pos2-pos1] = '\0';
    //printf("nome1 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.primeiro_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);

    // nome2
    pos1 = pos2 + sizeof(char);
    pos2 = pos1;
    while(buffer[pos2]!='#') pos2 += sizeof(char);
    memcpy(b.ultimo_nome,&(buffer[pos1]),pos2-pos1);
    b.ultimo_nome[pos2-pos1] = '\0';
    //printf("nome2 %d - %d = %d %s \n",pos2,pos1,pos2-pos1,b.ultimo_nome);

    //printf("MARCADOR %c\n",buffer[pos2]);
    pos1 = pos2 + sizeof(char);

    // nota1
    memcpy(&b.nota[0],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[0]);

    // nota2
    memcpy(&b.nota[1],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b.nota[1]);

    // nota3
    memcpy(&b.nota[2],&(buffer[pos1]),sizeof(float));
    pos1 += sizeof(float);
    //printf("nota %.2f\n",b

    //if ra!=-1 escreve no arquivo novo
   // printf("%d\n", b.RA);
    if (b.RA!=-1){
        //COpia comp_reg e b pro buffer3

    memcpy(&(buffer3[pos3]),&comp_reg,sizeof(int));
    pos3+=sizeof(int);

    // ra
    memcpy(&(buffer3[pos3]),&b.RA,sizeof(int));
    pos3 += sizeof(int);

    // nome1 + #
    memcpy(&(buffer3[pos3]),b.primeiro_nome,sizeof(char)*strlen(b.primeiro_nome));
    pos3 += sizeof(char)*strlen(b.primeiro_nome);
    buffer3[pos3] = '#';
    //printf("nome1 %s\n",&(buffer[pos1-sizeof(char)*strlen(a[r].primeiro_nome)]));
    pos3 += 1*sizeof(char);

    // nome2 + #
    memcpy(&(buffer3[pos3]),b.ultimo_nome,sizeof(char)*strlen(b.ultimo_nome));
    pos3 += sizeof(char)*strlen(b.ultimo_nome);
    buffer3[pos3] = '#';
    //printf("nome2 %s\n",&(buffer[pos1-sizeof(char)*strlen(a[r].ultimo_nome)]));
    pos3 += 1*sizeof(char);

    // nota1
    memcpy(&(buffer3[pos3]),&b.nota[0],sizeof(float));
    //printf("nota1 %s\n",&(buffer[pos1]));
    pos3 += sizeof(float);

    // nota2
        memcpy(&(buffer3[pos3]),&b.nota[1],sizeof(float));
    //printf("nota2 %s\n",&(buffer[pos1]));
    pos3 += sizeof(float);

    // nota3

    memcpy(&(buffer3[pos3]),&b.nota[2],sizeof(float));
    //printf("nota3 %s\n",&(buffer[pos1]));
    pos3 += sizeof(float);



        //escreve
        fseek(fp_novo,0,SEEK_END);
        fwrite(buffer3,sizeof(char),pos3,fp_novo); //de onde começa o arquivo
        pos3=0;

    }
    else{
            int num_reg2;
            //atualiza num_reg e grava no arquivo /*AQUI*/
            pos3=0;
            fseek(fp_novo,0,SEEK_SET);
           fread(&(buffer3[pos3]),sizeof(char),sizeof(int),fp_novo);
           memcpy(&(num_reg2), &buffer3[0], sizeof(int));
     //      printf(" aca->%d<-aca\n",num_reg2 );
            num_reg2--;

         fseek(fp_novo, 0, SEEK_SET);
         fwrite(&num_reg2,sizeof(int),1,fp_novo);


    }
    //adiciona no arquivo indice

    /* imprime o registro lido*/
   /// printf("%d %6d %s %s %.2f %.2f %.2f\n",
   //  comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
  //}
}
  fclose(fp_novo);
  fclose(fp);

  //Exclui outro arquivo
 remove("dados.bin");
    char arq_antigo[] = "dados2.bin";
    char arq_novo[] = "dados.bin";
    //Renomeia o auxiliar
    rename(arq_antigo, arq_novo);
  imprime_dados();

}

void le_script(){

FILE *arq;
 arq = fopen("script.txt", "r");
      char linha[200];//Escolher tamanho adequado p linha /PS: nao funciona com ponteiro pois na linha 50 usa-se sizeof(linha)
      char *nome, *ult_nome, *nota1, *nota2, *nota3, *ra_aux, *comando,*registro;
      int num=0;
      if(arq == NULL)
             printf("Erro, nao foi possivel abrir o arquivo\n");
     else
         while((fgets(linha,sizeof(linha), arq))!=NULL )
          {
            if (linha[0] == 'a'){
                printf("Adicionar\n");
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");

                nome = (char*)strtok(registro, ",");
                ult_nome = (char*)strtok(NULL, ",");
                nota1 = (char*)strtok(NULL, ",");
                nota2 = (char*)strtok(NULL, ",");
                nota3 = (char*)strtok(NULL, ",");


                adicionarAluno(nome,ult_nome,atof(nota1),atof(nota2),atof(nota3));
                //imprime_dados();

            }
            else if (linha[0] == 'b'){
                printf("Busca\n");
                int aux;
                char auxChar[10];
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                sprintf(auxChar , "%d" , aux);
                int desloc = buscarAluno("RA" , auxChar);
                //int desloc = buscar_indice(atoi(registro),&aux);
                Aluno *n =  recupera_registro(desloc);


            }
            else if (linha[0] == 'r'){
                printf("Remocao\n");
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                remove_logic(atoi(registro));
                //imprime_dados();

            }
            if (linha[0] == 'R'){
                printf("Reorganização\n");
                reorganizacao_dados();


            }

}
}