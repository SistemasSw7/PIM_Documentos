
#include <stdio.h>

// Funcoes que ja criamos
void visualizarHorariosBarbeiro(void);
void visualizarIntervalosBarbeiro(void);
void visualizarFolgasBarbeiro(void);

// Menu compartilhado entre o barbeiro e o ADM
void menuConsultarAgendaBarbeiro(void) {

    int opcao = 0;

    // Mantem o submenu aberto ate escolher voltar
    while (opcao != 4) {

        printf("\n===== CONSULTAR AGENDA DO BARBEIRO =====\n");
        printf("1 - Visualizar horarios de trabalho\n");
        printf("2 - Visualizar intervalos\n");
        printf("3 - Visualizar folgas\n");
        printf("4 - Voltar\n");

        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            int c;

            printf("Digite apenas numeros!\n");

            while ((c = getchar()) != '\n' && c != EOF) {
            }

            opcao = 0;
            continue;
        }

        switch (opcao) {

            case 1:
                // Mostra os horarios de trabalho
                visualizarHorariosBarbeiro();
                break;

            case 2:
                // Mostra os intervalos cadastrados
                visualizarIntervalosBarbeiro();
                break;

            case 3:
                // Mostra as folgas cadastradas
                visualizarFolgasBarbeiro();
                break;

            case 4:
                printf("Voltando ao menu anterior...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
}
