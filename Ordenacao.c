#include <math.h>
#include <stdlib.h>

#include "ManutencaoArquivo.h"
#include "Ordenacao.h"
#include "TAD_bufferEntrada.h"
#include "TAD_bufferSaida.h"

int compare (const void * a, const void * b) // Compara o campo id do registro ITEM_VENDA para o qsort da stdlib
{
  ITEM_VENDA *A = (ITEM_VENDA *)a;
  ITEM_VENDA *B = (ITEM_VENDA *)b;
  return ( A->id - B->id );
}

static int Menor_Elemento(BUFFER **vetor_buffer_entrada, int qtde_buffers_entrada)
{
    ITEM_VENDA proximo, menor;
    int posicao = 0;

    for(int i = 0; i < qtde_buffers_entrada; i++) // O menor inicial é o prox do primeiro buffer de entrada com o status setado em 1
        if(vetor_buffer_entrada[i]->estado_buffer == 1){
            menor = *Proximo_BufferEntrada(vetor_buffer_entrada[i]);
            posicao = i;
            break;
        };

    for(int i = posicao; i < qtde_buffers_entrada; i++) // Compara o menor com os prox dos outros buffers de entrada com status 1
        if (vetor_buffer_entrada[i]->estado_buffer == 1)
        {
            proximo = *Proximo_BufferEntrada(vetor_buffer_entrada[i]);
            if (proximo.id < menor.id)
            {
                menor = proximo;
                posicao = i;
            };
        }

    return posicao; 
}

void Intercalacao(BUFFER **vetor_buffer_entrada, int qtde_buffers_entrada, int qtde_registros, BUFFER *buffer_saida, const char* arquivo_saida)
{

    FILE* arquivo_saida_aberto = fopen(arquivo_saida, "wb");
    int posicao_menor_elemento = 0; 

    ITEM_VENDA* consumido;

    // BUFFER *buffer_saida = Criar_BufferSaida(((33554432/4)/sizeof(ITEM_VENDA)), 256000);  

    for (int i = 0; i < qtde_registros; i++)
    {
        posicao_menor_elemento = Menor_Elemento(vetor_buffer_entrada, qtde_buffers_entrada);

        Inserir_BufferSaida(buffer_saida, &vetor_buffer_entrada[posicao_menor_elemento]->vetor_itens[vetor_buffer_entrada[posicao_menor_elemento]->quantidade_consumidos]);
        
        char nome_arquivo[Conta_digitos(qtde_buffers_entrada)];
        sprintf(nome_arquivo, "%d", posicao_menor_elemento);
        consumido = Consumir_BufferEntrada(vetor_buffer_entrada[posicao_menor_elemento], nome_arquivo);        
        
        if(buffer_saida->quantidade_consumidos == buffer_saida->quantidade_registros || buffer_saida->quantidade_consumidos_total == buffer_saida->quantidade_registros_total)
            Despejar_BufferSaida(buffer_saida, arquivo_saida_aberto);

        printf("%d %d\n", i, consumido->id);
        if(i != consumido->id){
            printf("%d", consumido->id);
            return;
        };
    };

    fclose(arquivo_saida_aberto);

    unsigned long e = Calcular_Tamanho_Arquivo(arquivo_saida);
    printf("Tamanho do arquivo saída %lu\n", e);

};