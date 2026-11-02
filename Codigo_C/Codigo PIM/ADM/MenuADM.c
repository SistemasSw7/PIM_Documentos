
#include <stdio.h>

// Declaracao das funcoes utilizadas no menu administrativo
void menuVisualizarAgendamentos(void); // Abre o menu de consulta de agendamentos
void consultarClientesADM(void);      // Consulta os clientes cadastrados
void gerenciarAgendamentosADM(void);  // Funcao responsavel pelo gerenciamento dos agendamentos
void menuConsultarAgendaBarbeiro(void); // Abre o submenu de consulta da agenda do barbeiro
void configurarHorarios(void);  // Permite ao ADM configure os horarios de trabalho
void configurarIntervalosFolgas(void); //Permite ao ADM configurar intervalos e folgas

// Funcao responsavel pelo menu do administrador
void menuADM(void){

    // Variavel que armazena a opcao escolhida pelo administrador
    int opcao = 0;

    // Mantem o menu aberto enquanto a opcao for diferente de 4
    while(opcao != 7){

        // Exibe as opcoes disponiveis para o administrador
        printf("\n===== AREA DO ADMINISTRADOR =====\n");

        printf("1 - Visualizar agendamentos\n");
        printf("2 - Consultar clientes\n");
        printf("3 - Gerenciar agendamentos\n");
        printf("4 - Consultar agenda do barbeiro\n");
        printf("5 - Configurar horarios de trabalho\n");
        printf("6 - Configurar intervalos e folgas\n");
        printf("7 - Sair da conta\n");

        // Solicita a opcao desejada pelo administrador
        printf("Escolher uma opcao\n");
        scanf("%d", &opcao);

        // Verifica qual opcao foi escolhida
        switch (opcao){

            case 1:
                // Abre o submenu compartilhado de agendamentos
                menuVisualizarAgendamentos();
                break;

            case 2:
                // Abre o submenu de visualizacao
                menuVisualizarAgendamentos();
                break;

            case 3:
                // Abre o menu de gerenciamento de agendamentos
                gerenciarAgendamentosADM();
                break;

            case 4:
                //Abre a consulta de horarios, intervalos e folgas
                menuConsultarAgendaBarbeiro();
                break;

            case 5:
                // Permite configurar os horarios do barbeiro
                configurarHorarios();
                break;

            case 6:
                // Permite configurar intervalos e folgas
                configurarIntervalosFolgas();
                break;

            case 7:
                //Sai da area do administrador
                printf("Saindo da conda do ADM...\n");
                break;

            default:
                // Exibe uma mensagem quando a opcao digitada nao existe
                printf("Opcao invalida!\n");

        }
    }

}
