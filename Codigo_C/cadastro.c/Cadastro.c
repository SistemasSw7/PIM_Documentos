
#include <stdio.h>
#include <string.h>

// Estrutura responsável por armazenar os dados de cada cliente
struct Cliente {
    int id;
    char nome[50];
    char email[100];
    char telefone[20];
    char senha[50];
};

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
    cadastrocliente = fopen("cliente.dat", "rb");

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
        cadastrocliente = fopen("cliente.dat", "rb");

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


    // ==================== SALVAR CLIENTE ====================

    // Abre o arquivo em modo de adição binária
    // "ab" adiciona registros sem apagar os anteriores
    cadastrocliente = fopen("cliente.dat", "ab");

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

        // Fecha o arquivo após salvar o cadastro
        fclose(cadastrocliente);
    }

}

int main() {
    cadastrarCliente();

    return 0;
}
