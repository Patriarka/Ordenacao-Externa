all:
	gcc -g -c Ordenacao.c 
	gcc -g -c ManutencaoArquivo.c 
	gcc -g -c TAD_bufferEntrada.c 
	gcc -g -c TAD_bufferSaida.c 
	gcc -g -c big_file.c 
	gcc -g -c main.c
	gcc -g *.o -o main -lm
			