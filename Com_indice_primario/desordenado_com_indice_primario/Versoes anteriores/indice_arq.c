
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define BSIZE 100

typedef struct ancora {
  int RA;
  //ponteiro para registro
  //ponteiro para proximo
} ancora;


/*Variaveis globais*/
 int   pos1,pos2,r;
  FILE *fp;
  char buffer[100];
  int num_reg = 4;

int main()
{
      int   comp_reg;
      ancora a[4],b; //Cria um vetor com 4


     //Isso será lido do arquivo de dados, atualizar fp p apontar p o registro especifico
      a[0].RA = 123;
      a[1].RA = 523;
      a[2].RA = 623;
      a[3].RA = 923;


     int num=0;

/***************************************************************************/

 /*Criação do arquivo binario*/

  fp = fopen("indice_desordenado.bin","wb");

  pos1 = 0; //Numero de bytes ocupados no buffer

  memcpy(&(buffer[pos1]),&num_reg,sizeof(int)); //Copia numero de registros para primeira posição do buffer
  //printf("num_reg %d %s\n",num_reg,&(buffer[pos1])); //buffer[pos] varia pq???
  pos1 += sizeof(int); //esta sendo ocupado o numero de bytes de um inteiro no buffer

  for (r=0; r < num_reg; r++) {
    //printf("num_reg %d %d\n",num_reg,(buffer[pos1]));
    comp_reg =  //calcula quantos bytes serao ocupados no buffer
      sizeof(int); // ra
      //printf("%d %d \n",comp_reg, a[r].RA );

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


  } // end for

  if (pos1>0) {
    //printf("final %s %d %d\n",buffer,strlen(buffer),pos1);
    fwrite(buffer,sizeof(char),pos1,fp);
  }

  fclose(fp);

/*************************************************************************/

/* Simulando a leitura do arquivo*/
  fp = fopen("indice_desordenado.bin","r+");

  pos1 = 0;
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("---->num_reg %d\n",num_reg);



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
    /* imprime o registro lido*/
    printf("%d %d\n",  comp_reg,b.RA);
  }

  fclose(fp);

  return 0;

}