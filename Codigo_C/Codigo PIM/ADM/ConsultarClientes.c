
#include <stdio.h>

// Inclui a estrutura que armazena os dados dos clientes
#include "../Cliente/Cliente.h"

// Funcao responsavel por consultar os clientes cadastrados
void consultarClientesADM(void){

    // Ponteiro utilizado para acessar o arquivo de clientes
    FILE *arquivo;

    // Variavel que armazena os dados de cada cliente lido
    struct Cliente cliente;

    // Exibe o titulo da consulta
    printf("\n===== CLIENTES CADASTRADOS =====\n");

    // Abre o arquivo de clientes no modo leitura binaria
    arquivo = fopen("../../Dados/cliente.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL){

        // Exibe uma mensagem caso nao consiga abrir o arquivo
        printf("Nenhum cliente encontrado ou erro ao abrir o arquvio!\n");

        // Encerra a funcao caso o arquivo nao seja aberto
        return;
    }

    // Percorre o arquivo lendo um cliente por vez
    // O loop continua enquanto conseguir ler um registro completo
    while(fread(&cliente, sizeof(struct Cliente), 1, arquivo) == 1){

        // Exibe o ID do cliente
        printf("\nID: %d\n", cliente.id);

        // Exibe o nome do cliente
        printf("Nome: %s\n", cliente.nome);

        // Exibe o telefone do cliente
        printf("Telefone: %s\n", cliente.telefone);

        // Exibe o email do cliente
        printf("Email: %s\n", cliente.email);

        // Exibe a data de nascimento no formato dia/mes/ano
        printf("Data de nascimento: %02d/%02d/%04d\n", cliente.diaNascimento, cliente.mesNascimento, cliente.anoNascimento);

        // Linha utilizada para separar os clientes
        printf("---------------------------------------\n");
    }

    // Fecha o arquivo apos finalizar a leitura
    fclose(arquivo);

}
