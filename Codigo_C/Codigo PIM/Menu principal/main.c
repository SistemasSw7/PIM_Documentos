
#include <stdio.h>
void cadastrarCliente(void);
int fazerLogin(void);
void menuCliente(int idCliente);

int main() {

    int opcao = 0;
    int idCliente = -1;

    while (opcao != 3) {

        printf("\n===== SISTEMA BARBEARIA =====\n");

        printf("1 - Cadastrar cliente\n");
        printf("2 - Fazer login\n");
        printf("3 - Sair\n");

        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao){
            case 1:
                cadastrarCliente();
                break;

            case 2:
                idCliente = fazerLogin();
                if (idCliente > 0) {
                    printf("Cliente logado com ID: %d\n", idCliente);
                    menuCliente(idCliente);
                    idCliente = -1;
                }
                break;

            case 3:
                printf("Saindo...\n");
                break;
        
        default:
            printf("Opcao invalida!\n");
        }
    }



    return 0;



}
