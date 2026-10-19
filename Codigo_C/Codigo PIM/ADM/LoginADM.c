
#include <stdio.h>
#include <string.h>

// Funcao responsavel por realizar o login do administrador
int fazerLoginADM(void) {

    // Variaveis que armazenam o usuario e a senha digitados
    char usuario[50];
    char senha[50];

    // Exibe o titulo da tela de login
    printf("\n===== LOGIN ADMINISTRADOR =====\n");

    // Solicita o nome de usuario do administrador
    printf("Digite o usuario: ");
    scanf("%49s", usuario);

    // Solicita a senha do administrador
    printf("Digite a senha: ");
    scanf("%49s", senha);

    // Verifica se o usuario e a senha correspondem
    // as credenciais definidas para teste
    // strcmp retorna 0 quando as strings sao iguais
    // O operador && exige que as duas condicoes sejam verdadeiras
    if(strcmp(usuario, "admin") == 0 && strcmp(senha, "teste123.") == 0) {

        // Informa que o login foi realizado com sucesso
        printf("Login ADM realizado com sucesso!\n");

        // Retorna 1 para indicar que o acesso foi autorizado
        return 1;

    }else {

        // Exibe uma mensagem caso o usuario ou a senha estejam incorretos
        printf("Usuario ou senha incorretos!\n");

        // Retorna 0 para indicar que o acesso foi negado
        return 0;
    }
}
