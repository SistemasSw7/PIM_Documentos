
#include <stdio.h>
#include <string.h>

// Inclui a estrutura Cliente, que armazena
// as informacoes cadastradas dos clientes
#include "../Cliente/Cliente.h"

// Funcao responsavel por realizar o login do cliente
// Retorna o ID se o login for valido ou -1 se falhar
int fazerLogin(void) {

    // Armazena o email e a senha digitados pelo cliente
    char email[100];
    char senha[50];

    // Variavel que identifica se o login foi realizado
    int loginValido = 0;

    // Estrutura utilizada para receber os dados
    // dos clientes armazenados no arquivo
    struct Cliente clienteArquivo;

    // Abre o arquivo binario de clientes para leitura
    FILE *arquivo;
    arquivo = fopen("../Dados/cliente.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes!\n");
        return -1;
    }

    printf("Arquivo aberto com sucesso!\n");

    // Solicita o email cadastrado
    printf("Digite seu email.\n");
    if (scanf("%99s", email) != 1) {
        printf("Entrada invalida!\n");
        fclose(arquivo);
        return -1;
    }

    // Solicita a senha do cliente
    printf("Digite sua senha.\n");
    if (scanf("%49s", senha) != 1) {
        printf("Entrada invalida!\n");
        fclose(arquivo);
        return -1;
    }

    // Percorre os registros armazenados no arquivo
    // lendo um cliente por vez
    while (fread(&clienteArquivo, sizeof(struct Cliente), 1, arquivo) == 1) {

        // Compara o email e a senha informados
        // com os dados do cliente encontrado
        // strcmp retorna 0 quando os textos sao iguais
        if (strcmp(email, clienteArquivo.email) == 0 &&
            strcmp(senha, clienteArquivo.senha) == 0) {

            // Indica que as credenciais foram encontradas
            loginValido = 1;

            // Exibe a confirmacao do login
            printf("Login realizado com sucesso!\n");
            printf("Bem-vindo, %s!\n", clienteArquivo.nome);

            // Guarda o ID antes de encerrar a leitura
            int idCliente = clienteArquivo.id;

            // Fecha o arquivo para liberar os recursos
            fclose(arquivo);

            // Retorna o ID para o menu principal
            return idCliente;
        }
    }

    // Fecha o arquivo apos percorrer os registros
    fclose(arquivo);

    // Exibe uma mensagem caso nao encontre
    // um cliente com o email e a senha informados
    if (loginValido == 0) {
        printf("Email ou senha incorretos!\n");
    }

    // Retorna -1 para indicar que o login falhou
    return -1;
}
