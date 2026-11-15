
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../Cliente/Cliente.h"


void cadastrarCliente() {

    // Armazena os dados do novo cliente
    struct Cliente cliente = {0};

    // Armazena os clientes lidos do arquivo
    struct Cliente clientearquivo;

    // Variaveis utilizadas nas validacoes
    int quantidade = 0;
    int telefonevalido;
    int emailExistente = 0;
    int temMaiuscula;
    int temEspecial;
    int nascimentoValido = 0;

    // Ponteiro para abrir, ler e salvar arquivos
    FILE *cadastrocliente;


    // ==================== GERAR ID ====================

    cadastrocliente = fopen("../Dados/cliente.dat", "rb");

    if (cadastrocliente != NULL) {

        while (fread(&clientearquivo,
                     sizeof(struct Cliente),
                     1,
                     cadastrocliente) == 1) {

            quantidade++;
        }

        fclose(cadastrocliente);
    }

    cliente.id = quantidade + 1;


    // ==================== CADASTRAR NOME ====================

    printf("Digite seu nome\n");

    // Permite nomes com espacos
    scanf(" %49[^\n]", cliente.nome);


    // ==================== VALIDAR TELEFONE ====================

    do {

        printf("Digite seu telefone\n");
        scanf("%19s", cliente.telefone);

        telefonevalido = 1;

        // Verifica se todos os caracteres sao numeros
        for (size_t i = 0; i < strlen(cliente.telefone); i++) {

            if (cliente.telefone[i] < '0' ||
                cliente.telefone[i] > '9') {

                telefonevalido = 0;
            }
        }

    } while (strlen(cliente.telefone) < 10 ||
             strlen(cliente.telefone) > 11 ||
             telefonevalido == 0);


    // ==================== CADASTRAR DATA DE NASCIMENTO ====================

    // Consulta a data atual
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);

    if (dataAtual == NULL) {

        printf("Erro ao consultar a data atual!\n");
        return;
    }

    int diaAtual = dataAtual->tm_mday;
    int mesAtual = dataAtual->tm_mon + 1;
    int anoAtual = dataAtual->tm_year + 1900;

    do {

        printf("Data de nascimento\n");

        printf("Digite o dia: ");

        if (scanf("%d", &cliente.diaNascimento) != 1) {

            printf("Dia invalido!\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            if (c == EOF) {
                return;
            }

            continue;
        }

        printf("Digite o mes: ");

        if (scanf("%d", &cliente.mesNascimento) != 1) {

            printf("Mes invalido!\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            if (c == EOF) {
                return;
            }

            continue;
        }

        printf("Digite o ano: ");

        if (scanf("%d", &cliente.anoNascimento) != 1) {

            printf("Ano invalido!\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            if (c == EOF) {
                return;
            }

            continue;
        }

        // Reinicia a validacao
        nascimentoValido = 1;


        // ==================== VALIDAR ANO ====================

        // Nao aceita anos anteriores a 1900
        // nem anos posteriores ao atual

        if (cliente.anoNascimento < 1900 ||
            cliente.anoNascimento > anoAtual) {

            nascimentoValido = 0;
        }


        // ==================== VALIDAR MES E DIA ====================

        // Verifica se o mes esta entre 1 e 12

        if (cliente.mesNascimento < 1 ||
            cliente.mesNascimento > 12) {

            nascimentoValido = 0;

        } else {

            // Quantidade de dias de cada mes
            int diasPorMes[] = {
                31, 28, 31, 30, 31, 30,
                31, 31, 30, 31, 30, 31
            };

            int ano = cliente.anoNascimento;

            // Verifica se o ano e bissexto
            if ((ano % 4 == 0 && ano % 100 != 0) ||
                (ano % 400 == 0)) {

                // Fevereiro possui 29 dias
                diasPorMes[1] = 29;
            }

            // Verifica se o dia existe no mes informado
            if (cliente.diaNascimento < 1 ||
                cliente.diaNascimento >
                diasPorMes[cliente.mesNascimento - 1]) {

                nascimentoValido = 0;
            }
        }


        // ==================== IMPEDIR DATAS FUTURAS ====================

        if (cliente.anoNascimento > anoAtual ||

            (cliente.anoNascimento == anoAtual &&
             cliente.mesNascimento > mesAtual) ||

            (cliente.anoNascimento == anoAtual &&
             cliente.mesNascimento == mesAtual &&
             cliente.diaNascimento > diaAtual)) {

            nascimentoValido = 0;
        }


        // ==================== RESULTADO DA VALIDACAO ====================

        if (nascimentoValido == 0) {

            printf("\nData de nascimento invalida!\n");
            printf("Digite uma data valida que nao esteja no futuro.\n");
        }

    } while (nascimentoValido == 0);


    // ==================== VALIDAR EMAIL ====================

    do {

        // Verifica se o email contem @ e ponto
        do {

            printf("Digite seu email\n");
            scanf("%99s", cliente.email);

        } while (strchr(cliente.email, '@') == NULL ||
                 strchr(cliente.email, '.') == NULL);

        cadastrocliente = fopen("../Dados/cliente.dat", "rb");

        emailExistente = 0;

        if (cadastrocliente != NULL) {

            while (fread(&clientearquivo,
                         sizeof(struct Cliente),
                         1,
                         cadastrocliente) == 1) {

                if (strcmp(cliente.email,
                           clientearquivo.email) == 0) {

                    emailExistente = 1;
                }
            }

            fclose(cadastrocliente);

            if (emailExistente == 1) {

                printf("Email ja cadastrado!\n");
            }
        }

    } while (emailExistente == 1);


    // ==================== VALIDAR SENHA ====================

    printf("\nSua senha deve conter:\n");
    printf("- No minimo 6 caracteres\n");
    printf("- Pelo menos 1 letra maiuscula\n");
    printf("- Pelo menos 1 caractere especial (@, #, !, $)\n");

    do {

        printf("\nDigite uma senha\n");
        scanf("%49s", cliente.senha);

        temMaiuscula = 0;
        temEspecial = 0;

        for (size_t i = 0; i < strlen(cliente.senha); i++) {

            // Verifica letra maiuscula
            if (cliente.senha[i] >= 'A' &&
                cliente.senha[i] <= 'Z') {

                temMaiuscula = 1;
            }

            // Verifica caractere especial
            if ((cliente.senha[i] >= '!' &&
                 cliente.senha[i] <= '/') ||

                (cliente.senha[i] >= ':' &&
                 cliente.senha[i] <= '@') ||

                (cliente.senha[i] >= '[' &&
                 cliente.senha[i] <= '`') ||

                (cliente.senha[i] >= '{' &&
                 cliente.senha[i] <= '~')) {

                temEspecial = 1;
            }
        }

        if (strlen(cliente.senha) < 6 ||
            temMaiuscula == 0 ||
            temEspecial == 0) {

            printf("Senha invalida! Use no minimo 6 caracteres, uma letra maiuscula e um caractere especial.\n");
        }

    } while (strlen(cliente.senha) < 6 ||
             temMaiuscula == 0 ||
             temEspecial == 0);


    // ==================== SALVAR CLIENTE ====================

    cadastrocliente = fopen("../Dados/cliente.dat", "ab");

    if (cadastrocliente == NULL) {

        printf("Erro ao abrir o arquivo\n");

    } else {

        printf("Arquivo aberto com sucesso\n");

        if (fwrite(&cliente,
                   sizeof(struct Cliente),
                   1,
                   cadastrocliente) != 1) {

            printf("Erro ao salvar o cliente!\n");
            fclose(cadastrocliente);
            return;
        }


        // ==================== EXIBIR CADASTRO ====================

        printf("\n=======CLIENTE CADASTRADO=======\n");

        printf("ID: %d\n", cliente.id);
        printf("NOME: %s\n", cliente.nome);
        printf("TELEFONE: %s\n", cliente.telefone);
        printf("EMAIL: %s\n", cliente.email);

        fclose(cadastrocliente);
    }
}
