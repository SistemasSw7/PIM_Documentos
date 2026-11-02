#include <stdio.h>
#include <string.h>

//Funcao responsavel por autenticar o barbeiro
int fazerLoginBarbeiro(void){

    //Variaveis que armazena o email e a senha
    char email[100];
    char senha[50];

    //Exibe a tela de login
    printf("\n===== LOGIN DO BARBEIRO =====\n");

    //Solicita o email
    printf("Digite o email: ");
    scanf("%99s", email);

    //Solicita a senha
    printf("Digite a senha: ");
    scanf("%49s", senha);

    //Verifica se o email e a senha estao corretos
    if(strcmp(email, "Barbeiro@gmail.com") == 0 && strcmp(senha, "teste123.") == 0){
        printf("Login realizado com sucesso!\n");
        return 1;
    }else{
        printf("Email ou senha incorreta!\n");
        return 0;
    }

}
//Permite ate 3 tentativas de login do barbeiro
int loginBarbeiroComTentativas(void){

    int tentativa;

    //Repete o login no maximo 3 vezes
    for(tentativa = 1; tentativa <= 3; tentativa++){

        //Chama a funcao de login que ja criamos
        if(fazerLoginBarbeiro() == 1){
            return 1;
        }

        //Informa quantas tentativas restam
        if(tentativa < 3){
            printf("Tentativas restantes: %d\n", 3 - tentativa);
        }
    }
    //Executado depois de 3 tentativas incorretas
    printf("Limite de tentativas atingido!\n");
    printf("Voltando ao menu principal...\n");
    return 0;

}