#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ManutencaoArquivo.h"
#include "TAD_bufferEntrada.h"

BUFFER *Criar_BufferEntrada(int qtde_registros_por_particao, const char *arquivo)
{

  long tamanho_arquivo_particao = Calcular_Tamanho_Arquivo(arquivo);

  BUFFER *buffer_entrada = (BUFFER *) malloc(sizeof(BUFFER));

  buffer_entrada->arquivo = fopen(arquivo, "rb");

  buffer_entrada->estado_buffer = 1;
  buffer_entrada->quantidade_consumidos = 0;

  buffer_entrada->quantidade_registros = qtde_registros_por_particao;
  buffer_entrada->quantidade_registros_total = tamanho_arquivo_particao / sizeof(ITEM_VENDA);
  buffer_entrada->quantidade_consumidos_total = 0;

  Ler_Registros(arquivo, buffer_entrada, qtde_registros_por_particao);

  return buffer_entrada;
};

ITEM_VENDA *Proximo_BufferEntrada(BUFFER *buffer)
{
  return &buffer->vetor_itens[buffer->quantidade_consumidos];
};

ITEM_VENDA *Consumir_BufferEntrada(BUFFER *buffer, const char *arquivo)
{
  if (buffer->estado_buffer == 0)
    return NULL;

  if (Vazio_BufferEntrada(buffer) == 1)
  { // Caso a quantidade de registros consumidos seja igual a quantidade de registros contidos no buffer
    Ler_Registros(arquivo, buffer, buffer->quantidade_consumidos);
    buffer->quantidade_consumidos = 0;
  };

  buffer->quantidade_consumidos++;
  buffer->quantidade_consumidos_total++;

  if (buffer->quantidade_consumidos_total == buffer->quantidade_registros_total)
    buffer->estado_buffer = 0; // Desativando o buffer

  return &buffer->vetor_itens[buffer->quantidade_consumidos - 1];
};

int Vazio_BufferEntrada(BUFFER *buffer)
{
  if (buffer->quantidade_consumidos == buffer->quantidade_registros)
    return 1;
  else
    return 0;
}

int Destruir_Arquivo(const char *arquivo)
{
  int removido = remove(arquivo);
  if (removido == 0)
    return 0;
  else
  {
    printf("nao removido.\n");
    return -1;
  }
};

void Destruir(const char *arquivo_entrada, BUFFER **vetor_buffer_entrada, int qtde_buffers)
{
  for (int i = 0; i < qtde_buffers; i++)
  {
    char nome_arquivo_particao[Conta_digitos(qtde_buffers)];
    sprintf(nome_arquivo_particao, "%d", i);
    Destruir_Arquivo(nome_arquivo_particao);
    free(vetor_buffer_entrada[i]->vetor_itens);
    free(vetor_buffer_entrada[i]);
  };
  free(vetor_buffer_entrada);
  remove(arquivo_entrada);

}
