/****************************************************/
/* Aluno:    Charles Washington de Aquino dos Santos*/
/* Avaliação 04: Trabalho Final						*/					
/* UDB-255 - 2015-1 - Prof. Daniel Ferreira 		*/ 	
/* Compilador: Gcc versão 4.8.2						*/
/****************************************************/

#include "funcsLimiariza.h"

void carregarIMG(FILE *ptr, struct image *IMG){
unsigned char buffer[1];
char *aux;
int pos = 0, p, i2, tamCab = 0;
aux = malloc(1);
buffer[0] = 1;
	for (p = 0; p < 4; p++){
		while (buffer[0] != 10){
			fread(buffer,1,1,ptr);
			if (buffer[0] == ' '){
			buffer[0] = 10;
			}
			tamCab++;
			aux[pos] = buffer[0];
			if (buffer[0] != 10){
				pos++;
				aux = realloc(aux, sizeof(aux)+1);
			}
		}
		if (p == 0){
			for (i2 = 0; i2 < (pos); i2++){
				IMG->cod[i2]=aux[i2];
			}
			IMG->cod[i2] = 0;
		}
		if (p == 1){
			IMG->nC = atoi(aux);
		}
		if (p == 2){
			IMG->nL = atoi(aux);
		}
		if (p == 3){
			IMG->maxVal = atoi(aux);
		}
		pos=0;
		buffer[0] = 1;
		aux = realloc(aux, 1);
	}
IMG->tamCab = tamCab;
}

void alocaMem(int numLinhas, int numColunas, struct janela *JAN){ //aloca memória
	JAN->dados = calloc(numLinhas*numColunas,sizeof(char));
	JAN->dadosFL = calloc(numLinhas*numColunas,sizeof(float));
}

void retornaSubMat(int numLinhas, int numColunas, FILE *arquivo, int posI, int posJ, struct image *IMG, struct janela *JAN){ //essa função retorna uma submatriz e lida caso a amostra precise ser maior que a quantidade de itens restando na matriz imagem.
int i; unsigned char buffer[1]; int posNoArq;

	if ((posI+(numLinhas)) > IMG->nL){ //esse IF e o debaixo impedem que a janela estoure o tamanho da imagem (falha de segmentação).
		posI = posI-((posI+numLinhas)-IMG->nL);
	}
	if ((posJ+(numColunas)) > IMG->nC){
		posJ = posJ - ((posJ+numColunas)-IMG->nC);
	}

	posNoArq = IMG->tamCab+(posI*IMG->nC)+posJ;

	for (i = 0; i < (numLinhas*numColunas); i++){
		fseek(arquivo, posNoArq, SEEK_SET);
		fread(buffer, 1, 1, arquivo);
		JAN->dados[i] = buffer[0];
		posNoArq++;

		if ((i+1)%numColunas==0){
		posNoArq = posNoArq + (IMG->nC-numColunas); //quebra de acordo com o número de colunas
		fseek(arquivo, posNoArq, SEEK_SET);
		}

	}
JAN->nLinhas = numLinhas;
JAN->nColunas = numColunas;
}

void escreveSubMat(int numLinhas, int numColunas, FILE *NovoArquivo, int posI, int posJ, struct image *IMGiLAB, struct janela *JANiLAB, struct janela *JANiRGB, float TXiLAB, float TXiRGB){
unsigned char x; int posNoArq, i;
	if ((posI+(numLinhas)) > IMGiLAB->nL){	//esse IF e o debaixo impedem que a janela estoure o tamanho da imagem (falha de segmentação).
		posI = posI-((posI+numLinhas)-IMGiLAB->nL);
	}
	if ((posJ+(numColunas)) > IMGiLAB->nC){
		posJ = posJ - ((posJ+numColunas)-IMGiLAB->nC);
	}
	posNoArq = IMGiLAB->tamCab+(posI*IMGiLAB->nC)+posJ; //vai até a posição necessária no arquivo
	for (i = 0; i < (numLinhas*numColunas); i++){
		fseek(NovoArquivo, posNoArq, SEEK_SET); //define a posição

		if (JANiLAB->dadosFL[i] >= TXiLAB || JANiRGB->dadosFL[i] >= TXiRGB){ //"sobreposição" das imagens iRGB e iLAB limiarizadas ocorre aqui
			x = 255;
		}
		else
		{
			x = 0;
		}

		fwrite(&x, 1, 1, NovoArquivo); //escrita no arquivo de imagem gerado
		posNoArq++;

		if ((i+1)%numColunas==0){//controla a quebra da linha
		posNoArq = posNoArq + (IMGiLAB->nC-numColunas);
		fseek(NovoArquivo, posNoArq, SEEK_SET);
		}

	}
}



void normaliza(struct janela *JAN, int maxVal){
	int i; float aux;
	for (i = 0; i < (JAN->nLinhas*JAN->nColunas); i++){
		aux = JAN->dados[i];
		JAN->dadosFL[i] = aux / maxVal;
	}
}

float retornaMediaFL(struct janela *JAN){
int i;
float medMatriz = 0;
	for (i = 0; i < (JAN->nLinhas*JAN->nColunas); i++){
			medMatriz += JAN->dadosFL[i];
	}
medMatriz = medMatriz/(JAN->nLinhas * JAN->nColunas);
return medMatriz;
}

float retornaDesvPadrao(struct janela *JAN){
	int i;
	float media = retornaMediaFL(JAN);
	float somaMatriz = 0 ;
	for (i=0; i < (JAN->nLinhas*JAN->nColunas); i++){
     		somaMatriz =  somaMatriz + ((JAN->dadosFL[i] - media) * (JAN->dadosFL[i] - media));
	}
	somaMatriz = somaMatriz/(JAN->nLinhas * JAN->nColunas) ;
	somaMatriz =  sqrt(somaMatriz);
	return somaMatriz;
}

float retornaTXEQ3(float media, float desvPadrao, float paramK){
	return (media*(1+(paramK*((desvPadrao/0.5)-1))));
}

float retornaTXEQ4(float media, float desvPadrao, float paramP, float paramK){
	return (media*(1+paramP*exp(-10*media)+paramK*((desvPadrao/0.5)-1)));
}

void escreveARQ(FILE *novoArquivo, struct image *IMG){
	int aux=10;
	fseek(novoArquivo, 0, SEEK_SET);
	fwrite(IMG->cod,2,1,novoArquivo);
	fwrite(&aux,1,1,novoArquivo);
	fprintf(novoArquivo, "%d", IMG->nC);
	fwrite(&aux,1,1,novoArquivo);
	fprintf(novoArquivo, "%d", IMG->nL);
	fwrite(&aux,1,1,novoArquivo);
	fprintf(novoArquivo, "%d", IMG->maxVal);
	fwrite(&aux,1,1,novoArquivo);
}

