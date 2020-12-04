#include <stdlib.h>

#include "TAD_bufferSaida.h"
#include "ManutencaoArquivo.h"

BUFFER* Criar_BufferSaida(int s, int quantidade_registros) 
{

  BUFFER* buffer_saida = (BUFFER *) malloc(sizeof(BUFFER));

  buffer_saida->estado_buffer = 1;
  buffer_saida->quantidade_consumidos = 0;

  buffer_saida->vetor_itens = (ITEM_VENDA*) malloc(s * sizeof(ITEM_VENDA));
  
  buffer_saida->quantidade_registros = s;
  buffer_saida->quantidade_consumidos = 0;
  buffer_saida->quantidade_consumidos_total = 0;
  buffer_saida->quantidade_registros_total = quantidade_registros; // Quantidade máxima de elementos que podem serem inseridos no buffer de saída

  return buffer_saida;
};

void Inserir_BufferSaida(BUFFER* buffer, ITEM_VENDA* item_venda){
  buffer->vetor_itens[buffer->quantidade_consumidos++] = *item_venda; // Mais um elemento consumido dos contidos no buffer saida
  buffer->quantidade_consumidos_total++; // Mais um elemento consumido em relação a todos os elementos que devem ser consumdos
}

void Despejar_BufferSaida(BUFFER* buffer, FILE* arquivo){

  for(int i = 0; i < buffer->quantidade_consumidos; i++) 
    fwrite(&buffer->vetor_itens[i], sizeof(ITEM_VENDA), 1, arquivo);
  
  buffer->quantidade_consumidos = 0; // seta o valor em 0, para caso volte a realizar novas inserções
  
};

void Destruir_BufferSaida(BUFFER* buffer_saida){
  free(buffer_saida->vetor_itens);
  free(buffer_saida);
}