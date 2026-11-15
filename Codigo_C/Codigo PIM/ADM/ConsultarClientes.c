
#include <stdio.h>

// Inclui a estrutura que armazena os dados dos clientes
#include "../Cliente/Cliente.h"

// Funcao responsavel por consultar os clientes cadastrados
void consultarClientesADM(void) {

    // Ponteiro utilizado para acessar o arquivo de clientes
    FILE *arquivo;

    // Variavel que armazena os dados de cada cliente lido
    struct Cliente cliente;

    // Exibe o titulo da consulta
    printf("\n===== CLIENTES CADASTRADOS =====\n");

    // Abre o arquivo de clientes no modo leitura binaria
    arquivo = fopen("../Dados/cliente.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {

        printf("Nenhum cliente encontrado ou erro ao abrir o arquivo!\n");
        return;
    }

    // Percorre o arquivo lendo um cliente por vez
    while (fread(&cliente,
                 sizeof(struct Cliente),
                 1,
                 arquivo) == 1) {

        printf("\nID: %d\n", cliente.id);
        printf("Nome: %s\n", cliente.nome);
        printf("Telefone: %s\n", cliente.telefone);
        printf("Email: %s\n", cliente.email);

        // Exibe a data de nascimento
        printf("Data de nascimento: %02d/%02d/%04d\n",
               cliente.diaNascimento,
               cliente.mesNascimento,
               cliente.anoNascimento);

        // Separador entre os clientes
        printf("---------------------------------------\n");
    }

    // Fecha o arquivo depois da consulta
    fclose(arquivo);
}
