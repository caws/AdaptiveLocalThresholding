all: trabalhoFinal

trabalhoFinal: funcoes.o limiariza.o funcsLimiariza.h
	gcc limiariza.o funcoes.o funcsLimiariza.h -o trabalhoFinal -lm
limiariza.o:
	gcc -c limiariza.c -o limiariza.o -lm

funcoes.o:
	gcc -c funcsLimiariza.c -o funcoes.o -lm

