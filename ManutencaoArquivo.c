#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "TAD_bufferEntrada.h"
#include "TAD_bufferSaida.h"
#include "Ordenacao.h"
#include "ManutencaoArquivo.h"

int Conta_digitos(int N){ // Conta quantos digitos tem um determinado nome de um arquivo temporário
    int cont = 1;

    if(N >= 10){
        N = N/10;
        cont = cont + Conta_digitos(N);
    }

    return cont;
}

void Ler_Registros(const char *arquivo_entrada, BUFFER *buffer, int qtde_registros) // Executa a leitura dos registros dos arquivos temporários para os buffer de entrada
{
    if (buffer->quantidade_registros_total == 0){ // Se o buffer não tem mais registros para ler do arquivo temporário
        printf("\nbuffer vazio\n");
        return;
    };

    ITEM_VENDA *vetor_itens = (ITEM_VENDA *) malloc(qtde_registros * sizeof(ITEM_VENDA));

    for (int i = 0; i < qtde_registros; i++)
        fread(&vetor_itens[i], sizeof(ITEM_VENDA), 1, buffer->arquivo);

    buffer->vetor_itens = vetor_itens;

    fclose(buffer->arquivo);
};

// Através de um buffer, add o conteúdo do arquivo de e em um arquivo temporário
void Adicionar_Arquivo(ITEM_VENDA *temp, int n_registros, int numero_particao)
{
    char nome_novo_arquivo[Conta_digitos(numero_particao)];
    sprintf(nome_novo_arquivo, "%d", numero_particao); // Cria o nome do arquivo temporário

    FILE *novaParticao = fopen(nome_novo_arquivo, "wb"); 

    fwrite(temp, sizeof(ITEM_VENDA), n_registros, novaParticao);

    fclose(novaParticao);
};

int Dividir_Arquivo(const char *arquivo_entrada, int quantidade_registros_por_particao, int quantidade_registros, long int tamanho_arquivo)
{
    FILE *arq;
    ITEM_VENDA *temp;

    int qtdeParticoes = 0, qtdeElementos = 0, lidos_com_sucesso = 0;
    long int tamanhoLido = 0;

    temp = (ITEM_VENDA *) malloc(quantidade_registros * sizeof(ITEM_VENDA));
    arq = fopen(arquivo_entrada, "rb");

    while (!feof(arq))
    {
        if (tamanho_arquivo - tamanhoLido < (quantidade_registros_por_particao * sizeof(ITEM_VENDA)))
            quantidade_registros_por_particao = ((tamanho_arquivo - tamanhoLido) / sizeof(ITEM_VENDA));

        if (quantidade_registros_por_particao == 0)
            break;

        for (int i = 0; i < quantidade_registros_por_particao; i++)
        {
            lidos_com_sucesso = fread(&temp[i], sizeof(ITEM_VENDA), 1, arq);
            if (lidos_com_sucesso == 0)
                return 0;
        };
        qsort(temp, quantidade_registros_por_particao, sizeof(ITEM_VENDA), compare);
        Adicionar_Arquivo(temp, quantidade_registros_por_particao, qtdeParticoes);
        tamanhoLido += quantidade_registros_por_particao * sizeof(ITEM_VENDA);
        qtdeParticoes++;
    };

    fclose(arq);
    free(temp);

    return qtdeParticoes;
};

long Calcular_Tamanho_Arquivo(const char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "rb");

    // guarda tamanho do arquivo
    long tamanho;

    // calcula o tamanho
    fseek(arquivo, 0, SEEK_END);
    tamanho = ftell(arquivo);

    rewind(arquivo);

    fclose(arquivo);

    return tamanho;
};

void Ordenacao_Externa(const char *arquivo_entrada, long b, long s, const char *arquivo_saida)
{    
    // inicializar variaveis
    unsigned long e = Calcular_Tamanho_Arquivo(arquivo_entrada);

    printf("Tamanho do arquivo %lu\n", e);

    long k = ceil((float)e / b); // k == Quantidade de divisões do arquivo de entrada

    printf("Quantidade de buffers k %ld\n", k);
    
    int quantidade_registros_por_buffers = floor(((b - s) / k) / 1024); // Quantidade de registros que cada buffer de entrada deve armazenar 
    
    printf("Registros por buffer %d\n", quantidade_registros_por_buffers);
    
    int quantidade_registros = (e/sizeof(ITEM_VENDA)); // Quantidade total de registros no arquivo de entrada

    printf("Quantidade de registros %d\n", quantidade_registros);
    
    int quantidade_registros_por_particao = ceil(quantidade_registros / k); // Quantidade de registros 
    
    printf("Registros por particao %d\n", quantidade_registros_por_particao);

    // dividir arquivo e ordenar os registros em N arquivos temporários
    int novo_k = Dividir_Arquivo(arquivo_entrada, quantidade_registros_por_particao, quantidade_registros, e);

    // criar N buffers de entrada e um buffer de saida
    BUFFER **vetor_buffer_entrada = (BUFFER **) malloc(novo_k * sizeof(BUFFER*));
    BUFFER *buffer_saida = Criar_BufferSaida((s/sizeof(ITEM_VENDA)), quantidade_registros);  

    char nome_arquivo[Conta_digitos(novo_k)];
    for(int i = 0; i < novo_k; i++)
    {
        sprintf(nome_arquivo, "%d", i);
        vetor_buffer_entrada[i] = Criar_BufferEntrada(quantidade_registros_por_particao, nome_arquivo);
    };
        
    // consumir todos os arquivos temporários através dos buffer de entrada
    Intercalacao(vetor_buffer_entrada, novo_k, quantidade_registros, buffer_saida, arquivo_saida); 

    // destruir arquivos e buffers
    Destruir_BufferSaida(buffer_saida);  
    Destruir(arquivo_entrada, vetor_buffer_entrada, novo_k);  
}; 