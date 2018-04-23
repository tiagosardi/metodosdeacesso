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

//Funcao que adiciona um aluno
void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2);
//Função que recupera um registro dado um deslocamento no arquivo principal
Aluno* recupera_registro(int deslocamento);
//Imprime todos os registros do arquivo
void imprime_dados();
//Imprime todos os indices do arquivo de indice
void imprime_indice();
//Função que busca dados no script(txt)
Aluno*  recupera_dados(Aluno *a);
//Cria arquivo de dados e indice
void cria_arquivos(Aluno *a, ancora *c);
//Remove logicamente um registro(Muda RA para -1)
void remove_logic(int deslocamento);
//Busca registro no arquivo de indices
int buscar_indice(int chave, int *deslocamento_indice);
//Insere uma ancora no indice
void insere_ordenado_indice(int RA, int deslocamento);
//Busca posição para inserção ordenada(usada na função insere_ordenado_indice)
int buscaPosicao(int RA);
//Funcao de reorganização dos arquivos de dados e consequentemente de indices
void reorganizacao_dados();

void le_script();
/*Variaveis globais*/
 int   pos1,pos2,r;
FILE *fp;
char buffer[100];
//Indice
char buffer_indice[100];
FILE *fp_indice;

void temporeoganiza(){

  clock_t t0, tf;
  int mm;
    double tempo_gasto, somatorio;
    //t0 = clock();
    int i;
    srand ((unsigned)time(NULL));


      t0 = clock();//remove_logic(i+1);
      reorganizacao_dados(); //adicionarAluno(geraNomes(), geraSobrenomes(), geraNotas(), geraNotas(), geraNotas()) ;
       tf = clock();
       tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;

    printf("%lf\n", tempo_gasto);

}
int main()
{
      srand( (unsigned)time(NULL) );
      int   comp_reg;
      int comp_reg_indice;
      int deslocamento_total=sizeof(int); //Inicia deslocamento(offset) em sizeof(int) por conta do num_reg no inicio do arquivo.
      int num_reg= TAM_MAX ;//TAM_MAX é o numero de registros
      Aluno b;
      ancora *c=(ancora*)malloc(sizeof(ancora)*num_reg), d;//Vetor p indice
     Aluno *a = (Aluno*)malloc(sizeof(Aluno)*num_reg);



    geraArquivo();

recupera_dados(a);
 cria_arquivos(a, c);
 le_script();

 //printf("Antes\n");
 //imprime_dados();

//remove_logic(6);

 //reorganizacao_dados();
 //printf("Depois\n");
 //imprime_dados();
 //imprime_indice();


return 0;
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
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                int desloc = buscar_indice(atoi(registro),&aux);
                if (desloc!=-1) {
                    Aluno *n =  recupera_registro(desloc);
                }
            }
            else if (linha[0] == 'r'){
                printf("Remocao\n");
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                remove_logic(atoi(registro));
                imprime_dados();

            }
            if (linha[0] == 'R'){
                printf("Reorganização\n");
                reorganizacao_dados();


            }

}
}

void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2){
  int comp_reg, num_reg, pos1=0,r;
  Aluno a;
  char buffer[BSIZE];
  int ra;

  // Funcao que cria um RA para adicionar ao aluno.

  ra = criaRA();

  // cria 1o registro
    a.RA = ra;
    strcpy(a.primeiro_nome,primeiro_nome);
    strcpy(a.ultimo_nome,ultimo_nome);
    a.nota[0]=nota0;
    a.nota[1]=nota1;
    a.nota[2]=nota2;

  FILE *fp;

    //Atualiza num_reg
    fp = fopen("dados.bin","rb+");
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);
    memcpy(&(num_reg), &buffer[0], sizeof(int));
    num_reg++;

    memcpy(&(buffer[pos1]), &num_reg, sizeof(int));
    pos1=sizeof(int);
    fseek(fp, 0, SEEK_SET);
    fwrite(&buffer,sizeof(char),sizeof(int),fp);

    fclose(fp);

    pos1=0;

  //Leitura arquivo

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
  pos1 += 1*sizeof(char);
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

//Atualiza no indice

char buffer_aux[100];

int comp_reg_indice = sizeof(int)*2;
pos1=0;
ancora ind;
ind.RA = ra;
fp = fopen("dados.bin","rb+");
fseek(fp, 0, SEEK_END);
ind.deslocamento = ftell(fp)-comp_reg;

//printf("deslocameeeento: %d\n",ind.deslocamento);
fclose(fp);

//atualiza num_reg
    char buffer3[100];
    fp_indice = fopen("indice_desordenado.bin","rb+");
    memcpy(&(buffer3[0]), &num_reg, sizeof(int));
    fwrite(&buffer3,sizeof(char),sizeof(int),fp_indice);
    fclose(fp_indice);


   insere_ordenado_indice(a.RA,ind.deslocamento);
  //imprime_indice();

}

//busca pelo RA busca(4,&posicao)
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

            //recupera_registro(d.deslocamento);
            //printf("chegou chegou\n" );
            fclose(fp);
            return d.deslocamento; //n conta comp reg


    }

   *deslocamento_indice = *deslocamento_indice + 3*sizeof(int); //comp_reg, ra, deslocamento

  } //end do for


  fclose(fp);
  return -1;//nao encontrado

}
Aluno* recupera_registro(int deslocamento){

  Aluno aluno;
  char buffer2[100];
  int pos1=0, pos2;
  FILE *fp = fopen("dados.bin","r+");
  fseek(fp, deslocamento, SEEK_SET);
  fread(buffer2,sizeof(char),BSIZE,fp);

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
  //printf("---->num_reg indice %d\n",num_reg);

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
     //printf("r%d\n",r); //4444
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

  deslocamento = deslocamento + comp_reg+sizeof(int);
}

  if (pos1>0) {
    //printf("final %s %d %d\n",buffer,strlen(buffer),pos1);
    fwrite(buffer,sizeof(char),pos1,fp);
  }
 fclose(fp);
imprime_indice();

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
    fp_indice = fopen("indice_desordenado.bin","rb+");
    fseek(fp_indice, 0, SEEK_END);

    tam_arquivo = ftell(fp_indice); //Tamanho do Arquivo
    //if (tam_arquivo>100) return;

    fim = tam_arquivo;  //O fim recebe o tamanho do arquivo

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

}


int buscaPosicao(int RA)
{
  //return -1;
    char buffer_busca[100];

   //fim, tamanho arq, fim
    int tam_arquivo2, fim2;
    FILE *fp4;
    fp4 = fopen("indice_desordenado.bin","rb");
    if(fp4 == NULL)
    {
        return -1;
    }
    fseek(fp4, 0, SEEK_END);
    tam_arquivo2 = ftell(fp4);
    fim2= tam_arquivo2;
    fseek(fp4, 0, SEEK_SET);

    int RA_lido, bloco=0, tam_buffer, pos8=0, pos_correta, tam_registro;

    pos8 += sizeof(int);

    while(tam_arquivo2 != 0)
    {
        if(tam_arquivo2 >= BSIZE)
        {
            tam_buffer = BSIZE;
        }
        else
        {
            tam_buffer = tam_arquivo2;
        }
        fseek(fp4, (BSIZE*bloco), SEEK_SET);
        fread(&(buffer_busca), sizeof(char), tam_buffer, fp4);

        while(pos8 < tam_buffer)
        {
            pos_correta = 0;
            // Verifico se posso ler NUM_TOTAL_BYTES
            if(pos8 > BSIZE-4)
            {
                pos_correta = pos8;

                fseek(fp4,ftell(fp4)-(BSIZE-pos8),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
                fread(buffer_busca,sizeof(char),BSIZE,fp4);        //carrega o proximo registro desde o seu inicio
                pos8 = 0;
            }

            memcpy(&tam_registro, &(buffer_busca[pos8]), sizeof(int));
            memcpy(&RA_lido, &(buffer_busca[pos8+4]), sizeof(int));

            if(RA_lido > RA)
            {
                fclose(fp4);
                return ((BSIZE*bloco) + pos_correta + pos8);
            }
            pos8 += tam_registro + pos_correta+4;
        }
        if(pos8 >= BSIZE) pos8 -= BSIZE;
        bloco++;
        tam_arquivo2 -= tam_buffer;
    }
    fclose(fp4);
    return -1;
}


void remove_logic(int ra){

   int deslocamento_indice;
   int deslocamento = buscar_indice(ra,&deslocamento_indice); //comp_reg/ra/int
   if (deslocamento==-1){

     return;
     }
 Aluno *x = recupera_registro(deslocamento);
   FILE *fp=fopen("dados.bin","rb+");

  //printf("x->ra %d\n",x->RA);
  x->RA = -1;

  //printf("%d %s\n",x->RA,x->primeiro_nome );
 int comp_reg =
      sizeof(int) + // ra
      strlen(x->primeiro_nome)*(sizeof(char)) + // nome1
      1*sizeof(char) + // #
      strlen(x->ultimo_nome)*(sizeof(char)) + // nome2
      1*sizeof(char) + // #
      sizeof(float) + sizeof(float) + sizeof(float); // nota1 nota2 nota3
      //int j = -1;
      //printf("++%d++\n", deslocamento);

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
fwrite((buffer_aux),sizeof(char),pos_aux,fp);
fclose(fp);

FILE *fp_indice =fopen("indice_desordenado.bin","rb+");
char buffer_indice1[sizeof(int)*3];
int pos0=0;

ancora m;
int comp_reg_indice=0;

fseek(fp_indice,deslocamento_indice-3*sizeof(int),SEEK_SET); //-3*sizeof int pq estava modificando o pro
fread(&(buffer_indice1[pos0]),sizeof(char),sizeof(int)*3,fp_indice);     /* le BSIZE bytes do arquivo para buffer*/

//comp_reg
memcpy(&comp_reg_indice,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);
//printf("comp_reg_indice: %d\n",comp_reg_indice);
//ra
memcpy(&m.RA,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);

//deslocamento
memcpy(&m.deslocamento,&(buffer_indice1[pos0]),sizeof(int));
pos0+=sizeof(int);

//memcpy(&(buffer_indice[pos1_indice]),&c[r].RA,sizeof(int));
m.RA= -1;
memcpy(&(buffer_indice1[4]),&m.RA,sizeof(int));

fwrite(buffer_indice1,comp_reg_indice,1,fp_indice);
fclose(fp_indice);
  //imprime_dados();
  //imprime_indice();

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
    //printf("---->num_reg000 %d\n",num_reg);

    int chave;
    char buffer3[100];
   //to sobrescrevendo num reg

  FILE *fp_novo = fopen("dados2.bin","wb");
  fwrite(&(buffer[0]),sizeof(char),sizeof(int),fp_novo);
  fclose(fp_novo);

  FILE *fp_indice_novo = fopen("indice_desordenado.bin","wb");
  fwrite(&(buffer[0]),sizeof(char),sizeof(int),fp_novo);
   fclose(fp_indice_novo);
   int desloc = sizeof(int);

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
   // printf("%d\n---------------", b.RA);
    if (b.RA!=-1){

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
        //Cria ancora no indice para registro
        insere_ordenado_indice(b.RA,desloc);
        desloc = desloc + comp_reg + sizeof(int);

    }
    else{
            int num_reg2;
            //atualiza num_reg e grava no arquivo /*AQUI*/
            pos3=0;
            fseek(fp_novo,0,SEEK_SET);
           fread(&(buffer3[pos3]),sizeof(char),sizeof(int),fp_novo);
           memcpy(&(num_reg2), &buffer3[0], sizeof(int));
           //printf(" aca->%d<-aca\n",num_reg2 );
            num_reg2--;
              fseek(fp_novo, 0, SEEK_SET);
              fwrite(&num_reg2,sizeof(int),1,fp_novo);
              fp_indice_novo = fopen("indice_desordenado.bin","rb+");

               memcpy(&(buffer3[0]), &num_reg2, sizeof(int));
             fseek(fp_indice_novo, 0, SEEK_SET);
              fwrite(&buffer3,sizeof(char),sizeof(int),fp_indice_novo);
              fclose(fp_indice_novo);
    }

}
  fclose(fp_novo);
  fclose(fp);
  //fclose(fp_indice_novo);

  //Exclui outro arquivo
 remove("dados.bin");
    char arq_antigo[] = "dados2.bin";
    char arq_novo[] = "dados.bin";
    //Renomeia o auxiliar
    rename(arq_antigo, arq_novo);

  //imprime_dados();
  //imprime_indice();
}
