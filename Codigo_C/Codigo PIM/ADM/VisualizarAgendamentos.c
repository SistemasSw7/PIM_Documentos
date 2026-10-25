
#include <stdio.h>

// Inclui a estrutura que armazena os dados dos agendamentos
#include "../Agendamento/Agendamento.h"

// Funcao responsavel por mostrar todos os agendamentos ao ADM
void visualizarAgendamentosADM(void){

    // Ponteiro utilizado para acessar o arquivo de agendamentos
    FILE *arquivo;

    // Variavel que armazena cada agendamento lido do arquivo
    struct Agendamento agendamento;

    // conta quantos agendamentos foram encontrados
    int quantidade = 0;

    // Exibe o titulo da consulta
    printf("\n===== TODOS OS AGENDAMENTOS =====\n");

    // Abre o arquivo de agendamentos no modo leitura binaria
    arquivo = fopen("../../Dados/agendamentos.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL){

        // Informa que nao foi possivel acessar os agendamentos
        printf("Nenhum agendamento encontrado!\n");

        // Encerra a funcao caso o arquivo nao seja aberto
        return;
    }

    // Percorre o arquivo lendo um agendamento por vez
    // O loop termina quando nao consegue ler outro registro completo
    while (fread(&agendamento, sizeof(struct Agendamento), 1, arquivo) == 1){
    
        // conta quantos agendamentos foram encontrados
        quantidade++;


        // Exibe o ID do agendamento
        printf("ID do agendamento: %d\n", agendamento.id);

        // Exibe o ID do cliente responsavel pelo agendamento
        printf("ID do cliente: %d\n", agendamento.idCliente);

        // Exibe o nome da pessoa que sera atendida
        printf("Nome: %s\n", agendamento.nomeAtendimento);

        // Exibe a data do agendamento no formato dia/mes/ano
        printf("Data: %02d/%02d/%04d\n", agendamento.dia, agendamento.mes, agendamento.ano);

        // Exibe o horario do atendimento
        printf("Horario: %02d:00\n", agendamento.hora);

        // Linha utilizada para separar os agendamentos
        printf("\n------------------------------\n");
    }
    if (quantidade == 0){
        printf("Nenhum agendamento cadastrado!\n");
    }

    // Fecha o arquivo apos finalizar a leitura
    fclose(arquivo);

}
