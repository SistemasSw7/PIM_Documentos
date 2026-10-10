#include <stdio.h>
#include <string.h>
#include "../Cliente/Cliente.h"


void fazerLogin() {


    char email[100];
    char senha[50];
    int loginValido = 0;

    struct Cliente clienteArquivo;

    FILE *arquivo;
    arquivo = fopen("../../Dados/cliente.dat", "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de clientes!\n");
        return;
    }

    printf("Arquivo aberto com sucesso!\n");

    printf("Digite seu email.\n");
    scanf("%99s", email);

    printf("Digite sua senha.\n");
    scanf("%49s", senha);

    while (fread(&clienteArquivo, sizeof(struct Cliente), 1, arquivo) == 1) {
        
            if(strcmp(email, clienteArquivo.email) == 0 && strcmp(senha, clienteArquivo.senha) == 0) {

                loginValido = 1;

                printf("Login realizado com sucesso!\n");
                break;
            }
    }
    fclose(arquivo); 

    if (loginValido == 0) {
        printf("Email ou senha incorretos!\n");
    }


}
