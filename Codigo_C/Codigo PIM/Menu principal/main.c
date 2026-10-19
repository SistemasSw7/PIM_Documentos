
#include <stdio.h>

// Declaracao das funcoes utilizadas no sistema
void cadastrarCliente(void);  // Funcao responsavel pelo cadastro de clientes
int fazerLogin(void);         // Realiza o login do cliente e retorna seu ID
void menuCliente(int idCliente); // Abre o menu do cliente logado
int fazerLoginADM(void);      // Realiza o login do administrador
void menuADM(void);           // Abre o menu administrativo

int main() {

    // Variavel que armazena a opcao escolhida no menu
    int opcao = 0;

    // Armazena o ID do cliente logado
    // O valor -1 indica que nenhum cliente esta logado
    int idCliente = -1;

    // Mantem o sistema funcionando enquanto a opcao for diferente de 4
    while (opcao != 4) {

        // Exibe o menu principal da barbearia
        printf("\n===== SISTEMA BARBEARIA =====\n");

        printf("1 - Cadastrar cliente\n");
        printf("2 - Fazer login\n");
        printf("3 - Login administrador\n");
        printf("4 - Sair\n");

        // Solicita a opcao desejada pelo usuario
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Verifica qual opcao foi escolhida
        switch (opcao){

            case 1:
                // Chama a funcao responsavel pelo cadastro de clientes
                cadastrarCliente();
                break;

            case 2:
                // Realiza o login e recebe o ID do cliente
                idCliente = fazerLogin();

                // Verifica se o login foi realizado com sucesso
                if (idCliente > 0) {

                    // Exibe o ID do cliente logado
                    printf("Cliente logado com ID: %d\n", idCliente);

                    // Abre o menu do cliente passando seu ID
                    menuCliente(idCliente);

                    // Reinicia o ID apos sair do menu do cliente
                    idCliente = -1;
                }
                break;

            case 3:
                // Realiza o login do administrador
                // O retorno 1 indica que o login foi autorizado
                if (fazerLoginADM() == 1) {

                    // Exibe a mensagem de boas-vindas
                    printf("Bem-vindo a area administrativa!\n");

                    // Abre o menu administrativo
                    menuADM();
                }
                break;

            case 4:
                // Encerra o sistema quando o usuario escolhe a opcao 4
                printf("Saindo...\n");
                break;

        default:
            // Exibe uma mensagem caso a opcao seja invalida
            printf("Opcao invalida!\n");
        }
    }

    // Finaliza a execucao do programa
    return 0;

}
