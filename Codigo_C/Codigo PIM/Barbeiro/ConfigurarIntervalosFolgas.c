
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

// Nova funcao: configura um dia fixo de folga por semana
static void configurarFolgaSemanal(void);


// ============================================================
// MENU DE INTERVALOS E FOLGAS
// ============================================================

void configurarIntervalosFolgas(void) {

    int opcao = 0;

    // Mantem o menu aberto ate escolher voltar
    while (opcao != 4) {

        printf("\n===== INTERVALOS E FOLGAS =====\n");
        printf("1 - Configurar intervalos\n");
        printf("2 - Configurar folga por data\n");
        printf("3 - Configurar folga semanal\n");
        printf("4 - Voltar\n");
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
                // Configura o intervalo de um dia
                configurarIntervalos();
                break;

            case 2:
                // Mantem a folga por data como ja funcionava
                configurarFolgas();
                break;

            case 3:
                // Configura a folga fixa da semana
                configurarFolgaSemanal();
                break;

            case 4:
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

        // Verifica se o atendimento cai no periodo informado
        if (diaAgendamento == diaSemana &&
            agendamento.hora >= horaInicio &&
            agendamento.hora < horaFim) {

            printf("\nExiste um agendamento nesse periodo!\n");

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
// CONFIGURAR FOLGAS POR DATA
// ============================================================

// Esta funcao foi mantida como ja estava.
// O arquivo folgas.dat continua separado da folga semanal.

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
// VISUALIZAR FOLGAS CADASTRADAS POR DATA
// ============================================================

// Continua mostrando apenas as folgas por data.
// A folga semanal NAO entra nesta lista.

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


// ============================================================
// CONFIGURAR FOLGA SEMANAL FIXA
// ============================================================

// Esta funcao e nova.
//
// Permite escolher um unico dia fixo de folga por semana.
// Exemplo: domingo, que corresponde ao numero 7.
//
// A folga semanal e gravada em folga_semanal.dat.
// O arquivo folgas.dat nao e modificado.

static void configurarFolgaSemanal(void) {

    int diaSemana;
    int confirmar;
    int diaAtual = 0;

    FILE *arquivo;

    const char *nomesDias[] = {
        "",
        "Segunda-feira",
        "Terca-feira",
        "Quarta-feira",
        "Quinta-feira",
        "Sexta-feira",
        "Sabado",
        "Domingo"
    };

    printf("\n===== CONFIGURAR FOLGA SEMANAL =====\n");

    // Exibe todos os dias da semana
    for (int i = 1; i <= 7; i++) {

        printf("%d - %s\n", i, nomesDias[i]);
    }

    // Solicita um unico dia para ser folga fixa
    printf("Escolha o dia de folga semanal: ");

    if (scanf("%d", &diaSemana) != 1) {

        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    // Impede dias fora de 1 a 7
    if (diaSemana < 1 || diaSemana > 7) {

        printf("Dia da semana invalido!\n");
        return;
    }

    // ========================================================
    // CONSULTAR A FOLGA SEMANAL ATUAL
    // ========================================================

    arquivo = fopen("folga_semanal.dat", "rb");

    if (arquivo != NULL) {

        // Le o dia que ja esta cadastrado
        if (fread(&diaAtual, sizeof(int), 1, arquivo) != 1) {

            printf("Erro ao ler a folga semanal!\n");
            fclose(arquivo);
            return;
        }

        if (fclose(arquivo) != 0) {

            printf("Erro ao fechar o arquivo de folga semanal!\n");
            return;
        }

        // Verifica se o registro e valido
        if (diaAtual < 1 || diaAtual > 7) {

            printf("Folga semanal salva esta invalida!\n");
            return;
        }

        // Impede salvar novamente a mesma configuracao
        if (diaAtual == diaSemana) {

            printf("%s ja e a folga semanal cadastrada!\n",
                   nomesDias[diaSemana]);

            return;
        }

        printf("\nFolga semanal atual: %s\n",
               nomesDias[diaAtual]);

    } else if (errno != ENOENT) {

        printf("Erro ao consultar a folga semanal!\n");
        return;
    }

    // ========================================================
    // VERIFICAR AGENDAMENTOS FUTUROS
    // ========================================================

    // Verifica todas as horas do dia escolhido.
    // Assim, nao permite transformar em folga um dia que
    // ja tenha atendimentos agendados.

    int conflito = verificarConflitoAgendamentoIntervalo(
        diaSemana,
        0,
        24
    );

    if (conflito == 1) {

        printf("\nNao foi possivel configurar essa folga!\n");

        printf("Existem agendamentos futuros nesse dia da semana.\n");

        return;
    }

    if (conflito == -1) {

        printf("Nao foi possivel verificar os agendamentos!\n");
        return;
    }

    // ========================================================
    // CONFIRMAR A FOLGA SEMANAL
    // ========================================================

    printf("\n===== CONFIRMAR FOLGA SEMANAL =====\n");

    printf("Dia escolhido: %s\n", nomesDias[diaSemana]);

    printf("Esse dia sera folga toda semana.\n");

    printf("1 - Confirmar\n");
    printf("2 - Cancelar\n");
    printf("Escolha uma opcao: ");

    if (scanf("%d", &confirmar) != 1) {

        printf("Entrada invalida!\n");
        return;
    }

    if (confirmar != 1) {

        printf("Alteracao cancelada!\n");
        return;
    }

    // ========================================================
    // VERIFICAR SE EXISTE BACKUP PENDENTE
    // ========================================================

    FILE *backup = fopen("folga_semanal_backup.dat", "rb");

    if (backup != NULL) {

        fclose(backup);

        printf("Existe um backup pendente da folga semanal!\n");
        printf("Verifique o backup antes de continuar.\n");

        return;
    }

    if (errno != ENOENT) {

        perror("Erro ao verificar backup");
        return;
    }

    // ========================================================
    // CRIAR ARQUIVO TEMPORARIO
    // ========================================================

    // Grava primeiro em um arquivo temporario,
    // para nao apagar a configuracao anterior em caso de erro.

    FILE *temporario = fopen("folga_semanal_temp.dat", "wb");

    if (temporario == NULL) {

        printf("Erro ao criar arquivo temporario!\n");
        return;
    }

    // Guarda apenas o numero do dia da semana
    if (fwrite(&diaSemana, sizeof(int), 1, temporario) != 1) {

        printf("Erro ao gravar a folga semanal!\n");

        fclose(temporario);
        remove("folga_semanal_temp.dat");

        return;
    }

    if (fclose(temporario) != 0) {

        printf("Erro ao finalizar arquivo temporario!\n");

        remove("folga_semanal_temp.dat");
        return;
    }

    // ========================================================
    // VERIFICAR SE JA EXISTE FOLGA SEMANAL
    // ========================================================

    int existeOriginal = 0;

    FILE *original = fopen("folga_semanal.dat", "rb");

    if (original != NULL) {

        existeOriginal = 1;
        fclose(original);

    } else if (errno != ENOENT) {

        printf("Erro ao verificar a folga semanal anterior!\n");

        remove("folga_semanal_temp.dat");
        return;
    }

    // ========================================================
    // PRESERVAR A FOLGA SEMANAL ANTERIOR
    // ========================================================

    if (existeOriginal == 1) {

        if (rename("folga_semanal.dat",
                   "folga_semanal_backup.dat") != 0) {

            printf("Erro ao criar backup da folga semanal!\n");

            remove("folga_semanal_temp.dat");
            return;
        }
    }

    // ========================================================
    // SALVAR NOVA FOLGA SEMANAL
    // ========================================================

    if (rename("folga_semanal_temp.dat",
               "folga_semanal.dat") != 0) {

        printf("Erro ao salvar a folga semanal!\n");

        // Tenta restaurar a configuracao anterior
        if (existeOriginal == 1) {

            if (rename("folga_semanal_backup.dat",
                       "folga_semanal.dat") != 0) {

                printf("ATENCAO: restaure a folga pelo backup!\n");
            }
        }

        return;
    }

    // Remove o backup apos salvar com sucesso
    if (existeOriginal == 1) {

        if (remove("folga_semanal_backup.dat") != 0) {

            printf("Aviso: nao foi possivel remover o backup!\n");
        }
    }

    printf("\nFolga semanal configurada com sucesso!\n");

    printf("Dia: %s\n", nomesDias[diaSemana]);
}
