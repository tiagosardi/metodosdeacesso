
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#include "gerador_de_dados.h"

#define BSIZE 100

//FAZER
//Tirar marcador - Feito
//Criar imprime indice e imprime arquivo de dados - Feito

//Busca - Feito, mas sequencial
//Insercao -
//Exclusao - Feito/2 - falta excluir no arquivo de indice
//Atualização
//Modificar indice ao inserir, excluir,etc


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


void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2);
int buscarAluno(int identificador , int chave, int comp_reg);
Aluno* recupera_registro(int deslocamento);
void imprime_dados();
void imprime_indice();
Aluno*  recupera_dados(Aluno *a);
void cria_arquivos(Aluno *a, ancora *c);
void remove_logic(int deslocamento);
int buscar_indice(int chave, int *deslocamento_indice);
/*Variaveis globais*/
 int   pos1,pos2,r;
FILE *fp;
char buffer[100];

/*Indice*/
char buffer_indice[100];
FILE *fp_indice;

int main()
{
      int   comp_reg;
      int comp_reg_indice;
      int deslocamento_total=sizeof(int); //Inicia deslocamento(offset) em sizeof(int) por conta do num_reg no inicio do arquivo.
      int num_reg= TAM_MAX ;/*editar na biblioteca*/
      Aluno b;  //Cria o vetor do tamanho do numero de registros
      ancora *c=(ancora*)malloc(sizeof(ancora)*num_reg), d;//Vetor p indice
     Aluno *a = (Aluno*)malloc(sizeof(Aluno)*num_reg);



    geraArquivo(); //Tirar isso da main e gerar todos arquivos adequados antes

    recupera_dados(a);
    cria_arquivos(a, c);
/***************************************************************************/
imprime_dados(); //Imprime todos os dados que estão no arquivo de dados
imprime_indice(); //Imprime todos os indices que estão no arquivo de indices

/*Simula busca no indice*/
int n;
 int mm = buscar_indice(14,&n);
    if (mm==-1)
    {
     printf(" nao encontrado\n");
    }
    printf("nana\n");
remove_logic(3);

return 0;


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

  FILE *fp;
  fp = fopen("dados.bin","rb");

  //Leitura arquivo
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     // le BSIZE bytes do arquivo para buffer
  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg

  close(fp);

  //fp = fopen("dados.bin","");

  num_reg++;                    //Adiciona +1 registro que sera adicionado
  //printf("\n----->Numero de registros: %d\n", num_reg);
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


    int* vetor1 = criaVetor1(); //deletar isso aqui

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
  fclose(fp);

}

//busca pelo RA
int buscar_indice(int chave, int *deslocamento_indice){
/*int(ra) + int(deslocamento)*/

  int comp_reg;
  int num_reg;
  ancora c,d;

 FILE *fp = fopen("indice_desordenado.bin","r+");

  pos1 = 0;
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("---->num_reg %d\n",num_reg);

 *deslocamento_indice=sizeof(int);


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
    memcpy(&d.RA,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    //deslocamento
    memcpy(&d.deslocamento,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    if(d.RA==chave) {
            recupera_registro(d.deslocamento);
            printf("chegou chegou\n" );
            fclose(fp);
           return d.deslocamento;

    }

   *deslocamento_indice = *deslocamento_indice + 3*sizeof(int); //comp_reg, ra, deslocamento

  } //end do for


  fclose(fp);
  return -1;//nao encontrado

}

Aluno* recupera_registro(int deslocamento){
  //desloca
  //joga no buffer
  //interpreta até a nota3
  //printf("chegou deslocamento: %d\n",deslocamento );
  //deslocamento = deslocamento;
//esquecendo de contar algo no deslocamento

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

   printf(" Registro recuperado:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
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
    memcpy(&d.RA,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);


    //deslocamento
  memcpy(&d.deslocamento,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);

    //printf("ra %d\n",b.RA);
    //intf("%d\n",a[r].marcador );
    /* imprime o registro lido*/
      printf("%d %d\n",d.RA,d.deslocamento);

  }
}

Aluno*  recupera_dados(Aluno *a){
/*Captura dados no txt*/
    int num_reg=TAM_MAX;

      char linha[200];//Escolher tamanho adequado p linha /PS: nao funciona com ponteiro pois na linha 50 usa-se sizeof(linha)
      char *nome, *ult_nome, *nota1, *nota2, *nota3, *ra_aux;
      FILE *arq;

      int num=0;
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

  return a;


}


void cria_arquivos(Aluno *a, ancora *c){
    int comp_reg, num_reg, comp_reg_indice;

ancora  d;
FILE *fp, *fp_indice;
num_reg=TAM_MAX;

int deslocamento_total = sizeof(int);
 /*Criação do arquivo binario*/

//Arquivo
  fp = fopen("dados.bin","wb");
  pos1 = 0;
  memcpy(&(buffer[pos1]),&num_reg,sizeof(int));
  pos1 += sizeof(int);


  //Indice

  fp_indice = fopen("indice_desordenado.bin","wb");
  int pos1_indice = 0;
  memcpy(&(buffer_indice[pos1_indice]),&num_reg,sizeof(int));
  pos1_indice += sizeof(int);


  for (r=0; r < num_reg; r++) {

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



    //Indice

    c[r].RA = a[r].RA;

    //printf("num_reg %d %d\n",num_reg,(buffer[pos1]));
    comp_reg_indice =  //Calcula quantos bytes serao ocupados no buffer
      sizeof(int)+sizeof(int) ; // ra e deslocamento
      c[r].deslocamento = deslocamento_total; //Salva quantos bytes precisa pular p chegar nesse registro
      deslocamento_total +=comp_reg+sizeof(int); //Incrementa o deslocamento total


/* Montando buffer*/
    if (pos1_indice+sizeof(int)>=BSIZE) {         //se a escrita de um int ultrapassar o tamanho do buffer.
      fwrite(buffer_indice,sizeof(char),pos1_indice,fp_indice); //escreve o buffer em memoria e
      pos1_indice = 0;                            //esvazia buffer
    }
    memcpy(&(buffer_indice[pos1_indice]),&comp_reg_indice,sizeof(int)); //escreve quantos bytes o registro ocupa no buffer

    pos1_indice+=sizeof(int);                             //atualiza numero de bytes ocupados no buffer

    // ra
    if (pos1_indice+sizeof(int)>=BSIZE) {
      fwrite(buffer_indice,sizeof(char),pos1_indice,fp_indice);
      pos1_indice = 0;
    }
    memcpy(&(buffer_indice[pos1_indice]),&c[r].RA,sizeof(int));
    pos1_indice += sizeof(int);

 // deslocamento
    if (pos1_indice+sizeof(int)>=BSIZE) {
      fwrite(buffer_indice,sizeof(char),pos1_indice,fp_indice);
      pos1_indice = 0;
    }
    memcpy(&(buffer_indice[pos1_indice]),&c[r].deslocamento,sizeof(int));

    pos1_indice += sizeof(int);


  } // end for


    if (pos1_indice>0) {

    fwrite(buffer_indice,sizeof(char),pos1_indice,fp_indice);
  }

  fclose(fp_indice);


  if (pos1>0) {
    //printf("final %s %d %d\n",buffer,strlen(buffer),pos1);
    fwrite(buffer,sizeof(char),pos1,fp);
  }

  fclose(fp);


}

void remove_logic(int ra){

   int deslocamento_indice;
   int deslocamento = buscar_indice(ra,&deslocamento_indice); //comp_reg/ra/int
  Aluno *x = recupera_registro(deslocamento);
  printf("%d %s\n",x->RA,x->primeiro_nome );


   FILE *fp=fopen("dados.bin","rb+");

  printf("x->ra %d\n",x->RA);
  x->RA = -1;

  printf("%d %s\n",x->RA,x->primeiro_nome );
 int comp_reg =
      sizeof(int) + // ra
      strlen(x->primeiro_nome)*(sizeof(char)) + // nome1
      1*sizeof(char) + // #
      strlen(x->ultimo_nome)*(sizeof(char)) + // nome2
      1*sizeof(char) + // #
      sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3
      //int j = -1;
      printf("++%d++\n", deslocamento);

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
printf("Removido %s, %s %d\n",x->primeiro_nome,x->ultimo_nome, comp_reg);
printf("\n----------------------\n");
fclose(fp);





FILE *fp_indice =fopen("indice_desordenado.bin","rb+");
fseek(fp_indice,deslocamento_indice,SEEK_SET);







char buffer_indice1[sizeof(int)*3];
int pos0=0;

ancora m;
int comp_reg_indice=0;

fseek(fp_indice,deslocamento_indice,SEEK_SET);
fread(&(buffer_indice1[pos0]),sizeof(char),sizeof(int)*3,fp);     /* le BSIZE bytes do arquivo para buffer*/



//comp_reg
memcpy(&comp_reg_indice,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);
printf("comp_reg_indice: %d\n",comp_reg_indice);
//ra
memcpy(&m.RA,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);
printf("ra da porra do indice%d\n",m.RA );
//deslocamento
memcpy(&m.deslocamento,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);
printf("deslocamento da porra do indice%d ind-> %d\n",m.deslocamento,deslocamento_indice);

//memcpy(&(buffer_indice[pos1_indice]),&c[r].RA,sizeof(int));
m.RA= -1;
memcpy(&(buffer_indice1[4]),&m.RA,sizeof(int));




fwrite(buffer_indice1,comp_reg_indice,1,fp_indice);
fclose(fp_indice);
  imprime_dados();
  imprime_indice();

//tira do indice

}