#pragma once

#include <stdio.h>

#include "big_file.h"
#include "ManutencaoArquivo.h"

BUFFER* Criar_BufferSaida(int s, int quantidade_registros);
void Inserir_BufferSaida(BUFFER* buffer, ITEM_VENDA* item_venda);
void Despejar_BufferSaida(BUFFER* buffer, FILE* nome_arquivo);
void Destruir_BufferSaida(BUFFER* buffer);
