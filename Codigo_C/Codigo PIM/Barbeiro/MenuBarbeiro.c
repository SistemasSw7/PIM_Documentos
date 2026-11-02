
#include <stdio.h>

// Declaracao da funcao responsavel por configurar
// os dias e horarios de trabalho do barbeiro
void configurarHorarios(void);

//os dias e o horarios de intervalos e folgas
void configurarIntervalosFolgas(void);

// Abre o menu de consulta de agendamentos
void menuVisualizarAgendamentos(void);

//Mosta os horarios de trabalho cadastrados
void visualizarHorariosBarbeiro(void);

//Abre o submenu de consulta da agenda do barbeiro
void menuConsultarAgendaBarbeiro(void);

// Funcao responsavel por exibir e controlar o menu do barbeiro
void menuBarbeiro(void) {

    // Armazena a opcao escolhida pelo barbeiro
    int opcao = 0;

    // Mantem o menu funcionando ate o barbeiro escolher sair
    while (opcao != 5) {

        // Exibe as funcionalidades disponiveis
        printf("\n===== AREA DO BARBEIRO =====\n");
        printf("1 - Configurar dia e horarios de trabalho\n");
        printf("2 - Configurar intervalos e folgas\n");
        printf("3 - Visualizar agendamentos\n");
        printf("4 - Consultar agenda do barbeiro\n");
        printf("5 - Sair da conta\n");

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
                // Abre o submenu de visualizacao de agendamentos
                menuVisualizarAgendamentos();
                break;
            case 4:
                // Abre o submenu de consulta da agenda
                menuConsultarAgendaBarbeiro();
                break;

            case 5:
            //Encerra o menu do barbeiro
            printf("Saindo da conta do barbeiro...\n");
            break;

            default:
                // Exibe uma mensagem caso a opcao digitada seja invalida
                printf("Opcao invalida!\n");
                break;
        }
    }
}
