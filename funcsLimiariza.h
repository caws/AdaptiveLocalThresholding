/****************************************************/
/* Aluno:    Charles Washington de Aquino dos Santos*/
/* Avaliação 04: Trabalho Final						*/					
/* UDB-255 - 2015-1 - Prof. Daniel Ferreira 		*/ 	
/* Compilador: Gcc versão 4.8.2						*/
/****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

struct image{ //Estrutura com código, número de colunas, número de linhas e conteúdo da imagem
	char cod[3];
	int nC, nL;
	int maxVal;
	int tamCab;
};

struct janela{
	int nColunas;
	int nLinhas;
	unsigned char *dados;
	float *dadosFL;
};

//carrega uma imagem em uma variável FILE e obtém algumas das características de tal imagem
void carregarIMG(FILE *ptr, struct image *IMG);

//aloca memória dentro do struct Janela
void alocaMem(int numLinhas, int numColunas, struct janela *JAN);

//retorna uma submatriz de amostra com nLinhas e nColunas na posição I, J dada a partir do arquivo de imagem e a joga dentro do struct janela
void retornaSubMat(int numLinhas, int numColunas, FILE *arquivo, int posI, int posJ, struct image *IMG, struct janela *JAN);

//escreve uma sumatriz de amostra com nLinhas e nColunas no novo arquivo na posição I, J fornecida
void escreveSubMat(int numLinhas, int numColunas, FILE *NovoArquivo, int posI, int posJ, struct image *IMGiLAB, struct janela *JANiLAB, struct janela *JANiRGB, float TXiLAB, float TXiRGB);

//normaliza um vetor recebido
void normaliza(struct janela *JAN, int maxVal);

//retorna a média
float retornaMediaFL(struct janela *JAN);

//retorna desvio padrão
float retornaDesvPadrao(struct janela *JAN);

//retorna o TX da Equação 3
float retornaTXEQ3(float media, float desvPadrao, float paramK);

//retorna o TX da Equação 4
float retornaTXEQ4(float media, float desvPadrao, float paramP, float paramK);

//Escreve o cabeçalho do arquivo
void escreveARQ(FILE *ptr, struct image *IMG);
