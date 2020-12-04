#pragma once

#include "big_file.h"
#include "ManutencaoArquivo.h"

BUFFER *Criar_BufferEntrada(int qtde_registros_por_particao, const char *arquivo);
ITEM_VENDA* Proximo_BufferEntrada(BUFFER* buffer);
ITEM_VENDA* Consumir_BufferEntrada(BUFFER* buffer, const char* arquivo);
int Vazio_BufferEntrada(BUFFER* buffer);
void Destruir(const char *arquivo_entrada, BUFFER **vetor_buffer_entrada, int qtde_buffers);
