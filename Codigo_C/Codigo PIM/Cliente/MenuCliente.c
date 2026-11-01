#include <stdio.h>

void agendarHorario(int idCliente);
void meusAgendamentos(int idCliente);
void cancelarAgendamento(int idCliente);
void menuCliente(int idCliente) {

    int opcao = 0;

    while(opcao != 4) {

        printf("\n===== AREA DO CLIENTE =====\n");
        printf("Cliente ID: %d\n", idCliente);

        printf("1 - Agendar horario\n");
        printf("2 - Meus agendamento\n");
        printf("3 - Cancelar agendamento\n");
        printf("4 - Sair da conta\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {

            case 1:
                agendarHorario(idCliente);
                break;

            case 2:
                meusAgendamentos(idCliente);
                break;

            case 3:
                cancelarAgendamento(idCliente);
                break;

            case 4:
                printf("Saindo...\n");
                break;
            
        default:
            printf("Opcao invalida\n");


        }
    }

        

}