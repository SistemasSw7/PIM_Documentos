
#include <stdio.h>

// Declaracao das funcoes utilizadas no sistema
void cadastrarCliente(void);     // Realiza o cadastro de novos clientes
int fazerLogin(void);            // Realiza o login e retorna o ID do cliente
void menuCliente(int idCliente); // Abre o menu do cliente logado
int fazerLoginADM(void);         // Verifica o login do administrador
void menuADM(void);              // Abre o menu administrativo
void menuBarbeiro(void);         // Abre o menu do barbeiro

// Funcao principal, responsavel por iniciar o sistema
int main() {

    // Armazena a opcao escolhida no menu principal
    int opcao = 0;

    // Armazena o ID do cliente que realizou o login
    // O valor -1 indica que nenhum cliente esta logado
    int idCliente = -1;

    // Mantem o sistema funcionando ate o usuario escolher a opcao 5
    while (opcao != 5) {

        // Exibe as opcoes do menu principal
        printf("\n===== SISTEMA BARBEARIA =====\n");
        printf("1 - Cadastrar cliente\n");
        printf("2 - Fazer login\n");
        printf("3 - Login area do Barbeiro\n");
        printf("4 - Login area do ADM\n");
        printf("5 - Sair\n");

        // Solicita ao usuario a opcao desejada
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        // Verifica a opcao escolhida e executa a funcao correspondente
        switch (opcao) {

            case 1:
                // Chama a funcao responsavel pelo cadastro de clientes
                cadastrarCliente();

                // Encerra este caso e retorna ao menu principal
                break;

            case 2:
                // Realiza o login e armazena o ID retornado
                idCliente = fazerLogin();

                // Verifica se o cliente realizou o login com sucesso
                if (idCliente > 0) {

                    // Mostra o ID do cliente autenticado
                    printf("Cliente logado com ID: %d\n", idCliente);

                    // Abre o menu do cliente e envia seu ID
                    menuCliente(idCliente);

                    // Limpa o ID depois que o cliente sai da sua conta
                    idCliente = -1;
                }

                // Retorna ao menu principal
                break;

            case 3:
                // Abre a area do barbeiro
                // Permite configurar horarios, intervalos e folgas
                // O menu ainda nao possui uma autenticacao propria
                menuBarbeiro();
                break;

            case 4:
                // Chama a funcao de login do administrador
                // O retorno 1 significa que o acesso foi autorizado
                if (fazerLoginADM() == 1) {

                    // Exibe uma mensagem de boas-vindas
                    printf("Bem-vindo a area administrativa!\n");

                    // Abre o menu com as funcoes administrativas
                    menuADM();
                }

                // Retorna ao menu principal
                break;

            case 5:
                // Encerra o sistema quando o usuario escolhe a opcao 5
                printf("Saindo...\n");
                break;

            default:
                // Executado quando o usuario digita uma opcao invalida
                printf("Opcao invalida!\n");
                break;
        }
    }

    // Finaliza a execucao do programa
    // O retorno 0 indica que o programa terminou normalmente
    return 0;
}
