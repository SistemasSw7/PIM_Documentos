
#include <stdio.h>

// Funcao responsavel pelo menu do barbeiro
void menuBarbeiro(void){

    // Variavel que armazena a opcao escolhida pelo barbeiro
    int opcao = 0;

    // Mantem o menu aberto enquanto a opcao for diferente de 4
    while (opcao != 4){

        // Exibe as opcoes disponiveis para o barbeiro
        printf("\n===== AREA DO BARBEIRO =====\n");
        printf("1 - Configurar dia e horarios de trabalho\n");
        printf("2 - configurar intervalos e folgas\n");
        printf("3 - Visualizar agendamentos\n");
        printf("4 - Sair da conta\n");

        // Solicita a opcao desejada pelo barbeiro
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Verifica qual opcao foi escolhida
        switch (opcao){

            case 1:
                // Opcao para configurar os dias e horarios de trabalho
                // Funcionalidade ainda em desenvolvimento
                printf("Configurar dia e horario de trabalho\n");
                break;

            case 2:
                // Opcao para configurar intervalos e folgas
                // Funcionalidade ainda em desenvolvimento
                printf("configurar intervalos e folgas\n");
                break;

            case 3:
                // Opcao para visualizar os agendamentos
                // Funcionalidade ainda sera conectada
                printf("Visualizar agendamentos\n");
                break;

            case 4:
                // Encerra o menu do barbeiro e retorna ao menu anterior
                printf("Sair da conta do barbeiro...\n");
                break;

            default:
                // Exibe uma mensagem quando a opcao digitada e invalida
                printf("opcao invalida!\n");

        }
    }

}
