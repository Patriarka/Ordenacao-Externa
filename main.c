#include <stdio.h>
#include <time.h>

#include "big_file.h"
#include "ManutencaoArquivo.h"

#define N 1572864 // Quantidade de registros a serem inseridos

int main(int argc, char** argv){

  clock_t t;

  t = clock();

  gerar_array_iv("entrada.dat", N, 42); // gera o array de registros, que é introduzido no arquivo de entrada

  long b = 134217728; // número maximo de bytes para armazenar registros durante toda a operação

  long s = b/4; // número de bytes do buffer de saida

  Ordenacao_Externa("entrada.dat", b, s, "saida.dat");  // Realiza a ordenação do arquivo de entrada

  t = clock() - t;

  printf("%f seconds\n",((float)t)/CLOCKS_PER_SEC);

};
