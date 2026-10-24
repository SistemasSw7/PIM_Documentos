
#include <stdio.h>

// Declaracao das funcoes utilizadas no menu administrativo
void visualizarAgendamentosADM(void); // Exibe todos os agendamentos cadastrados
void consultarClientesADM(void);      // Consulta os clientes cadastrados
void gerenciarAgendamentosADM(void);  // Funcao responsavel pelo gerenciamento dos agendamentos

// Funcao responsavel pelo menu do administrador
void menuADM(void){

    // Variavel que armazena a opcao escolhida pelo administrador
    int opcao = 0;

    // Mantem o menu aberto enquanto a opcao for diferente de 4
    while(opcao != 4){

        // Exibe as opcoes disponiveis para o administrador
        printf("\n===== AREA DO ADMINISTRADOR =====\n");

        printf("1 - Visualizar agendamentos\n");
        printf("2 - Consultar clientes\n");
        printf("3 - Gerenciar agendamentos\n");
        printf("4 - Sair da conta\n");

        // Solicita a opcao desejada pelo administrador
        printf("Escolher uma opcao\n");
        scanf("%d", &opcao);

        // Verifica qual opcao foi escolhida
        switch (opcao){

            case 1:
                // Chama a funcao que mostra todos os agendamentos
                visualizarAgendamentosADM();
                break;

            case 2:
                // Chama a funcao que mostra os clientes cadastrados
                consultarClientesADM();
                break;

            case 3:
                // Abre o menu de gerenciamento de agendamentos
                gerenciarAgendamentosADM();
                break;

            case 4:
                // Encerra o menu ADM e retorna ao menu principal
                printf("Saindo...\n");
                break;

            default:
                // Exibe uma mensagem quando a opcao digitada nao existe
                printf("Opcao invalida!\n");

        }
    }

}
