#pragma once
#include "ManutencaoArquivo.h"

int compare (const void * a, const void * b);
void Intercalacao(BUFFER *vetor_buffer_entrada, int qtde_buffers_entrada, int qtde_registros, BUFFER* buffer_saida, const char* arquivo_saida);