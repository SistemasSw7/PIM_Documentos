
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
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

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

//Mostra apenas os agendamentos de uma data especifica
void visualizarAgendamentosPorData(void){

    FILE *arquivo;
    struct Agendamento agendamento;

    int dia, mes, ano;
    int quantidade = 0;

    //Solicita a data que sera consultada
    printf("\nDigite a data (DD/MM/AAAA): ");
    if(scanf("%d/%d/%d", &dia, &mes, &ano) != 3){
        printf("Data digitada incorretamente!\n");

        //Limpa a entrada para evitar problemas no menu
        int c;
        while((c = getchar()) != '\n' && c != EOF){
        }
        return;
    }

    //Abre o arquivo de agendamentos
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

        if (arquivo == NULL) {
            printf("Nenhum agendamento encontrado!\n");
            return;
        }

        printf("\n===== AGENDAMENTOS DO DIA %02d/%02d/%04d =====\n",
            dia, mes, ano);

        // Le um agendamento por vez
        while (fread(&agendamento, sizeof(struct Agendamento), 1, arquivo) == 1){

            // Confere se a data do agendamento e igual a data informada
            if (agendamento.dia == dia &&
                agendamento.mes == mes &&
                agendamento.ano == ano) {

                quantidade++;

                printf("ID do agendamento: %d\n", agendamento.id);
                printf("ID do cliente: %d\n", agendamento.idCliente);
                printf("Nome: %s\n", agendamento.nomeAtendimento);

                printf("Data: %02d/%02d/%04d\n",
                    agendamento.dia, agendamento.mes, agendamento.ano);

                printf("Horario: %02d:00\n", agendamento.hora);

                printf("\n------------------------------\n");
            }
        }

        if (quantidade == 0) {
            printf("Nenhum agendamento encontrado para essa data!\n");
        }

        fclose(arquivo);
}

//Menu de consulta ultiliza pelo ADM e pelo barbeiro
void menuVisualizarAgendamentos(void){

    int opcao = 0;

    while(opcao != 3){
        printf("\n===== VISUALIZAR AGENDAMENTOS =====\n");

        printf("1 - Visualizar todos os agendamentos\n");
        printf("2 - Visualizar agendamentos por data\n");
        printf("3 - Voltar\n");

        printf("Escolher uma opcao: ");
        
        if(scanf("%d", &opcao) != 1){
            int c;

            printf("Digite apenas numero!\n");

            while ((c = getchar()) != '\n' && c != EOF){
            }
            opcao = 0;
            continue;
        }
        switch (opcao){

        case 1:
            //Mostra todos os agendamentos cadastrados
            visualizarAgendamentosADM();
            break;

        case 2:
             // Consulta os agendamentos de uma data especifica
            visualizarAgendamentosPorData();
            break;
        case 3:
            printf("Voltando ao menu anterios...\n");
            break;
        
        default:
            printf("Opcao invalida!\n");
            break;
        }
    }
}
