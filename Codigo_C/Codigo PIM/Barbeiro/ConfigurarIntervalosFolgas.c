
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "HorarioBarbeiro.h"
#include "IntervaloBarbeiro.h"
#include "FolgaBarbeiro.h"
#include "../Agendamento/Agendamento.h"
#include "../Validacao/ValidarData.h"

// Prototipos das funcoes
void configurarIntervalos(void);
void configurarFolgas(void);

// ============================================================
// MENU DE INTERVALOS E FOLGAS
// ============================================================

void configurarIntervalosFolgas(void) {

    int opcao = 0;

    while (opcao != 3) {

        printf("\n===== INTERVALOS E FOLGAS =====\n");
        printf("1 - Configurar intervalos\n");
        printf("2 - Configurar folgas\n");
        printf("3 - Voltar\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {

            printf("Digite apenas numeros!\n");

            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            if (c == EOF) {
                return;
            }

            opcao = 0;
            continue;
        }

        switch (opcao) {

            case 1:
                configurarIntervalos();
                break;

            case 2:
                configurarFolgas();
                break;

            case 3:
                printf("Voltando ao menu anterior...\n");
                break;

            default:
                printf("Opcao invalida!\n");
                break;
        }
    }
}

// ============================================================
// VERIFICAR CONFLITOS COM AGENDAMENTOS
// ============================================================

// Retorna:
// 1 = existe conflito
// 0 = nenhum conflito
// -1 = erro na verificacao

static int verificarConflitoAgendamentoIntervalo(
    int diaSemana, int horaInicio, int horaFim) {

    FILE *arquivo;
    struct Agendamento agendamento;

    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);

    if (dataAtual == NULL) {
        printf("Erro ao consultar a data atual!\n");
        return -1;
    }

    int hoje = (dataAtual->tm_year + 1900) * 10000
             + (dataAtual->tm_mon + 1) * 100
             + dataAtual->tm_mday;

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {

        if (errno == ENOENT) {
            return 0;
        }

        printf("Erro ao abrir os agendamentos!\n");
        return -1;
    }

    while (fread(&agendamento,
                 sizeof(struct Agendamento),
                 1, arquivo) == 1) {

        int dataAgendamento = agendamento.ano * 10000
                           + agendamento.mes * 100
                           + agendamento.dia;

        // Ignora atendimentos de datas anteriores
        if (dataAgendamento < hoje) {
            continue;
        }

        // Descobre o dia da semana do agendamento
        struct tm data = {0};

        data.tm_mday = agendamento.dia;
        data.tm_mon = agendamento.mes - 1;
        data.tm_year = agendamento.ano - 1900;
        data.tm_isdst = -1;

        if (mktime(&data) == (time_t)-1) {

            printf("Erro ao verificar data do agendamento!\n");
            fclose(arquivo);
            return -1;
        }

        int diaAgendamento = data.tm_wday;

        // Domingo no sistema corresponde ao dia 7
        if (diaAgendamento == 0) {
            diaAgendamento = 7;
        }

        // Verifica se o atendimento cai no intervalo
        if (diaAgendamento == diaSemana &&
            agendamento.hora >= horaInicio &&
            agendamento.hora < horaFim) {

            printf("\nExiste um agendamento nesse intervalo!\n");

            printf("Data: %02d/%02d/%04d\n",
                   agendamento.dia,
                   agendamento.mes,
                   agendamento.ano);

            printf("Horario: %02d:00\n", agendamento.hora);

            fclose(arquivo);
            return 1;
        }
    }

    if (ferror(arquivo)) {

        printf("Erro ao ler os agendamentos!\n");
        fclose(arquivo);
        return -1;
    }

    if (fclose(arquivo) != 0) {

        printf("Erro ao fechar os agendamentos!\n");
        return -1;
    }

    return 0;
}

// ============================================================
// CONFIGURAR INTERVALOS DO BARBEIRO
// ============================================================

void configurarIntervalos(void) {

    int horaInicioIntervalos;
    int horaFimIntervalos;
    int diaSemana;

    int inicioExpediente = 0;
    int fimExpediente = 0;
    int encontrouExpediente = 0;

    struct IntervaloBarbeiro intervalo;
    struct IntervaloBarbeiro intervaloSalvo;
    struct HorarioBarbeiro horario;

    FILE *arquivoHorario;
    FILE *arquivoIntervalo;

    // ========================================================
    // ESCOLHER DIA DA SEMANA
    // ========================================================

    printf("\nEscolha o dia da semana:\n");
    printf("1 - Segunda-feira\n");
    printf("2 - Terca-feira\n");
    printf("3 - Quarta-feira\n");
    printf("4 - Quinta-feira\n");
    printf("5 - Sexta-feira\n");
    printf("6 - Sabado\n");
    printf("7 - Domingo\n");
    printf("Digite o dia: ");

    if (scanf("%d", &diaSemana) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (diaSemana < 1 || diaSemana > 7) {

        printf("Dia da semana invalido!\n");
        return;
    }

    // ========================================================
    // INFORMAR HORARIOS DO INTERVALO
    // ========================================================

    printf("Digite a hora de inicio do intervalo: ");

    if (scanf("%d", &horaInicioIntervalos) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    printf("Digite a hora do fim do intervalo: ");

    if (scanf("%d", &horaFimIntervalos) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    // ========================================================
    // VALIDAR HORARIOS
    // ========================================================

    if (horaInicioIntervalos < 0 ||
        horaInicioIntervalos > 23) {

        printf("Hora de inicio invalida!\n");
        return;
    }

    if (horaFimIntervalos < 0 ||
        horaFimIntervalos > 23) {

        printf("Hora de fim invalida!\n");
        return;
    }

    if (horaFimIntervalos <= horaInicioIntervalos) {

        printf("Horario invalido!\n");
        return;
    }

    // ========================================================
    // CONSULTAR EXPEDIENTE
    // ========================================================

    arquivoHorario = fopen("horario.dat", "rb");

    if (arquivoHorario == NULL) {

        printf("Erro ao abrir o arquivo de horarios!\n");
        return;
    }

    while (fread(&horario,
                 sizeof(struct HorarioBarbeiro),
                 1, arquivoHorario) == 1) {

        if (horario.dia == diaSemana) {

            inicioExpediente = horario.horaInicio;
            fimExpediente = horario.horaFim;

            encontrouExpediente = 1;

            printf("Expediente encontrado!\n");
        }
    }

    if (ferror(arquivoHorario)) {

        printf("Erro ao consultar os horarios!\n");
        fclose(arquivoHorario);
        return;
    }

    if (fclose(arquivoHorario) != 0) {

        printf("Erro ao fechar o arquivo de horarios!\n");
        return;
    }

    if (encontrouExpediente == 0) {

        printf("Nenhum expediente cadastrado nesse dia!\n");
        return;
    }

    // ========================================================
    // VERIFICAR SE O INTERVALO ESTA DENTRO DO EXPEDIENTE
    // ========================================================

    if (horaInicioIntervalos < inicioExpediente ||
        horaFimIntervalos > fimExpediente) {

        printf("Intervalo fora do expediente!\n");
        return;
    }

    // ========================================================
    // VERIFICAR CONFLITOS COM AGENDAMENTOS
    // ========================================================

    int conflito = verificarConflitoAgendamentoIntervalo(
        diaSemana,
        horaInicioIntervalos,
        horaFimIntervalos
    );

    if (conflito == 1) {

        printf("Intervalo cancelado por conflito de agendamento!\n");
        return;
    }

    if (conflito == -1) {

        printf("Nao foi possivel verificar os agendamentos!\n");
        return;
    }

    // ========================================================
    // PREENCHER ESTRUTURA DO INTERVALO
    // ========================================================

    intervalo.dia = diaSemana;
    intervalo.horaInicio = horaInicioIntervalos;
    intervalo.horaFim = horaFimIntervalos;

    // ========================================================
    // SALVAR OU ATUALIZAR INTERVALO SEM DUPLICAR
    // ========================================================

    long posicaoEncontrada = -1;
    int quantidadeEncontrada = 0;

    // Abre o arquivo para leitura e atualizacao
    arquivoIntervalo = fopen("intervalos.dat", "rb+");

    if (arquivoIntervalo != NULL) {

        while (fread(&intervaloSalvo,
                     sizeof(struct IntervaloBarbeiro),
                     1, arquivoIntervalo) == 1) {

            if (intervaloSalvo.dia == diaSemana) {

                quantidadeEncontrada++;

                // Guarda a posicao do registro encontrado
                long posicao = ftell(arquivoIntervalo);

                if (posicao < 0) {

                    printf("Erro ao localizar o intervalo!\n");
                    fclose(arquivoIntervalo);
                    return;
                }

                posicaoEncontrada =
                    posicao - (long)sizeof(struct IntervaloBarbeiro);
            }
        }

        if (ferror(arquivoIntervalo)) {

            printf("Erro ao consultar os intervalos!\n");
            fclose(arquivoIntervalo);
            return;
        }

        // Bloqueia registros ja duplicados
        if (quantidadeEncontrada > 1) {

            printf("Ha intervalos duplicados nesse dia!\n");
            printf("Corrija os registros antes de editar!\n");

            fclose(arquivoIntervalo);
            return;
        }

        // ====================================================
        // ATUALIZAR INTERVALO EXISTENTE
        // ====================================================

        if (quantidadeEncontrada == 1) {

            if (fseek(arquivoIntervalo,
                      posicaoEncontrada,
                      SEEK_SET) != 0) {

                printf("Erro ao localizar o intervalo!\n");
                fclose(arquivoIntervalo);
                return;
            }

            if (fwrite(&intervalo,
                       sizeof(struct IntervaloBarbeiro),
                       1, arquivoIntervalo) != 1) {

                printf("Erro ao atualizar o intervalo!\n");
                fclose(arquivoIntervalo);
                return;
            }

            if (fclose(arquivoIntervalo) != 0) {

                printf("Erro ao finalizar a atualizacao!\n");
                return;
            }

            printf("Intervalo atualizado com sucesso!\n");
            return;
        }

        if (fclose(arquivoIntervalo) != 0) {

            printf("Erro ao fechar o arquivo de intervalos!\n");
            return;
        }

    } else if (errno != ENOENT) {

        printf("Erro ao abrir o arquivo de intervalos!\n");
        return;
    }

    // ========================================================
    // CADASTRAR NOVO INTERVALO
    // ========================================================

    // So chega aqui se nao existir intervalo nesse dia
    arquivoIntervalo = fopen("intervalos.dat", "ab");

    if (arquivoIntervalo == NULL) {

        printf("Erro ao abrir o arquivo de intervalos!\n");
        return;
    }

    if (fwrite(&intervalo,
               sizeof(struct IntervaloBarbeiro),
               1, arquivoIntervalo) != 1) {

        printf("Erro ao salvar o intervalo!\n");
        fclose(arquivoIntervalo);
        return;
    }

    if (fclose(arquivoIntervalo) != 0) {

        printf("Erro ao finalizar o arquivo de intervalos!\n");
        return;
    }

    printf("Intervalo configurado com sucesso!\n");
}

// ============================================================
// CONFIGURAR FOLGAS DO BARBEIRO
// ============================================================

void configurarFolgas(void) {

    int dia;
    int mes;
    int ano;

    struct FolgaBarbeiro folga;
    struct FolgaBarbeiro folgalida;

    FILE *arquivoFolga;
    FILE *arquivoConsulta;

    printf("\n===== CONFIGURAR FOLGAS =====\n");

    // Solicita o dia
    printf("Digite o dia de folga: ");

    if (scanf("%d", &dia) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (dia < 1 || dia > 31) {

        printf("Dia invalido!\n");
        return;
    }

    // Solicita o mes
    printf("Digite o mes da folga: ");

    if (scanf("%d", &mes) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (mes < 1 || mes > 12) {

        printf("Mes invalido!\n");
        return;
    }

    // Solicita o ano
    printf("Digite o ano da folga: ");

    if (scanf("%d", &ano) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (ano < 1900 || ano > 9999) {

        printf("Ano invalido!\n");
        return;
    }

    // Valida a data completa
    if (validarData(dia, mes, ano) == 0) {

        printf("Data de folga invalida!\n");
        return;
    }

    // ========================================================
    // VERIFICAR SE A FOLGA JA EXISTE
    // ========================================================

    arquivoConsulta = fopen("folgas.dat", "rb");

    if (arquivoConsulta != NULL) {

        while (fread(&folgalida,
                     sizeof(struct FolgaBarbeiro),
                     1, arquivoConsulta) == 1) {

            if (folgalida.dia == dia &&
                folgalida.mes == mes &&
                folgalida.ano == ano) {

                printf("Ja existe uma folga cadastrada nessa data!\n");

                fclose(arquivoConsulta);
                return;
            }
        }

        fclose(arquivoConsulta);
    }

    // ========================================================
    // SALVAR FOLGA
    // ========================================================

    folga.dia = dia;
    folga.mes = mes;
    folga.ano = ano;

    arquivoFolga = fopen("folgas.dat", "ab");

    if (arquivoFolga == NULL) {

        printf("Erro ao abrir o arquivo de folgas!\n");
        return;
    }

    if (fwrite(&folga,
               sizeof(struct FolgaBarbeiro),
               1, arquivoFolga) != 1) {

        printf("Erro ao salvar a folga!\n");
        fclose(arquivoFolga);
        return;
    }

    if (fclose(arquivoFolga) != 0) {

        printf("Erro ao finalizar o arquivo de folgas!\n");
        return;
    }

    printf("Folga confirmada com sucesso!\n");
}

// ============================================================
// VISUALIZAR INTERVALOS CADASTRADOS
// ============================================================

void visualizarIntervalosBarbeiro(void) {

    FILE *arquivo;
    struct IntervaloBarbeiro intervalo;

    int quantidade = 0;

    const char *diasSemana[] = {
        "Segunda-feira",
        "Terca-feira",
        "Quarta-feira",
        "Quinta-feira",
        "Sexta-feira",
        "Sabado",
        "Domingo"
    };

    printf("\n===== INTERVALOS CADASTRADOS =====\n");

    arquivo = fopen("intervalos.dat", "rb");

    if (arquivo == NULL) {

        printf("Nenhum intervalo cadastrado!\n");
        return;
    }

    while (fread(&intervalo,
                 sizeof(struct IntervaloBarbeiro),
                 1, arquivo) == 1) {

        if (intervalo.dia >= 1 && intervalo.dia <= 7) {

            quantidade++;

            printf("\nDia: %s\n",
                   diasSemana[intervalo.dia - 1]);

            printf("Inicio: %02d:00\n", intervalo.horaInicio);
            printf("Termino: %02d:00\n", intervalo.horaFim);
            printf("------------------------------\n");
        }
    }

    if (quantidade == 0) {
        printf("Nenhum intervalo cadastrado!\n");
    }

    fclose(arquivo);
}

// ============================================================
// VISUALIZAR FOLGAS CADASTRADAS
// ============================================================

void visualizarFolgasBarbeiro(void) {

    FILE *arquivo;
    struct FolgaBarbeiro folga;

    int quantidade = 0;

    printf("\n===== FOLGAS CADASTRADAS =====\n");

    arquivo = fopen("folgas.dat", "rb");

    if (arquivo == NULL) {

        printf("Nenhuma folga cadastrada!\n");
        return;
    }

    while (fread(&folga,
                 sizeof(struct FolgaBarbeiro),
                 1, arquivo) == 1) {

        quantidade++;

        printf("\nFolga %d\n", quantidade);

        printf("Data: %02d/%02d/%04d\n",
               folga.dia,
               folga.mes,
               folga.ano);

        printf("------------------------------\n");
    }

    if (quantidade == 0) {
        printf("Nenhuma folga cadastrada!\n");
    }

    fclose(arquivo);
}
