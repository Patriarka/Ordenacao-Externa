#pragma once

#include "big_file.h"

typedef struct BUFFER{
  int estado_buffer; // ligado = 1 | desligado - 0
  int quantidade_registros; // quantidade de registros que o buffer tem
  int quantidade_registros_total; // quantidade de registros que o buffer precisa ler no total
  int quantidade_consumidos; // qtde de registros que o buffer já consumiu em relação a variável de quantidade_registros
  int quantidade_consumidos_total;  // qtde de registros que o buffer já consumiu em relação ao total de registros que ela deve consumir
  ITEM_VENDA* vetor_itens; // vetor de ITEM_VENDA
} BUFFER;

int Conta_digitos(int N);
BUFFER *Iniciar_Buffer();
void Ler_Registros(const char *arquivo_entrada, BUFFER* buffer, int qtde_registros);
void Dividir_Arquivo(const char *arquivo_entrada, int quantidade_registros_por_particao, int quantidade_registros, long int tamanho_arquivo);
long Calcular_Tamanho_Arquivo(const char *nome_arquivo);
void Ordenacao_Externa(const char *arquivo_entrada, long b, long s, const char *arquivo_saida);