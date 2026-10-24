
#include <stdio.h>

// Declaracao das funcoes utilizadas no gerenciamento
void visualizarAgendamentosADM(void); // Exibe todos os agendamentos cadastrados
void cancelarAgendamentoADM(void);    // Permite cancelar um agendamento

// Funcao responsavel pelo gerenciamento dos agendamentos do ADM
void gerenciarAgendamentosADM(void){

    // Variavel que armazena a opcao escolhida pelo administrador
    int opcao = 0;

    // Mantem o menu aberto enquanto a opcao for diferente de 3
    while (opcao != 3){

        // Exibe o menu de gerenciamento de agendamentos
        printf("\n===== GERENCIAR AGENDAMENTOS =====\n");
        printf("1 - Visualizar agendamentos\n");
        printf("2 - Cancelar agendamento\n");
        printf("3 - Voltar ao menu ADM\n");

        // Solicita a opcao desejada pelo administrador
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Verifica qual opcao foi escolhida
        switch(opcao){

            case 1:
                // Chama a funcao que mostra todos os agendamentos
                visualizarAgendamentosADM();
                break;

            case 2:
                // Chama a funcao de cancelamento de agendamentos
                cancelarAgendamentoADM();
                break;

            case 3:
                // Encerra este menu e retorna ao menu administrativo
                printf("Voltando ao menu ADM...\n");
                break;

            default:
                // Exibe uma mensagem quando a opcao digitada nao existe
                printf("Opcao invalida!\n");
        }
    }

}
