void adicionarAluno(char *primeiro_nome, char *ultimo_nome, float nota0, float nota1, float nota2){
int comp_reg, num_reg, pos1=0,r;
  Aluno a;
  char buffer[BSIZE];
  int ra;

  // funcao que cria um RA para adicionar ao aluno.
  ra =  criaRA();

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

//busca sequencial
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


//pega dados do txt
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









