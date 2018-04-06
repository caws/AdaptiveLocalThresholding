/****************************************************/
/* Aluno:    Charles Washington de Aquino dos Santos*/
/* Avaliação 04: Trabalho Final						*/					
/* UDB-255 - 2015-1 - Prof. Daniel Ferreira 		*/ 	
/* Compilador: Gcc versão 4.8.2						*/
/****************************************************/

#include "funcsLimiariza.h"

int main(int argc, char *args[]){ //program.o pgmIlab.pgm pgmiRGB.pgm 3/4

if (argc != 4){
	printf("Utilize da seguinte maneira: ./programa imgiLAB.pgm imgiRGB.pgm 3 ou 4\n");
	return 1;
	}

int i, j, linhasJanela, colunasJanela;
FILE *iLAB, *iRGB, *novoArquivo;
float TXiLAB, TXiRGB, paramP = 3.0, paramK = 0.25;

struct image IMGiLAB, IMGiRGB; //struct das imagens originais.
struct janela amostraiLAB, amostraiRGB; //struct onde as amostras iLAB ou iRGB serao armazenadas.
iLAB = fopen(args[1],"rb");
iRGB = fopen(args[2],"rb");
novoArquivo = fopen("imagemLimiarizada.pbm","w+"); //criação da imagem final limiarizada com base na iLAB e iRGB.

printf("Qual a quantidade de linhas da amostra?");
scanf("%d", &linhasJanela);
printf("Qual a quantidade de colunas da amostra?");
scanf("%d", &colunasJanela);
printf("Que valor utilizar em P? (O padrão é 3)\n");
scanf("%f", &paramP);

if (atoi(args[3]) == 4){
	printf("Que valor utilizar em K? (O padrão é 0.25)\n");
	scanf("%f", &paramK);
	}

carregarIMG(iLAB, &IMGiLAB); //essa função obtém dados como como cod da imagem, número de colunas, número de linhas e maxval do arquivo, além de apontar o ponteiro *ptr ao arquivo.
carregarIMG(iRGB, &IMGiRGB);

printf("Tamanho do cabeçalho: %d", IMGiLAB.tamCab);
printf("Numero de linhas: %d\n", IMGiLAB.nL);
printf("Numero de colunas: %d\n", IMGiLAB.nC);

alocaMem(linhasJanela, colunasJanela, &amostraiLAB);
alocaMem(linhasJanela, colunasJanela, &amostraiRGB);

for (i = 0; i < IMGiLAB.nL; i += linhasJanela){ //i += linhasJanela
	for (j = 0; j < IMGiLAB.nC; j += colunasJanela){//j += colunasJanela
		retornaSubMat(linhasJanela, colunasJanela, iLAB, i, j, &IMGiLAB, &amostraiLAB); //devolve uma submatriz de amostra iLAB na posição i, j fornecida.
		retornaSubMat(linhasJanela, colunasJanela, iRGB, i, j, &IMGiRGB, &amostraiRGB); //devolve uma submatriz de amostra iRGB na posição i, j fornecida.

		normaliza(&amostraiLAB, IMGiLAB.maxVal);  //normaliza dados da matriz de amostra iLAB, cria lista com dados normalizados e a preenche
		normaliza(&amostraiRGB, IMGiRGB.maxVal);  //normaliza dados da matriz de amostra iRGB, cria lista com dados normalizados e a preenche

		if (atoi(args[3]) == 4){
			TXiLAB = retornaTXEQ4(retornaMediaFL(&amostraiLAB),retornaDesvPadrao(&amostraiLAB), paramP, paramK); //equação 4
			TXiRGB = retornaTXEQ4(retornaMediaFL(&amostraiRGB),retornaDesvPadrao(&amostraiRGB), paramP, paramK); //equação 4
			}

		if (atoi(args[3]) == 3){
			TXiLAB = retornaTXEQ3(retornaMediaFL(&amostraiLAB),retornaDesvPadrao(&amostraiLAB), paramK); //equação 3
			TXiRGB = retornaTXEQ3(retornaMediaFL(&amostraiRGB),retornaDesvPadrao(&amostraiRGB), paramK); //equação 3
			}
		printf("I: %d J: %d\n", i, j);
		printf("TXiLAB: %f \t TXiRGB: %f\n", TXiLAB, TXiRGB);
		escreveSubMat(linhasJanela, colunasJanela, novoArquivo, i, j, &IMGiLAB, &amostraiLAB, &amostraiRGB, TXiLAB, TXiRGB); //escreve, no arquivo novo, pixels preto ou branco dependendo do TXiLAB e TXiRGB fornecido.
	}
}
escreveARQ(novoArquivo, &IMGiLAB); //escreve o cabeçalho no novo arquivo de imagem
fclose(iLAB);
fclose(iRGB);
fclose(novoArquivo);
free(amostraiLAB.dados);
free(amostraiLAB.dadosFL);
free(amostraiRGB.dados);
free(amostraiRGB.dadosFL);
return 0;

}

