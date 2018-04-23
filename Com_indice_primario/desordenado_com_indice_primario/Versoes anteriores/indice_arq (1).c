
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAM_MAX  4
#define BSIZE 100

typedef struct ancora {
  int RA;
  int marcador;
  //ponteiro para registros
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
      a[0].marcador = 0;
      a[1].marcador = 0;
      a[2].marcador = 1;
      a[3].marcador = 0;


     int num=0;

/*******************************************************************/

 /*Criação do arquivo binario*/

  fp = fopen("indice_desordenado.bin","wb");

  pos1 = 0; //Numero de bytes ocupados no buffer

  memcpy(&(buffer[pos1]),&num_reg,sizeof(int)); //Copia numero de registros para primeira posição do buffer
  //printf("num_reg %d %s\n",num_reg,&(buffer[pos1])); //buffer[pos] varia pq???
  pos1 += sizeof(int); //esta sendo ocupado o numero de bytes de um inteiro no buffer

  for (r=0; r < num_reg; r++) {
    //printf("num_reg %d %d\n",num_reg,(buffer[pos1]));
    comp_reg =  //calcula quantos bytes serao ocupados no buffer
      sizeof(int)+sizeof(int); // ra
      //printf("%d %d \n",comp_reg, a[r].RA );

/* Montando buffer*/

    if (pos1+sizeof(int)>=BSIZE) {         //se a escrita de um int ultrapassar o tamanho do buffer.
      fwrite(buffer,sizeof(char),pos1,fp); //escreve o buffer em memoria e
      pos1 = 0;                            //esvazia buffer
    }
    memcpy(&(buffer[pos1]),&comp_reg,sizeof(int)); //escreve quantos bytes o registro ocupa no buffer

    pos1+=sizeof(int);                             //atualiza numero de bytes ocupados no buffer

    // ra
    if (pos1+sizeof(int)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].RA,sizeof(int));
    //printf("ra %s\n",&(buffer[pos1]));
    pos1 += sizeof(int);

 // marcador
    if (pos1+sizeof(int)>=BSIZE) {
      fwrite(buffer,sizeof(char),pos1,fp);
      pos1 = 0;
    }
    memcpy(&(buffer[pos1]),&a[r].marcador,sizeof(int));
    //printf("marcador %s\n",&(buffer[pos1]));
    pos1 += sizeof(int);
    //printf("\nReg:  %d %d/n",a[r].marcador,a[r].RA);

  } // end for

  if (pos1>0) {

    fwrite(buffer,sizeof(char),pos1,fp);
  }

  fclose(fp);

/************************************************************/

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
    //marcador
  memcpy(&b.marcador,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);
    //intf("%d\n",a[r].marcador );
    /* imprime o registro lido*/
      printf("%d %d\n",  b.marcador,b.RA);

  }

  fclose(fp);
  buscarIndice(1, 523 , 0);
  return 0;

}


//busca pelo RA
int buscarIndice(int identificador , int chave , int comp_reg){ //+++++++++++++++++
ancora b;
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
    //marcador
  memcpy(&b.marcador,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    //printf("ra %d\n",b.RA);
    //intf("%d\n",a[r].marcador );
    /* imprime o registro lido*/
      //printf("%d %d\n",  b.marcador,b.RA);
   if (identificador==1){ //busca por ra
    if (b.RA==chave){
    printf("achei saporra:\n");
    printf("%d %d\n",b.marcador,b.RA);
    }
  }



//ORDENAR ARQUIVO UTILIZANDO SHELL SHORT? DÁ PRA FAZER SE O FWRITE "TROCAR" REGISTROS DE POSIÇÕES


  } //end do for
  fclose(fp);


}
