#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "gerador_de_dados.h"

#define BSIZE 100

void imprime_dados();
void tempoAdicionarAluno();


typedef struct _aluno {
    int RA;
    char primeiro_nome[20];
    char ultimo_nome[20];
    float nota[3];
} Aluno;


/*Variaveis globais*/
int   pos1,pos2,r, vetBlocos[20], blocos;
FILE *fp;



void imprimeBusca(int deslocamento)
{

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

   printf(" Registro Busca:  %d %s %s %.2f %.2f %.2f\n",aluno.RA,aluno.primeiro_nome,aluno.ultimo_nome,aluno.nota[0],aluno.nota[1],aluno.nota[2]);
   return ;
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
    
    int RA_lido, bloco=0, tam_buffer, pos1=0, pos_correta, tam_registro, posVet; 
	i=0;
    
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
            
            memcpy(&tam_registro, &(buffer[pos1]), sizeof(int)); //le o tamanho do registro
            memcpy(&RA_lido, &(buffer[pos1+4]), sizeof(int)); // desloca da posicao de tam_registro para a do ra, pos1+4 = pos1+sizeof(int)

            if(RA_lido != 0 && RA_lido == RA) //Procura o ra
            {
                printf("\nRA Encontrado!!\n");
                pos2=((BSIZE*bloco) + pos_correta + pos1);
                imprimeBusca(pos2);
                fclose(fp);
                return (pos2);

            }
            if(RA_lido > RA)
            {
                fclose(fp);
                return ((BSIZE*bloco) + pos_correta + pos1);
            }
            pos1 += tam_registro + pos_correta + 4; //+4 do com_reg
        }
        if(pos1 >= BSIZE) 
		pos1 -= BSIZE;
        bloco++;
        tam_arquivo -= tam_buffer;
    }
    fclose(fp);
    return -1;
}



void insereOrdenado(Aluno a)
{
	fp = fopen("arquivo_ordenado.bin","rb+"); //Abre o arquivo, para a leitura
	
	char buffer[BSIZE];
	int   num_reg, aux, tam_arquivo, tam_reg,i, fim, inicio, quant_desl,desl, comp_reg, blocos=0, dif=0;

	pos1 = sizeof(int);
	fread(&(buffer[pos1]),sizeof(int),sizeof(int),fp);     
	memcpy(&num_reg,&(buffer[pos1]),sizeof(int));     // le a quantidade de registros
	
    num_reg++;
	    
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
	

    if(inicio == -2) //RA Repeitido
        return;
	
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
           // printf("\n\n==> Quantidade a deslocar: %d, inicio %d fim %d\n\n", quant_desl, inicio, fim);
            
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

    fseek(fp, 0, SEEK_SET);
    fwrite(&num_reg,sizeof(char),1,fp);

	fclose(fp);

			  
	return ;
	
}


//coloca o marcador -1 na posicao removida
void remocao(int chave)
{
	char buffer[BSIZE];
	FILE *arq;
	arq = fopen("arquivo_ordenado.bin","rb+"); //Abre o arquivo, para a leitura	
		
	int  posicao, marcador=-1, num_reg;
    
    posicao=buscaPosicao(chave);
   
    if(posicao==-1) //pecorreu o arquivo todo e nao encontrou 
    {
		printf("\nFalha na remoção arquivo não encontrado!\n");
		fclose(arq);
		return; 
		
	}
	
	posicao += sizeof(int);
	int dif;
	fseek(arq, posicao, SEEK_SET);
	fread(&(buffer[0]),sizeof(int),sizeof(int),arq);     
	memcpy(&(buffer[0]),&marcador,sizeof(int)); 
	fseek(arq, posicao, SEEK_SET);
	//fread(&buffer, sizeof(int), sizeof(int), fp);
	    //printf("\n----------------------------->Posicao %d conteudo %d\n", posicao, buffer[0]);
	    
	fwrite(&buffer[0],sizeof(int),sizeof(int),arq); //RA= -1 - excluido ignora

	fclose(arq);
	return; //Ultima posição
}





void reorganizacao_dados()
{
	char buffer[BSIZE];
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
    //printf("---->num_reg000 %d\n",num_reg);


    int chave;
    char buffer3[100];
   //to sobrescrevendo num reg

  FILE *fp_novo = fopen("dados2.bin","wb");
  fwrite(&(buffer[0]),sizeof(char),sizeof(int),fp_novo);
  fclose(fp_novo);


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
           //printf(" aca->%d<-aca\n",num_reg2 );
            num_reg2--;

         fseek(fp_novo, 0, SEEK_SET);
         fwrite(&num_reg2,sizeof(int),1,fp_novo);


    }
}
  fclose(fp_novo);
  fclose(fp);

//Exclui outro arquivo
remove("arquivo_ordenado.bin");
char arq_antigo[] = "dados2.bin";
char arq_novo[] = "arquivo_ordenado.bin";
//Renomeia o auxiliar
rename(arq_antigo, arq_novo);
printf("\nReorganizado:\n");
imprime_dados();

}





void imprime_dados()
{
	char buffer[BSIZE];
    int num_reg=0;
    int comp_reg = 0;
    Aluno b;
/* Simulando a leitura do arquivo*/
    FILE *fp;
    fp = fopen("arquivo_ordenado.bin","r+");

    pos1 = 0;
    fread(&(buffer[pos1]),sizeof(char),BSIZE,fp);     /* le BSIZE bytes do arquivo para buffer*/

    memcpy(&num_reg,&(buffer[pos1]),sizeof(int));
        // le um inteiro dop buffer para a variavel num_reg
    pos1 += sizeof(int);                              // atualiza posicao de leitura
    //printf("---->num_reg %d\n",num_reg);

    int chave;

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

  printf("\n\n");
  fclose(fp);

}


void tempoRemocao(){
 
         
    clock_t t0, tf;
    double tempo_gasto, somatorio=0;
    //t0 = clock();
    int i;
    //char i_char[10];
	srand((unsigned)time(NULL));
    for(i = 0 ; i<50 ; i ++){
        //sprintf(i_char , "%d" , i+1);
        t0 = clock();
	remocao(1+rand()%TAM_MAX);
 
        tf = clock();
        tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
        somatorio += tempo_gasto;
        //printf("%lf\n", tempo_gasto);
    }
 
  //printf("%.7lf\n", somatorio/i);
 
}

void tempoReorganizacao(){
 
         
    clock_t t0, tf;
    double tempo_gasto, somatorio=0;
    //t0 = clock();
    int i;
    //char i_char[10];
	srand((unsigned)time(NULL));
    for(i = 0 ; i<50 ; i ++){
        //sprintf(i_char , "%d" , i+1);
        t0 = clock();
	reorganizacao_dados();
 
        tf = clock();
        tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
        somatorio += tempo_gasto;
        //printf("%lf\n", tempo_gasto);
    }
 
  printf("%.7lf\n", somatorio/i);
 
}


void tempoBuscarAluno(){
 
         
    clock_t t0, tf;
    double tempo_gasto, somatorio=0;
    //t0 = clock();
    int i;
    //char i_char[10];
	srand((unsigned)time(NULL));
    for(i = 0 ; i<50 ; i ++){
        //sprintf(i_char , "%d" , i+1);
        t0 = clock();
	buscaPosicao(1+rand()%TAM_MAX);
 
        tf = clock();
        tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
        somatorio += tempo_gasto;
        //printf("%lf\n", tempo_gasto);
    }
 
  printf("%.7lf\n", somatorio/i);
 
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

                Aluno a;
                a.RA = criaRA()*( rand() % TAM_MAX);
                strcpy(a.primeiro_nome,nome);
                strcpy(a.ultimo_nome,ult_nome);
                a.nota[0] = atof(nota1);
                a.nota[1] = atof(nota2);
                a.nota[2] = atof(nota3);
                insereOrdenado(a);

                //imprime_dados();

            }
            else if (linha[0] == 'b'){
                printf("Busca\n");

                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                 buscaPosicao(atoi(registro));

            }
            else if (linha[0] == 'r'){
                printf("Remocao\n");
                comando= (char*)strtok(linha, " ");
                registro= (char*)strtok(NULL, " ");
                remocao(atoi(registro));
                imprime_dados();

            }
            if (linha[0] == 'R'){
                printf("Reorganização\n");
                reorganizacao_dados();
                imprime_dados();


            }

}
}




int main()
{

	clock_t t0, tf;
    double tempo_gasto, somatorio=0;
  
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
	fwrite(&pos1, sizeof(int), 1, fp);
	
	fclose(fp);
  
  	for (r=0; r < TAM_MAX; r++) {
	  	//printf("\n********************************INSERIR %d \n", a[r].RA);
		t0 = clock();
	  	insereOrdenado(a[r]);  
       		 tf = clock();
        	tempo_gasto = ( (double) (tf - t0) ) / CLOCKS_PER_SEC;
        	somatorio += tempo_gasto;
	  
  	}
	imprime_dados();  
/*
    b.RA = 64545;
    sprintf(b.primeiro_nome,"Paula"); 
    sprintf(b.ultimo_nome,"Sinval"); 
    b.nota[0]=8.9;
    b.nota[1]=7.5;
    b.nota[2]=4.5;
    insereOrdenado(b);
    imprime_dados();
*/
    //buscaPosicao(5);
    //remocao(8);
    //remocao(2);
    //imprime_dados();
    //reorganizacao_dados();



    //le_script(); 

  return 0;
}
