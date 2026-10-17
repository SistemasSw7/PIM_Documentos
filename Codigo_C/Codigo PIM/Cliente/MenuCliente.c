
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../Cliente/Cliente.h"



void cadastrarCliente() {

    // Armazena os dados do novo cliente
    struct Cliente cliente;

    // Armazena temporariamente os clientes lidos do arquivo
    struct Cliente clientearquivo;

    // Variáveis utilizadas nas validações
    int quantidade = 0;
    int telefonevalido;
    int emailExistente = 0;
    int temMaiuscula;
    int temEspecial;

    // Ponteiro utilizado para abrir, ler e salvar arquivos
    FILE *cadastrocliente;


    // ==================== GERAR ID ====================

    // Abre o arquivo para contar os clientes já cadastrados
    cadastrocliente = fopen("../../Dados/cliente.dat", "rb");

    if (cadastrocliente != NULL) {

        // Lê cada registro e aumenta a quantidade de clientes
        while (fread(&clientearquivo, sizeof(struct Cliente), 1, cadastrocliente) == 1) {
            quantidade++;
        }

        // Fecha o arquivo após a leitura
        fclose(cadastrocliente);
    }

    // Gera o ID do novo cliente
    cliente.id = quantidade + 1;


    // ==================== CADASTRAR NOME ====================

    printf("Digite seu nome\n");

    // Permite digitar nomes com espaços, respeitando o limite
    scanf(" %49[^\n]", cliente.nome);


    // ==================== VALIDAR TELEFONE ====================

    do {
        printf("Digite seu telefone\n");
        scanf("%19s", cliente.telefone);

        // Inicialmente considera o telefone válido
        telefonevalido = 1;

        // Percorre cada caractere digitado
        for (int i = 0; i < strlen(cliente.telefone); i++) {

            // Se encontrar algo diferente de 0 a 9, invalida
            if (cliente.telefone[i] < '0' || cliente.telefone[i] > '9') {
                telefonevalido = 0;
            }
        }

        // Repete se não tiver 10 ou 11 dígitos numéricos
    } while (strlen(cliente.telefone) < 10 ||
             strlen(cliente.telefone) > 11 ||
             telefonevalido == 0);


    // ==================== VALIDAR EMAIL ====================

    do {

        // Verifica se o email contém @ e ponto
        do {
            printf("Digite seu email\n");
            scanf("%99s", cliente.email);

        } while (strchr(cliente.email, '@') == NULL ||
                 strchr(cliente.email, '.') == NULL);

        // Abre o arquivo para procurar emails já cadastrados
        cadastrocliente = fopen("../../Dados/cliente.dat", "rb");

        // 0 = email disponível | 1 = email já cadastrado
        emailExistente = 0;

        if (cadastrocliente != NULL) {

            // Percorre os clientes salvos no arquivo
            while (fread(&clientearquivo, sizeof(struct Cliente), 1, cadastrocliente) == 1) {

                // strcmp retorna 0 quando os emails são iguais
                if (strcmp(cliente.email, clientearquivo.email) == 0) {
                    emailExistente = 1;
                }
            }

            fclose(cadastrocliente);

            // Informa se o email já estiver cadastrado
            if (emailExistente == 1) {
                printf("Email ja cadastrado!\n");
            }
        }

        // Solicita outro email enquanto houver duplicidade
    } while (emailExistente == 1);


    // ==================== VALIDAR SENHA ====================

    // Apresenta os requisitos antes de solicitar a senha
    printf("\nSua senha deve conter:\n");
    printf("- No minimo 6 caracteres\n");
    printf("- Pelo menos 1 letra maiuscula\n");
    printf("- Pelo menos 1 caractere especial (@, #, !, $)\n");

    do {
        printf("\nDigite uma senha\n");
        scanf("%49s", cliente.senha);

        // Reinicia as verificações a cada tentativa
        temMaiuscula = 0;
        temEspecial = 0;

        // Percorre os caracteres da senha
        for (int i = 0; i < strlen(cliente.senha); i++) {

            // Verifica se existe pelo menos uma letra maiúscula
            if (cliente.senha[i] >= 'A' && cliente.senha[i] <= 'Z') {
                temMaiuscula = 1;
            }

            // Verifica se existe pelo menos um caractere especial
            if ((cliente.senha[i] >= '!' && cliente.senha[i] <= '/') ||
                (cliente.senha[i] >= ':' && cliente.senha[i] <= '@') ||
                (cliente.senha[i] >= '[' && cliente.senha[i] <= '`') ||
                (cliente.senha[i] >= '{' && cliente.senha[i] <= '~')) {

                temEspecial = 1;
            }
        }

        // Mostra um aviso se algum requisito não for atendido
        if (strlen(cliente.senha) < 6 ||
            temMaiuscula == 0 ||
            temEspecial == 0) {

            printf("Senha invalida! Use no minimo 6 caracteres, uma letra maiuscula e um caractere especial.\n");
        }

        // Repete até a senha atender a todos os requisitos
    } while (strlen(cliente.senha) < 6 ||
             temMaiuscula == 0 ||
             temEspecial == 0);

    // ==================== DATA DE NASCIMENTO ====================

    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);

    if (dataAtual == NULL){
        printf("Erro ao obter a data atual!\n");
        return;
    }
    int anoAtual = dataAtual->tm_year + 1900;

    // Pede a data de nascimento do cliente
    
    do{
        printf("Digite o dia do nascimento: ");
        
        if(scanf("%d", &cliente.diaNascimento) != 1){
            printf("Digite apenas numeros!\n");

            int c;
            while((c = getchar()) != '\n' && c != EOF){
            }
            cliente.diaNascimento = 0;
        }
        if (cliente.diaNascimento < 1 || cliente.diaNascimento > 31){
            printf("Dia invalido!\n");
        }
    }while (cliente.diaNascimento < 1 || cliente.diaNascimento > 31);

    do{
        printf("Digite o mes do nascimento: ");
        
        if(scanf("%d", &cliente.mesNascimento) != 1) {
            printf("Digite apenas numeros!\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF){
            }

            cliente.mesNascimento = 0;
        }
        if (cliente.mesNascimento < 1 || cliente.mesNascimento > 12){
            printf("Mes invalido!\n");
        }
    }while (cliente.mesNascimento < 1 || cliente.mesNascimento > 12);

    do{
        printf("Digite o ano do nascimento: ");
    
        if(scanf("%d", &cliente.anoNascimento)!= 1){
            printf("Digite apenas numeros!\n");

            int c;
            while((c = getchar()) != '\n' && c != EOF){
            }

            cliente.anoNascimento = 0;
        }
        if(cliente.anoNascimento < 1 || cliente.anoNascimento > anoAtual){
            printf("Ano de nascimento invalido!\n");
        }
    }while(cliente.anoNascimento < 1 || cliente.anoNascimento > anoAtual);

    // Verifica se o ano de nascimento e bissexto

    int bissexto = (cliente.anoNascimento % 4 == 0 && cliente.anoNascimento % 100 != 0) || cliente.anoNascimento % 400 == 0;

    // Verifica se o dia e valido para fevereiro

    if (cliente.mesNascimento == 2){
        if(cliente.diaNascimento > 29){
            printf("Data de nascimento invalida!\n");
            return;
        }
        if(cliente.diaNascimento == 29 && bissexto == 0){
            printf("Data de nascimento invalida!\n");
            return;
        }
    }

     // Verifica os meses que possuem apenas 30 dias
    if (cliente.diaNascimento == 31 &&
        (cliente.mesNascimento == 4 ||
        cliente.mesNascimento == 6 ||
        cliente.mesNascimento == 9 ||
        cliente.mesNascimento == 11)) {

        printf("Data de nascimento invalida!\n");
        return;
    }

    // Verifica se a data de nascimento ainda nao chegou
    if (cliente.anoNascimento == anoAtual) {

        if (cliente.mesNascimento > dataAtual->tm_mon + 1) {
            printf("Data de nascimento no futuro!\n");
            return;
        }

        if (cliente.mesNascimento == dataAtual->tm_mon + 1 &&
            cliente.diaNascimento > dataAtual->tm_mday) {

            printf("Data de nascimento no futuro!\n");
            return;
        }
    }


    // ==================== SALVAR CLIENTE ====================

    // Abre o arquivo em modo de adição binária
    // "ab" adiciona registros sem apagar os anteriores
    cadastrocliente = fopen("../../Dados/cliente.dat", "ab");

    // Verifica se o arquivo foi aberto corretamente
    if (cadastrocliente == NULL) {

        printf("Erro ao abrir o arquivo\n");

    } else {

        printf("Arquivo aberto com sucesso\n");


        // Grava os dados do novo cliente no arquivo
        fwrite(&cliente, sizeof(struct Cliente), 1, cadastrocliente);


        // ==================== EXIBIR CADASTRO ====================

        // Mostra os dados do cliente, sem exibir a senha
        printf("\n=======CLIENTE CADASTRADO=======\n");

        printf("ID: %d\n", cliente.id);
        printf("NOME: %s\n", cliente.nome);
        printf("TELEFONE: %s\n", cliente.telefone);
        printf("EMAIL: %s\n", cliente.email);
        printf("DATA DE NASCIMENTO: %02d/%02d/%d\n", cliente.diaNascimento, cliente.mesNascimento, cliente.anoNascimento);

        // Fecha o arquivo após salvar o cadastro
        fclose(cadastrocliente);
    }

}

