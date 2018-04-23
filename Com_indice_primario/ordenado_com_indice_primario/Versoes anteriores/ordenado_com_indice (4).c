
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
int blocos, vetBlocos[40]; //MUDAR VALOR AQUI
/*Indice*/
char buffer_indice[100];
FILE *fp_indice;
int deslocamento_total_indice=0; //por conta do num_reg?
/*Assinaturas das fuñçoes*/
void insere_ordenado_indice(int RA, int deslocamento);
void imprime_indice();
void cria_arquivos(Aluno *a, ancora *c);
int buscaPosicao(int RA);
void insereOrdenado(Aluno a);
void reorganizacao_dados();
void cria_indice();
void imprime_dados();
Aluno* recupera_registro(int deslocamento);
ancora* busca_indice(int chave);
void remove_logic(int ra);
void reorganizacao_dados();
//FAZER
//Ponteiro é por bloco
//INSERIR NO INDICE E NO BLOCO

//salvar vetor de blocos em um txt

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

//Inserção
//Busca - Cai no bloco e tem q achar o registro desejado
//Remoção
//Reorganização



void insere_ordenado_indice(int RA, int deslocamento){

    FILE *fp_indice;
    int   num_reg_indice, pos1_indice,aux, tam_arquivo, tam_reg,i, fim, inicio, quant_desl,desl, comp_reg_indice, blocos=0, dif=0;
    char buffer_indice[100];
    pos1_indice = sizeof(int);

    fp_indice = fopen("indice_ordenado.bin","rb"); //Abre o arquivo, para a leitura



    fread(&(buffer_indice[pos1_indice]),sizeof(int),sizeof(int),fp_indice);
    memcpy(&num_reg_indice,&(buffer_indice[pos1_indice]),sizeof(int));     // le a quantidade de registros
    comp_reg_indice = sizeof(int)*2;
    tam_reg=(comp_reg_indice + sizeof(int));
   fclose(fp_indice);

    inicio= buscaPosicao(RA); // recebe a o valor de onde deve começar a movimentação dos registros'
    //printf("posicao->%d\n", inicio); //12345
    //printf("Ra: %d\n",RA );
    fp_indice = fopen("indice_ordenado.bin","rb+");


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
            //printf("desl %d\n",desl );
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







void insereOrdenado(Aluno a)
{
    fp = fopen("arquivo_ordenado.bin","rb+"); //Abre o arquivo, para a leitura

    char buffer[BSIZE];
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
    //printf("inicio %d fim %d ", inicio, fim);


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

            //salva no indice


            return;

}





void imprime_indice(){

FILE *fp;
int num_reg, comp_reg;
ancora d;
/* Simulando a leitura do arquivo*/ //INDICE
  fp = fopen("indice_ordenado.bin","rb+");

  pos1 = 0;
  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //
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
    char buffer[BSIZE];
    int tam_arquivo, fim, i=0;
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
    vetBlocos[i]=pos1;
    i++;

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

            memcpy(&tam_registro, &(buffer[pos1]), sizeof(int)); //le o tamanho do registro
            memcpy(&RA_lido, &(buffer[pos1+4]), sizeof(int)); // desloca da posicao de tam_registro para a do ra, pos1+4 = pos1+sizeof(int)

            if(RA_lido > RA)
            {
                fclose(fp);
                return ((BSIZE*bloco) + pos_correta + pos1);
            }
            pos1 += tam_registro + pos_correta + 4;
        }
        vetBlocos[i]=(((BSIZE*bloco) + pos1)-tam_registro - 4); //PQ -4?
                i++;
                //anterior mais somas
        if(pos1 >= BSIZE)
            pos1 -= BSIZE;
        bloco++;
        tam_arquivo -= tam_buffer;
    }
    fclose(fp);
    return -1;
}



int main()
{

    int   comp_reg;
    int num_reg= TAM_MAX ;/*editar na biblioteca*/
    Aluno a[num_reg], b; //cria o vetor do tamanho de numero de registros
    char buffer[BSIZE];


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


    fp = fopen("arquivo_ordenado.bin","wb");  //Cria o arquivo no caso de não existir ainda

    pos1 = 0;

    fseek(fp, 0, SEEK_SET);
    fwrite(&num_reg, sizeof(int), 1, fp);

    fclose(fp);




  for (r=0; r < num_reg; r++) {
          //printf("\n********************************INSERIR %d \n", a[r].RA);
      insereOrdenado(a[r]);

  }
  //  printf("\nVETOR: ");

    for(r=0;r<10;r++)
    {
       // printf("%d ",vetBlocos[r]);
    }

   cria_indice(); //TEM Q CHAMAR P FUNCIONAR O INDICE
    //imprime_indice();



 //remove_logic(5);

//imprime_indice();
//temporeoganiza();

//printf("**************************************\n");
//remove_logic(10);
//reorganizacao_dados();
//imprime_dados();
//imprime_indice();
 //busca_indice(114);

  return 0;
}





void remove_logic(int chave){

      int comp_reg;
  int num_reg;
  ancora c,d, ant;

 FILE *fp_indice = fopen("indice_ordenado.bin","rb+");

  pos1 = 0;

  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp_indice);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("---->num_reg %d\n",num_reg);
  int deslocamento_total = sizeof(int);

  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp_indice, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp_indice,ftell(fp_indice)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp_indice);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp_indice,ftell(fp_indice)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp_indice);
    }

    /* copia demais campos do registro*/

    // ra
    memcpy(&d.RA,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    //deslocamento
    memcpy(&d.deslocamento,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
      int achou=0, fim_bloco=0,desl=0;
    if(d.RA>chave) {
        //printf("ENTROU NO IF************");
        //printf("d.ra->%d",d.RA);


            Aluno aluno;
                char buffer2[100];
                int pos1=0, pos2;
                FILE *fp = fopen("arquivo_ordenado.bin","rb+");
                fseek(fp, ant.deslocamento, SEEK_SET);
                fread(buffer2,sizeof(char),d.deslocamento-ant.deslocamento,fp);
/*Guarda no arquivo-> num_reg  + cada reg-> [com_reg,ra,nome1,nome2,notas*3]*/



                    while((!achou)&&(!fim_bloco)){
                         if(desl>=(d.deslocamento-ant.deslocamento)) {
                            fim_bloco=1;
                        }
                            else{
                             //comp_reg
                             int comp_reg =0;
                            memcpy(&comp_reg,&(buffer2[pos1]),sizeof(int));
                            pos1 += sizeof(int);

                            // ra
                            memcpy(&aluno.RA,&(buffer2[pos1]),sizeof(int));
                            pos1 += sizeof(int);
                                if(aluno.RA!=chave){
                                    desl = desl + comp_reg + sizeof(int);//sizeof do comp_reg
                                    pos1=pos1 + comp_reg-sizeof(int);
                                    continue;
                                }
                                else{



                                    //Escreve -1 no arquivo
                                    FILE *fp = fopen("arquivo_ordenado.bin","rb+");

                                        char buffer23[100];
                                        int n_ra=-1;
                                        memcpy(&buffer23[0],&(n_ra),sizeof(int));
                                        fseek(fp, ant.deslocamento+desl+ sizeof(int), SEEK_SET);
                                        fwrite((&buffer23[0]),sizeof(char),sizeof(int),fp);// NAO TA APAGANDO
                                        achou = 1;
                                        fclose(fp);

                                        achou = 1;

                                    }
                            }
                        }//end while





            }

  //printf(":DDDDD Registro recuperado:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
  //else printf("naaaaaaaaaao achou man\n");

    else if(d.RA==chave){//Exclui mas deixa lá, nao interfere na busca pq pega o ant


                        FILE *fp = fopen("arquivo_ordenado.bin","rb+");

                        char buffer23[100];
                        int n_ra=-1;
                        memcpy(&buffer23[0],&(n_ra),sizeof(int));
                        fseek(fp, d.deslocamento + sizeof(int), SEEK_SET);
                        fwrite((&buffer23[0]),sizeof(char),sizeof(int),fp);// NAO TA APAGANDO
                        achou = 1;
                        fclose(fp);
                       // FILE *fp_indice = fopen("indice_ordenado.bin","rb+");


                        n_ra = -1;
                        memcpy(&buffer23[0],&(n_ra),sizeof(int));
                        //printf("deslocamento_total %d\n", deslocamento_total);
                        fseek(fp_indice, deslocamento_total+sizeof(int), SEEK_SET);//deesloc?
                        fwrite((buffer23),sizeof(char),sizeof(int),fp_indice);
                        break;

            }
deslocamento_total  = deslocamento_total + 3*sizeof(int);


ant.RA = d.RA;
ant.deslocamento = d.deslocamento;

  } //end do for
  fclose(fp_indice);


  pos1=0;
 // return -1;//nao encontrado
 //fclose(fp_indice);
  //printf("remocao\n");
  imprime_dados();
  imprime_indice();
   // printf("fim remocao\n");

}


void reorganizacao_dados(){

    int num_reg=0;
    int comp_reg = 0;
    Aluno b;
/* Simulando a leitura do arquivo*/
    FILE *fp;
    fp = fopen("arquivo_ordenado.bin","rb+"); ///Modificar nomes depois

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
    //printf("---->num_reg000 %d\n",num_reg);


    int chave;
    char buffer3[100];
   //to sobrescrevendo num reg

  FILE *fp_novo = fopen("arquivo_ordenado2.bin","wb");
  fwrite(&(buffer[0]),sizeof(char),sizeof(int),fp_novo);
  fclose(fp_novo);

  char* nomesAqui = geraNomes();

  fp_novo = fopen("arquivo_ordenado2.bin","rb+");
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
    //printf("%d\n", b.RA);
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
          // printf(" aca->%d<-aca\n",num_reg2 );
            num_reg2--;

         fseek(fp_novo, 0, SEEK_SET);
         fwrite(&num_reg2,sizeof(int),1,fp_novo);


    }

}
  fclose(fp_novo);
  fclose(fp);

  //Exclui outro arquivo
 remove("arquivo_ordenado.bin");
    char arq_antigo[] = "arquivo_ordenado2.bin";
    char arq_novo[] = "arquivo_ordenado.bin";
    //Renomeia o auxiliar
    rename(arq_antigo, arq_novo);
  cria_indice(); //Recria o indice
  imprime_indice();
  //imprime_dados();
}




void cria_indice(){

    int num_reg=0;
    int anterior;
    int comp_reg = 0;
    Aluno b;
    int p_bloco=0;
    int vetor_blocos[TAM_MAX/2];
    int num_bytes=sizeof(int);
    int i, num_angoras=0;
    for(i=0;i<(TAM_MAX/2);i++){
        vetor_blocos[i]=-1;
    }
    FILE *fp;
    fp = fopen("arquivo_ordenado.bin","rb+");

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
    //printf("---->num_reg %d\n",num_reg);

    //CRIA ARQUIVO E GUARDA NUM_REG NELE
    char buffer3[100];
     FILE *fp_indice = fopen("indice_ordenado.bin","wb");
     fclose(fp_indice);

    memcpy(&(buffer3[0]), &num_reg, sizeof(int));
    fp_indice = fopen("indice_ordenado.bin","rb+");
    fseek(fp_indice, 0, SEEK_SET);
    fwrite(&buffer3,sizeof(char),sizeof(int),fp_indice);
    fclose(fp_indice);

    int chave;

  vetor_blocos[0]=sizeof(int);
  anterior = 1;
  for (r=0; r < num_reg; r++) {
    // Verifico se posso ler NUM_TOTAL_BYTES

    if (pos1 > BSIZE-4) {
      // fseek(fp, posicao corrente - (offset), a partir do início do arquivo)
        int mm = ftell(fp)-(BSIZE-pos1);
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);        //carrega o proximo registro desde o seu inicio
      num_bytes = 0;
      p_bloco++;
      vetor_blocos[p_bloco] = mm;
      anterior=1;
      num_angoras++;
      //printf("mmmmmmmmmmmm\n");
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      int nn = ftell(fp)-(BSIZE-pos1)-sizeof(int);
      fseek(fp,ftell(fp)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp);
      num_bytes = 0;
      p_bloco++;
      vetor_blocos[p_bloco] = nn;
      anterior=1;
      num_angoras++;
      //printf("-----nnnnnnnn\n");
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

    //if ((num_bytes+comp_reg+sizeof(int))>BSIZE)
    //{
      //  blocos++;
       // num_bytes = 0; //zero ou comp_reg?
    //}
    //Atualiza num_bytes
    num_bytes = num_bytes + comp_reg + sizeof(int);

    /* imprime o registro lido*/
    if (anterior) insere_ordenado_indice(b.RA, vetor_blocos[p_bloco]);
    anterior = 0;
    //printf("%d %6d %s %s %.2f %.2f %.2f\n",
    // comp_reg,b.RA,b.primeiro_nome,b.ultimo_nome,b.nota[0],b.nota[1],b.nota[2]);
  }

      //Atualiza num_reg
    char buffer4[100];
    fp_indice = fopen("indice_ordenado.bin","rb+");
    fread(&(buffer4[0]),sizeof(char),sizeof(int),fp_indice);
    int num_reg_aux;
    memcpy(&(num_reg_aux), &buffer4[0], sizeof(int));
    num_reg_aux=num_angoras;

    memcpy(&(buffer4[0]), &num_reg_aux, sizeof(int));
    fseek(fp_indice, 0, SEEK_SET);
    fwrite(&buffer4,sizeof(char),sizeof(int),fp_indice);

    fclose(fp_indice);


  for(i=0;i<(TAM_MAX/2);i++){
       //printf("%d\n",vetor_blocos[i] );
    }
  fclose(fp);
//printf("/*******************/\n");
//recupera_registro(86);

}


Aluno* recupera_registro(int deslocamento){

  Aluno aluno;
  char buffer2[100];
  int pos1=0, pos2;
  FILE *fp = fopen("arquivo_ordenado.bin","r+");
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

   //printf(" Registro recuperado:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
   return m;

}


ancora* busca_indice(int chave){
  int comp_reg;
  int num_reg;
  ancora c,d, ant;

 FILE *fp_indice = fopen("indice_ordenado.bin","rb+");

  pos1 = 0;

  fread(&(buffer[pos1]),sizeof(char),BSIZE,fp_indice);     /* le BSIZE bytes do arquivo para buffer*/

  memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le um inteiro dop buffer para a variavel num_reg
  pos1 += sizeof(int);                              // atualiza posicao de leitura
  //printf("---->num_reg %d\n",num_reg);

  for (r=0; r < num_reg; r++) {

    // Verifico se posso ler NUM_TOTAL_BYTES
    if (pos1 > BSIZE-4) {
      // fseek(fp_indice, posicao corrente - (offset), a partir do início do arquivo)
      fseek(fp_indice,ftell(fp_indice)-(BSIZE-pos1),SEEK_SET);  //posiciona ponteiro do arquivo no inicio do proximo registro
      fread(buffer,sizeof(char),BSIZE,fp_indice);        //carrega o proximo registro desde o seu inicio
      pos1 = 0;
    }
    // Lendo NUM_TOTAL_BYTES
    memcpy(&comp_reg,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    // Verifico se o registro inteiro esta no buffer
    if (comp_reg > BSIZE-pos1) {
      fseek(fp_indice,ftell(fp_indice)-(BSIZE-pos1),SEEK_SET);
      pos1 = 0;
      fread(buffer,sizeof(char),BSIZE,fp_indice);
    }

    /* copia demais campos do registro*/

    // ra
    memcpy(&d.RA,&(buffer[pos1]),sizeof(int));

    pos1 += sizeof(int);

    //deslocamento
    memcpy(&d.deslocamento,&(buffer[pos1]),sizeof(int));
    pos1 += sizeof(int);
      int achou=0, fim_bloco=0,desl=0;
    if(d.RA>chave) {


            Aluno aluno;
                char buffer2[100];
                int pos1=0, pos2;
                FILE *fp = fopen("arquivo_ordenado.bin","r+");
                fseek(fp, ant.deslocamento, SEEK_SET);
                fread(buffer2,sizeof(char),d.deslocamento-ant.deslocamento,fp);
/*Guarda no arquivo-> num_reg  + cada reg-> [com_reg,ra,nome1,nome2,notas*3]*/



                    while((!achou)&&(!fim_bloco)){
                         if(desl>=(d.deslocamento-ant.deslocamento)) fim_bloco=1;
            else{
                //comp_reg
                int comp_reg =0;
                memcpy(&comp_reg,&(buffer2[pos1]),sizeof(int));
                pos1 += sizeof(int);
                // ra
                memcpy(&aluno.RA,&(buffer2[pos1]),sizeof(int));
                pos1 += sizeof(int);
                //printf("klsadnsadsssssssssss***///-----%d\n",aluno.RA );
                    if(aluno.RA!=chave){
                        desl = desl + comp_reg + sizeof(int);//sizeof do comp_reg
                        pos1=pos1 + comp_reg-sizeof(int);
                        continue;
                    }
                    else{

                        achou = 1;
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
                        achou = 1;
                        continue;
                    }






        }
} //end while
  //printf(":DDDDD Registro recuperado:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
  //else printf("naaaaaaaaaao achou man\n");

     ancora *indic = (ancora*)malloc(sizeof(ancora));
    indic->RA = d.RA;
    indic->deslocamento = d.deslocamento;
   return indic;

    }
    else if(d.RA==chave){
        //printf("passou***\n");
        //recupera_registro(d.deslocamento);
        //printf(":DDDDD Registro recuperado2:  %d\n",d.RA);
        ancora *indic =  (ancora*)malloc(sizeof(ancora));
        indic->RA = d.RA;
        indic->deslocamento = d.deslocamento;

        return indic;
    }
ant.RA = d.RA;
ant.deslocamento = d.deslocamento;

  } //end do for


  fclose(fp_indice);
  pos1=0;
  ancora *indic =  (ancora*)malloc(sizeof(ancora));
    indic->RA =-1;
indic->deslocamento = -1;
  return indic;//nao encontrado

}


void imprime_dados(){

    int num_reg=0;
    int comp_reg = 0;
    Aluno b;
/* Simulando a leitura do arquivo*/
    FILE *fp;
    fp = fopen("arquivo_ordenado.bin","rb+");

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
    //printf("---->num_reg %d\n",num_reg);

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