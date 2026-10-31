
#include <stdio.h>

// Importa a estrutura que armazena os dias e horarios de trabalho
#include "HorarioBarbeiro.h"

// Importa a estrutura que armazena os intervalos do barbeiro
#include "IntervaloBarbeiro.h"

// Prototipo da funcao responsavel por configurar intervalos
void configurarIntervalos(void);


// ============================================================
// MENU DE INTERVALOS E FOLGAS
// ============================================================

// Funcao responsavel por apresentar as opcoes ao barbeiro
void configurarIntervalosFolgas(void) {

    // Armazena a opcao escolhida pelo barbeiro
    int opcao = 0;

    // Mantem o menu aberto ate o barbeiro escolher a opcao 3
    while (opcao != 3) {

        // Exibe as opcoes disponiveis
        printf("\n===== INTERVALOS E FOLGAS =====\n");

        printf("1 - Configurar intervalos\n");
        printf("2 - Configurar folgas\n");
        printf("3 - Voltar\n");

        // Solicita a opcao desejada
        printf("Escolha uma opcao: ");

        // Verifica se o usuario digitou um numero inteiro
        if (scanf("%d", &opcao) != 1) {

            printf("Digite apenas numeros!\n");

            // Limpa os caracteres invalidos da entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            // Reinicia a opcao para manter o menu ativo
            opcao = 0;
        }

        // Executa a funcionalidade correspondente a opcao
        switch (opcao) {

            case 1:
                // Chama a funcao que configura os intervalos
                configurarIntervalos();
                break;

            case 2:
                // Funcionalidade de folgas ainda nao implementada
                printf("Configurar folgas\n");
                break;

            case 3:
                // Retorna ao menu principal do barbeiro
                printf("Voltando ao menu do barbeiro...\n");
                break;

            default:
                // Informa quando a opcao nao existe
                printf("Opcao invalida!\n");
                break;
        }
    }
}


// ============================================================
// CONFIGURAR INTERVALOS DO BARBEIRO
// ============================================================

// Funcao que permite cadastrar um intervalo durante o expediente
void configurarIntervalos(void) {

    // Armazena os horarios informados pelo barbeiro
    int horaInicioIntervalos;
    int horaFimIntervalos;

    // Armazena o dia da semana escolhido
    int diaSemana;

    // Armazena o inicio e o fim do expediente cadastrado
    int inicioExpediente;
    int fimExpediente;

    // Controla se existe expediente cadastrado para o dia
    // 0 = nao encontrado | 1 = encontrado
    int encontrouExpediente = 0;

    // Estrutura que armazenara os dados do novo intervalo
    struct IntervaloBarbeiro intervalo;

    // Estrutura utilizada para ler os expedientes salvos
    struct HorarioBarbeiro horario;

    // Ponteiro utilizado para ler o arquivo de horarios
    FILE *arquivoHorario;

    // Ponteiro utilizado para salvar o arquivo de intervalos
    FILE *arquivoIntervalo;


    // ========================================================
    // ESCOLHER O DIA DA SEMANA
    // ========================================================

    // Exibe os dias disponiveis para configuracao
    printf("Escolha o dia da semana:\n");
    printf("1 - Segunda-feira\n");
    printf("2 - Terca-feira\n");
    printf("3 - Quarta-feira\n");
    printf("4 - Quinta-feira\n");
    printf("5 - Sexta-feira\n");
    printf("6 - Sabado\n");
    printf("7 - Domingo\n");

    // Solicita o dia desejado
    printf("Digite o dia: ");

    // Verifica se o valor informado e numerico
    if (scanf("%d", &diaSemana) != 1) {

        printf("Digite apenas numeros!\n");

        // Descarta os caracteres invalidos
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        // Encerra a configuracao
        return;
    }

    // Verifica se o dia esta entre segunda e domingo
    if (diaSemana < 1 || diaSemana > 7) {

        printf("Dia da semana invalido!\n");
        return;
    }


    // ========================================================
    // INFORMAR OS HORARIOS DO INTERVALO
    // ========================================================

    // Solicita a hora em que o intervalo comeca
    printf("Digite a hora de inicio do intervalo: ");

    // Verifica se a entrada e um numero inteiro
    if (scanf("%d", &horaInicioIntervalos) != 1) {

        printf("Digite apenas numeros!\n");

        // Limpa a entrada invalida
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    // Solicita a hora em que o intervalo termina
    printf("Digite a hora do fim do intervalo: ");

    // Verifica se a entrada e um numero inteiro
    if (scanf("%d", &horaFimIntervalos) != 1) {

        printf("Digite apenas numeros!\n");

        // Limpa a entrada invalida
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }


    // ========================================================
    // VALIDAR OS HORARIOS INFORMADOS
    // ========================================================

    // Impede horas iniciais menores que 0 ou maiores que 23
    if (horaInicioIntervalos < 0 || horaInicioIntervalos > 23) {

        printf("Hora de inicio invalida!\n");
        return;
    }

    // Impede horas finais menores que 0 ou maiores que 23
    if (horaFimIntervalos < 0 || horaFimIntervalos > 23) {

        printf("Hora de fim invalida!\n");
        return;
    }

    // Verifica se o termino e posterior ao inicio
    // Exemplo invalido: intervalo das 14h as 12h
    if (horaFimIntervalos <= horaInicioIntervalos) {

        printf("Horario invalido!\n");
        return;
    }


    // ========================================================
    // CONSULTAR O EXPEDIENTE DO BARBEIRO
    // ========================================================

    // Abre o arquivo que contem os expedientes cadastrados
    // "rb" significa leitura de arquivo binario
    arquivoHorario = fopen("horario.dat", "rb");

    // Verifica se foi possivel abrir o arquivo
    if (arquivoHorario == NULL) {

        printf("Erro ao abrir o arquivo de horarios!\n");
        return;
    }

    // Percorre todos os expedientes salvos no arquivo
    // fread le um registro da estrutura por vez
    while (fread(&horario, sizeof(struct HorarioBarbeiro),
                 1, arquivoHorario) == 1) {

        // Verifica se o expediente pertence ao dia escolhido
        if (horario.dia == diaSemana) {

            // Guarda o inicio e o fim do expediente encontrado
            inicioExpediente = horario.horaInicio;
            fimExpediente = horario.horaFim;

            // Indica que encontramos um expediente para o dia
            encontrouExpediente = 1;

            printf("Expediente encontrado!\n");
        }
    }

    // Fecha o arquivo apos terminar a leitura
    fclose(arquivoHorario);


    // ========================================================
    // VERIFICAR SE EXISTE EXPEDIENTE CADASTRADO
    // ========================================================

    // Impede cadastrar intervalos em dias sem expediente
    if (encontrouExpediente == 0) {

        printf("Nenhum expediente cadastrado nesse dia!\n");
        return;
    }


    // ========================================================
    // VERIFICAR SE O INTERVALO ESTA DENTRO DO EXPEDIENTE
    // ========================================================

    // Impede intervalos que comecam antes do expediente
    // ou terminam depois do horario de trabalho
    if (horaInicioIntervalos < inicioExpediente ||
        horaFimIntervalos > fimExpediente) {

        printf("Intervalo fora do expediente!\n");
        return;
    }


    // ========================================================
    // PREENCHER A ESTRUTURA DO INTERVALO
    // ========================================================

    // Armazena o dia e os horarios informados na estrutura
    intervalo.dia = diaSemana;
    intervalo.horaInicio = horaInicioIntervalos;
    intervalo.horaFim = horaFimIntervalos;


    // ========================================================
    // SALVAR O INTERVALO EM ARQUIVO BINARIO
    // ========================================================

    // Abre o arquivo no modo "ab"
    // Esse modo adiciona registros sem apagar os anteriores
    arquivoIntervalo = fopen("intervalos.dat", "ab");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivoIntervalo == NULL) {

        printf("Erro ao abrir o arquivo de intervalos!\n");
        return;
    }

    // Grava a estrutura completa no arquivo binario
    // fwrite retorna a quantidade de registros gravados
    if (fwrite(&intervalo, sizeof(struct IntervaloBarbeiro),
               1, arquivoIntervalo) != 1) {

        printf("Erro ao salvar o intervalo!\n");

        // Fecha o arquivo antes de encerrar a funcao
        fclose(arquivoIntervalo);
        return;
    }

    // Fecha o arquivo e verifica se houve erro no fechamento
    if (fclose(arquivoIntervalo) != 0) {

        printf("Erro ao finalizar o arquivo de intervalos!\n");
        return;
    }

    // Exibe a confirmacao somente apos salvar os dados
    printf("Intervalo configurado com sucesso!\n");
}
