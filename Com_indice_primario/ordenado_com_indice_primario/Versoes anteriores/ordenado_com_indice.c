
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

typedef struct ancora {
  int RA;
  int deslocamento;
 } ancora;

/*Variaveis globais*/
 int   pos1,pos2,r;
FILE *fp;
char buffer[100];

/*Indice*/
char buffer_indice[100];
FILE *fp_indice;

void insere_ordenado_indice(int RA, int deslocamento);
void imprime_indice();
void cria_arquivos(Aluno *a, ancora *c);
int buscaPosicao(int RA);

//FAZER
//Ponteiro é por bloco
//INSERIR NO INDICE E NO BLOCO





int main(){
	geraArquivo(); //Tirar isso da main e gerar todos arquivos adequados antes
	//cria_arquivos(a, c);//Criar arquivos binarios na main?

}



void cria_arquivos(Aluno *a, ancora *c){
    int comp_reg, num_reg;

ancora  d;
FILE *fp;
num_reg=TAM_MAX;
int deslocamento=sizeof(int);

int deslocamento_total = sizeof(int);
 /*Criação do arquivo binario*/

//Arquivo
  fp = fopen("dados.bin","wb");
  pos1 = 0;
  memcpy(&(buffer[pos1]),&num_reg,sizeof(int));
  pos1 += sizeof(int);


 //Indice
   pos1=0;
    FILE *fp_indice;
    char buffer_aux[100];
    fp_indice = fopen("indice_desordenado.bin","wb"); //Cria arquivo na primeira vez
    fclose(fp_indice);

    fp_indice = fopen("indice_desordenado.bin","wb+");
    fwrite(&num_reg,sizeof(char),sizeof(int),fp_indice);
    fclose(fp_indice);

  pos1=sizeof(int);
  for (r=0; r < num_reg; r++) {

     insere_ordenado_indice(a[r].RA,deslocamento);
     printf("r%d\n",r); //4444
    //Arquivo
    comp_reg =
      sizeof(int) + // ra
      strlen(a[r].primeiro_nome)*(sizeof(char)) + // nome1
      1*sizeof(char) + // #
      strlen(a[r].ultimo_nome)*(sizeof(char)) + // nome2
      1*sizeof(char) + // #
      sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3



/* Montando buffer*/
    if (pos1+sizeof(int)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&comp_reg,sizeof(int));
    pos1+=sizeof(int);

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



  deslocamento = deslocamento + comp_reg;
}

  if (pos1>0) {
    //printf("final %s %d %d\n",buffer,strlen(buffer),pos1);
    fwrite(buffer,sizeof(char),pos1,fp);
  }
//printf("criado %d\n", deslocamento);

  fclose(fp);
imprime_indice();

}





int buscaPosicao(int RA)
{
    int tam_arquivo, fim;
    fp = fopen("arquivo_ordenado.bin","rb");
    if(fp == NULL)
    {
        return -1;
    }
    fseek(fp, 0, SEEK_END);
    tam_arquivo = ftell(fp);
    fim= tam_arquivo;
    fseek(fp, 0, SEEK_SET);

    int RA_lido, bloco=0, tam_buffer, pos1=0, pos_correta, tam_registro;

    pos1 += sizeof(int);

    while(tam_arquivo != 0)
    {
        if(tam_arquivo >= BSIZE)
        {
            tam_buffer = BSIZE;
        }
        else
        {
            tam_buffer = tam_arquivo;
        }
        fseek(fp, (BSIZE*bloco), SEEK_SET);
        fread(&(buffer), sizeof(char), tam_buffer, fp);

        while(pos1 < tam_buffer)
        {
            pos_correta = 0;
            // Verifico se posso ler NUM_TOTAL_BYTES
            if(pos1 > BSIZE-4)
            {
                pos_correta = pos1;

				fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
				fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
				pos1 = 0;
            }

            memcpy(&tam_registro, &(buffer[pos1]), sizeof(int));
            memcpy(&RA_lido, &(buffer[pos1+4]), sizeof(int));

            if(RA_lido > RA)
            {
                fclose(fp);
                return ((BSIZE*bloco) + pos_correta + pos1);
            }
            pos1 += tam_registro + pos_correta + 4;
        }
        if(pos1 >= BSIZE) pos1 -= BSIZE;
        bloco++;
        tam_arquivo -= tam_buffer;
    }
    fclose(fp);
    return -1;
}



//INSERE ORDENADO PELO RA CRIA INDICE AQUI
void insereOrdenado(Aluno a)
{
	fp = fopen("arquivo_ordenado.bin","rb+"); //Abre o arquivo, para a leitura


	int   num_reg, aux, tam_arquivo, tam_reg,i, fim, inicio, quant_desl,desl, comp_reg, blocos=0, dif=0;

	pos1 = sizeof(int);
	fread(&(buffer[pos1]),sizeof(int),sizeof(int),fp);
	memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le a quantidade de registros


    comp_reg=  //calcula quantos bytes serao ocupados no buffer
	sizeof(int) + // ra
	strlen(a.primeiro_nome)*(sizeof(char)) + // nome1
	1*sizeof(char) + // #
	strlen(a.ultimo_nome)*(sizeof(char)) + // nome2
	1*sizeof(char) + // #
	sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3

	tam_reg=(comp_reg + sizeof(int));

	fclose(fp);
	inicio= buscaPosicao(a.RA); // recebe a o valor de onde deve começar a movimentação dos registros


	fp = fopen("arquivo_ordenado.bin","rb+");

	fseek(fp, 0, SEEK_END);
    tam_arquivo = ftell(fp); //Tamanho do Arquivo

    fim = tam_arquivo;  //O fim recebe o tamanho do arquivo
	printf("inicio %d fim %d ", inicio, fim);



	if(inicio>0)
	{
		 quant_desl = fim - inicio;
		while(quant_desl > 0)
        {
            printf("\n\n==> Quantidade a deslocar: %d, inicio %d fim %d\n\n", quant_desl, inicio, fim);

            if(quant_desl > BSIZE)
            {

					desl = BSIZE;
            }
            else
            {
                desl = quant_desl;
            }

            fim -= desl;
            fseek(fp, fim, SEEK_SET);
            fread(&(buffer), sizeof(char), desl, fp);
            fim += tam_reg;
            fseek(fp, fim, SEEK_SET);
            fwrite(buffer, sizeof(char), desl, fp);
            fim -= tam_reg;
            fseek(fp, fim, SEEK_SET);
            quant_desl -= desl;
            pos1=inicio;
        }
	}
	else
	{
		fseek(fp, tam_arquivo, SEEK_SET);
	}
	pos1=0;
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

			fwrite(buffer,sizeof(char),pos1,fp);
			fclose(fp);


			return;

}


//IMPRIME DADOS
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
    printf("---->num_reg %d\n",num_reg);

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


void imprime_indice(){

FILE *fp;
int num_reg, comp_reg;
ancora d;
/* Simulando a leitura do arquivo*/ //INDICE
  fp = fopen("indice_desordenado.bin","r+");

  pos1 = 0;
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  printf("---->num_reg indice %d\n",num_reg);



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
    memcpy(&d.RA,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);


    //deslocamento
  memcpy(&d.deslocamento,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);

      printf("%d %d\n",d.RA,d.deslocamento);

  }
}


void insere_ordenado_indice(int RA, int deslocamento){

    FILE *fp_indice;
    int   num_reg_indice, pos1_indice,aux, tam_arquivo, tam_reg,i, fim, inicio, quant_desl,desl, comp_reg_indice, blocos=0, dif=0;
    char buffer_indice[100];
    pos1_indice = sizeof(int);

    fp_indice = fopen("indice_desordenado.bin","rb"); //Abre o arquivo, para a leitura



    fread(&(buffer_indice[pos1_indice]),sizeof(int),sizeof(int),fp_indice);
    memcpy(&num_reg_indice,&(buffer_indice[pos1_indice]),sizeof(int));     // le a quantidade de registros
    comp_reg_indice = sizeof(int)*2;
    tam_reg=(comp_reg_indice + sizeof(int));

    fclose(fp_indice);
    inicio= buscaPosicao(RA); // recebe a o valor de onde deve começar a movimentação dos registros'
    //printf("posicao->%d\n", inicio); //12345
    //printf("Ra: %d\n",RA );
    fp_indice = fopen("indice_desordenado.bin","rb+");


    fseek(fp_indice, 0, SEEK_END);

    tam_arquivo = ftell(fp_indice); //Tamanho do Arquivo
    //if (tam_arquivo>100) return;

    fim = tam_arquivo;  //O fim recebe o tamanho do arquivo
  // printf("inicio %d fim %d ", inicio, fim);

    if(inicio>0)
    {
         quant_desl = fim - inicio;
        while(quant_desl > 0)
        {
            //printf("\n\n==> Quantidade a deslocar: %d, inicio %d fim %d\n\n", quant_desl, inicio, fim);

            if(quant_desl > BSIZE)
            {

                    desl = BSIZE;
            }
            else
            {
                desl = quant_desl;
            }

            fim -= desl;
            fseek(fp_indice, fim, SEEK_SET);
            fread(&(buffer_indice), sizeof(char), desl, fp_indice);
            fim += tam_reg;
            fseek(fp_indice, fim, SEEK_SET);
            fwrite(buffer_indice, sizeof(char), desl, fp_indice);
            printf("desl %d\n",desl );
            fim -= tam_reg;
            fseek(fp_indice, fim, SEEK_SET);
            quant_desl -= desl;
            pos1_indice=inicio;
        }
    }
    else
    {
        fseek(fp_indice, 0, SEEK_END);
    }
    pos1_indice=0;
            //tamanho do registro no buffer_indice
            memcpy(&(buffer_indice[pos1_indice]), &comp_reg_indice, sizeof(int));
            pos1_indice += sizeof(int);
            //RA
            memcpy(&(buffer_indice[pos1_indice]),&RA,sizeof(int));
            pos1_indice += sizeof(int);
            //Deslocamento
            memcpy(&(buffer_indice[pos1_indice]),&deslocamento,sizeof(int));
            pos1_indice += sizeof(int);

//Salvar no Arquivo Bin

            fwrite(buffer_indice,sizeof(char),pos1_indice,fp_indice);
            fclose(fp_indice);

            return;

}














