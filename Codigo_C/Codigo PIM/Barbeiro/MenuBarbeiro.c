
#include <stdio.h>

// Declaracao da funcao responsavel por configurar
// os dias e horarios de trabalho do barbeiro
void configurarHorarios(void);

//os dias e o horarios de intervalos e folgas
void configurarIntervalosFolgas(void);

// Funcao responsavel por exibir e controlar o menu do barbeiro
void menuBarbeiro(void) {

    // Armazena a opcao escolhida pelo barbeiro
    int opcao = 0;

    // Mantem o menu funcionando ate o barbeiro escolher sair
    while (opcao != 4) {

        // Exibe as funcionalidades disponiveis
        printf("\n===== AREA DO BARBEIRO =====\n");
        printf("1 - Configurar dia e horarios de trabalho\n");
        printf("2 - Configurar intervalos e folgas\n");
        printf("3 - Visualizar agendamentos\n");
        printf("4 - Sair da conta\n");

        // Solicita a opcao que o barbeiro deseja acessar
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Verifica a opcao escolhida e executa a acao correspondente
        switch (opcao) {

            case 1:
                // Chama a funcao que permite configurar
                // os dias e horarios de atendimento
                configurarHorarios();

                // Encerra este caso e retorna ao menu do barbeiro
                break;

            case 2:
                // Opcao destinada a configurar intervalos e folgas
                configurarIntervalosFolgas();
                break;

            case 3:
                // Opcao destinada a consultar os agendamentos
                // Funcionalidade ainda sera implementada
                printf("Visualizar agendamentos\n");
                break;

            case 4:
                // Encerra o menu do barbeiro
                // Retorna ao menu principal do sistema
                printf("Sair da conta do barbeiro...\n");
                break;

            default:
                // Exibe uma mensagem caso a opcao digitada seja invalida
                printf("Opcao invalida!\n");
                break;
        }
    }
}
