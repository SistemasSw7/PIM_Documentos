
#include <stdio.h>
#include <time.h>
#include <errno.h>

#include "HorarioBarbeiro.h"
#include "IntervaloBarbeiro.h"
#include "../Agendamento/Agendamento.h"

// ============================================================
// NOMES DOS ARQUIVOS
// ============================================================

#define ARQUIVO_HORARIOS "horario.dat"
#define ARQUIVO_TEMP "horario_semanal_temp.dat"
#define ARQUIVO_BACKUP "horario_semanal_backup.dat"

// Dias da semana: 1 = segunda e 7 = domingo
static const char *nomesDias[] = {
    "",
    "Segunda-feira",
    "Terca-feira",
    "Quarta-feira",
    "Quinta-feira",
    "Sexta-feira",
    "Sabado",
    "Domingo"
};

// Prototipo da configuracao semanal
static void configurarHorariosSemana(void);


// ============================================================
// FUNCAO AUXILIAR - LER UM NUMERO
// ============================================================

static int lerInteiro(const char *mensagem, int *valor) {

    printf("%s", mensagem);

    if (scanf("%d", valor) != 1) {

        printf("Digite apenas numeros!\n");

        // Limpa os caracteres invalidos da entrada
        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return 0;
    }

    return 1;
}


// ============================================================
// FUNCAO AUXILIAR - VALIDAR EXPEDIENTE
// ============================================================

static int solicitarExpediente(int *horaInicio, int *horaFim) {

    // Solicita o horario de inicio
    if (!lerInteiro(
            "Digite a hora de inicio (0 a 23): ",
            horaInicio)) {

        return 0;
    }

    // Solicita o horario de termino
    if (!lerInteiro(
            "Digite a hora de termino (0 a 23): ",
            horaFim)) {

        return 0;
    }

    // Verifica se os horarios sao validos
    if (*horaInicio < 0 || *horaInicio > 23 ||
        *horaFim < 0 || *horaFim > 23) {

        printf("Hora invalida!\n");
        return 0;
    }

    // O termino precisa ser maior que o inicio
    if (*horaFim <= *horaInicio) {

        printf("A hora de termino deve ser maior que a de inicio!\n");
        return 0;
    }

    return 1;
}


// ============================================================
// VERIFICAR CONFLITOS COM AGENDAMENTOS
// ============================================================

// Retornos:
//  0 = nenhum conflito
//  1 = existe conflito
// -1 = erro ao verificar

int verificarConflitoAgendamentos(
    int diaSemana,
    int horaInicio,
    int horaFim
) {

    // Consulta a data atual
    time_t agora = time(NULL);

    struct tm *dataAtual = localtime(&agora);

    if (dataAtual == NULL) {

        printf("Erro ao consultar a data atual!\n");
        return -1;
    }

    int diaAtual = dataAtual->tm_mday;
    int mesAtual = dataAtual->tm_mon + 1;
    int anoAtual = dataAtual->tm_year + 1900;

    // Abre os agendamentos cadastrados
    FILE *arquivo = fopen(
        "../Dados/agendamentos.dat", "rb"
    );

    if (arquivo == NULL) {

        // Se ainda nao existe arquivo, nao ha reservas
        if (errno == ENOENT) {
            return 0;
        }

        printf("Erro ao acessar os agendamentos!\n");
        return -1;
    }

    struct Agendamento agendamento;

    // Le cada agendamento existente
    while (fread(
        &agendamento,
        sizeof(struct Agendamento),
        1,
        arquivo
    ) == 1) {

        // Ignora agendamentos de datas anteriores
        if (agendamento.ano < anoAtual ||
            (agendamento.ano == anoAtual &&
             agendamento.mes < mesAtual) ||
            (agendamento.ano == anoAtual &&
             agendamento.mes == mesAtual &&
             agendamento.dia < diaAtual)) {

            continue;
        }

        // Monta a data do agendamento
        struct tm data = {0};

        data.tm_mday = agendamento.dia;
        data.tm_mon = agendamento.mes - 1;
        data.tm_year = agendamento.ano - 1900;
        data.tm_isdst = -1;

        // Descobre o dia da semana
        if (mktime(&data) == (time_t)-1) {

            printf("Erro ao verificar a data do agendamento!\n");

            fclose(arquivo);
            return -1;
        }

        int diaAgendamento = data.tm_wday;

        // Converte domingo de 0 para 7
        if (diaAgendamento == 0) {
            diaAgendamento = 7;
        }

        // Verifica apenas os agendamentos do dia escolhido
        if (diaAgendamento == diaSemana) {

            // Impede deixar uma reserva fora do expediente
            if (agendamento.hora < horaInicio ||
                agendamento.hora >= horaFim) {

                printf("\nExiste um agendamento que impede essa alteracao!\n");

                printf("Data: %02d/%02d/%04d\n",
                       agendamento.dia,
                       agendamento.mes,
                       agendamento.ano);

                printf("Horario: %02d:00\n",
                       agendamento.hora);

                fclose(arquivo);
                return 1;
            }
        }
    }

    // Verifica se houve erro na leitura
    if (ferror(arquivo)) {

        printf("Erro ao ler os agendamentos!\n");

        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);

    return 0;
}


// ============================================================
// VERIFICAR CONFLITOS COM INTERVALOS
// ============================================================

// Retornos:
//  0 = nenhum conflito
//  1 = existe conflito
// -1 = erro ao verificar

int verificarConflitoIntervalos(
    int diaSemana,
    int horaInicio,
    int horaFim
) {

    FILE *arquivo = fopen("intervalos.dat", "rb");

    if (arquivo == NULL) {

        // Nenhum intervalo foi cadastrado ainda
        if (errno == ENOENT) {
            return 0;
        }

        printf("Erro ao acessar os intervalos!\n");
        return -1;
    }

    struct IntervaloBarbeiro intervalo;

    // Percorre os intervalos cadastrados
    while (fread(
        &intervalo,
        sizeof(struct IntervaloBarbeiro),
        1,
        arquivo
    ) == 1) {

        // Verifica o intervalo do dia correspondente
        if (intervalo.dia == diaSemana) {

            // O intervalo precisa estar dentro do expediente
            if (intervalo.horaInicio < horaInicio ||
                intervalo.horaFim > horaFim) {

                printf("\nExiste um intervalo que impede essa alteracao!\n");

                printf("Dia: %s\n", nomesDias[diaSemana]);

                printf("Intervalo: %02d:00 as %02d:00\n",
                       intervalo.horaInicio,
                       intervalo.horaFim);

                fclose(arquivo);
                return 1;
            }
        }
    }

    // Verifica se houve erro de leitura
    if (ferror(arquivo)) {

        printf("Erro ao ler os intervalos!\n");

        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);

    return 0;
}


// ============================================================
// CARREGAR OS HORARIOS EXISTENTES
// ============================================================

// Guarda os horarios em um vetor de 8 posicoes.
// A posicao 0 nao e utilizada.
// As posicoes de 1 a 7 representam os dias da semana.

static int carregarHorarios(
    struct HorarioBarbeiro horarios[8],
    int diasCadastrados[8]
) {

    FILE *arquivo = fopen(ARQUIVO_HORARIOS, "rb");

    if (arquivo == NULL) {

        // Se nao existe arquivo, ainda nao ha horarios
        if (errno == ENOENT) {
            return 1;
        }

        printf("Erro ao abrir os horarios!\n");
        return 0;
    }

    struct HorarioBarbeiro horario;

    // Le os horarios cadastrados
    while (fread(
        &horario,
        sizeof(struct HorarioBarbeiro),
        1,
        arquivo
    ) == 1) {

        // Considera apenas dias validos
        if (horario.dia >= 1 && horario.dia <= 7) {

            // Se houver registros antigos duplicados,
            // mantem a configuracao encontrada por ultimo.
            horarios[horario.dia] = horario;

            diasCadastrados[horario.dia] = 1;
        }
    }

    if (ferror(arquivo)) {

        printf("Erro ao ler os horarios!\n");

        fclose(arquivo);
        return 0;
    }

    fclose(arquivo);

    return 1;
}


// ============================================================
// SALVAR OS HORARIOS SEM DUPLICAR DIAS
// ============================================================

// Salva primeiro em um arquivo temporario.
// Depois preserva o original em backup e substitui
// pelo novo arquivo.
//
// Assim, cada dia aparece no maximo uma vez.

static int salvarHorarios(
    const struct HorarioBarbeiro horarios[8],
    const int diasCadastrados[8]
) {

    // Verifica se existe algum backup pendente
    FILE *backup = fopen(ARQUIVO_BACKUP, "rb");

    if (backup != NULL) {

        fclose(backup);

        printf("Existe um backup pendente de horarios!\n");
        printf("Verifique o backup antes de continuar.\n");

        return 0;
    }

    if (errno != ENOENT) {

        perror("Erro ao verificar backup");
        return 0;
    }

    // Cria o arquivo temporario
    FILE *temporario = fopen(ARQUIVO_TEMP, "wb");

    if (temporario == NULL) {

        perror("Erro ao criar arquivo temporario");
        return 0;
    }

    // Grava os dias em ordem, de segunda a domingo
    for (int dia = 1; dia <= 7; dia++) {

        // Ignora dias que nao foram cadastrados
        if (diasCadastrados[dia] == 0) {
            continue;
        }

        // Grava exatamente um registro por dia
        if (fwrite(
            &horarios[dia],
            sizeof(struct HorarioBarbeiro),
            1,
            temporario
        ) != 1) {

            printf("Erro ao gravar horario!\n");

            fclose(temporario);
            remove(ARQUIVO_TEMP);

            return 0;
        }
    }

    // Confirma que a gravacao foi finalizada
    if (fclose(temporario) != 0) {

        printf("Erro ao finalizar os novos horarios!\n");

        remove(ARQUIVO_TEMP);
        return 0;
    }

    // Verifica se ja existe arquivo original
    int existeOriginal = 0;

    FILE *original = fopen(ARQUIVO_HORARIOS, "rb");

    if (original != NULL) {

        existeOriginal = 1;
        fclose(original);

    } else if (errno != ENOENT) {

        perror("Erro ao verificar horario.dat");

        remove(ARQUIVO_TEMP);
        return 0;
    }

    // Preserva o arquivo original antes da substituicao
    if (existeOriginal == 1) {

        if (rename(
            ARQUIVO_HORARIOS,
            ARQUIVO_BACKUP
        ) != 0) {

            printf("Erro ao criar backup dos horarios!\n");

            remove(ARQUIVO_TEMP);
            return 0;
        }
    }

    // Substitui o arquivo antigo pelo atualizado
    if (rename(
        ARQUIVO_TEMP,
        ARQUIVO_HORARIOS
    ) != 0) {

        printf("Erro ao salvar os novos horarios!\n");

        // Tenta restaurar o original
        if (existeOriginal == 1) {

            if (rename(
                ARQUIVO_BACKUP,
                ARQUIVO_HORARIOS
            ) != 0) {

                printf("ATENCAO: restaure o horario.dat pelo backup!\n");
            }
        }

        return 0;
    }

    // Remove o backup temporario apos salvar
    if (existeOriginal == 1) {

        if (remove(ARQUIVO_BACKUP) != 0) {
            printf("Aviso: nao foi possivel remover o backup!\n");
        }
    }

    return 1;
}


// ============================================================
// MENU - CONFIGURAR HORARIOS
// ============================================================

void configurarHorarios(void) {

    int opcao;

    printf("\n===== CONFIGURAR HORARIOS =====\n");

    printf("1 - Configurar um dia especifico\n");
    printf("2 - Configurar todos os dias da semana\n");
    printf("3 - Voltar\n");

    if (!lerInteiro("Escolha uma opcao: ", &opcao)) {
        return;
    }

    // Configuracao semanal
    if (opcao == 2) {

        configurarHorariosSemana();
        return;
    }

    // Voltar ao menu anterior
    if (opcao == 3) {
        return;
    }

    if (opcao != 1) {

        printf("Opcao invalida!\n");
        return;
    }

    // ========================================================
    // CONFIGURAR UM DIA ESPECIFICO
    // ========================================================

    struct HorarioBarbeiro horario;

    printf("\n===== CONFIGURAR UM DIA =====\n");

    for (int dia = 1; dia <= 7; dia++) {
        printf("%d - %s\n", dia, nomesDias[dia]);
    }

    // Solicita o dia
    if (!lerInteiro(
        "Escolha o dia da semana: ",
        &horario.dia
    )) {
        return;
    }

    // Valida o dia informado
    if (horario.dia < 1 || horario.dia > 7) {

        printf("Dia invalido!\n");
        return;
    }

    // Solicita e valida as horas de trabalho
    if (!solicitarExpediente(
        &horario.horaInicio,
        &horario.horaFim
    )) {
        return;
    }

    // ========================================================
    // VERIFICAR CONFLITOS
    // ========================================================

    int conflito = verificarConflitoAgendamentos(
        horario.dia,
        horario.horaInicio,
        horario.horaFim
    );

    if (conflito != 0) {

        printf("Alteracao cancelada por conflito ou erro nos agendamentos!\n");
        return;
    }

    int conflitoIntervalo = verificarConflitoIntervalos(
        horario.dia,
        horario.horaInicio,
        horario.horaFim
    );

    if (conflitoIntervalo != 0) {

        printf("Alteracao cancelada por conflito ou erro nos intervalos!\n");
        return;
    }

    // ========================================================
    // CARREGAR E ATUALIZAR O DIA
    // ========================================================

    struct HorarioBarbeiro horarios[8];

    int diasCadastrados[8] = {0};

    if (!carregarHorarios(
        horarios,
        diasCadastrados
    )) {
        return;
    }

    // Verifica se o dia ja estava cadastrado
    int jaExistia = diasCadastrados[horario.dia];

    // Cria ou substitui o horario do dia escolhido
    horarios[horario.dia] = horario;

    diasCadastrados[horario.dia] = 1;

    // Salva os horarios atualizados
    if (!salvarHorarios(
        horarios,
        diasCadastrados
    )) {
        return;
    }

    if (jaExistia == 1) {
        printf("\nHorario atualizado com sucesso!\n");
    } else {
        printf("\nHorario cadastrado com sucesso!\n");
    }

    printf("Dia: %s\n", nomesDias[horario.dia]);

    printf("Inicio: %02d:00\n", horario.horaInicio);
    printf("Termino: %02d:00\n", horario.horaFim);
}


// ============================================================
// CONFIGURAR OS SETE DIAS DA SEMANA
// ============================================================

static void configurarHorariosSemana(void) {

    int horaInicio;
    int horaFim;
    int confirmar;

    printf("\n===== CONFIGURAR HORARIO SEMANAL =====\n");

    printf("Serao configurados os 7 dias da semana.\n");

    // Solicita o mesmo expediente para todos os dias
    if (!solicitarExpediente(
        &horaInicio,
        &horaFim
    )) {
        return;
    }

    // ========================================================
    // VERIFICAR CONFLITOS NOS SETE DIAS
    // ========================================================

    for (int dia = 1; dia <= 7; dia++) {

        // Verifica agendamentos do dia
        int conflito = verificarConflitoAgendamentos(
            dia,
            horaInicio,
            horaFim
        );

        if (conflito != 0) {

            printf("\nAlteracao semanal cancelada!\n");
            printf("Verifique os agendamentos de %s.\n",
                   nomesDias[dia]);

            return;
        }

        // Verifica os intervalos do dia
        int conflitoIntervalo = verificarConflitoIntervalos(
            dia,
            horaInicio,
            horaFim
        );

        if (conflitoIntervalo != 0) {

            printf("\nAlteracao semanal cancelada!\n");
            printf("Verifique os intervalos de %s.\n",
                   nomesDias[dia]);

            return;
        }
    }

    // ========================================================
    // CONFIRMAR A ALTERACAO SEMANAL
    // ========================================================

    printf("\n===== RESUMO DA SEMANA =====\n");

    for (int dia = 1; dia <= 7; dia++) {

        printf("%s: %02d:00 as %02d:00\n",
               nomesDias[dia],
               horaInicio,
               horaFim);
    }

    printf("\nAplicar esse expediente aos 7 dias?\n");
    printf("1 - Sim\n");
    printf("2 - Nao\n");

    if (!lerInteiro(
        "Escolha uma opcao: ",
        &confirmar
    )) {
        return;
    }

    if (confirmar != 1) {

        printf("Alteracao cancelada!\n");
        return;
    }

    // ========================================================
    // PREPARAR OS SETE HORARIOS
    // ========================================================

    struct HorarioBarbeiro horarios[8];

    int diasCadastrados[8] = {0};

    // Cria um registro para cada dia
    for (int dia = 1; dia <= 7; dia++) {

        horarios[dia].dia = dia;

        horarios[dia].horaInicio = horaInicio;

        horarios[dia].horaFim = horaFim;

        diasCadastrados[dia] = 1;
    }

    // ========================================================
    // SALVAR TODOS OS DIAS DE UMA VEZ
    // ========================================================

    if (!salvarHorarios(
        horarios,
        diasCadastrados
    )) {

        printf("Nao foi possivel salvar a semana!\n");
        return;
    }

    printf("\nHorarios semanais atualizados com sucesso!\n");

    printf("Dias atualizados: 7\n");

    printf("Inicio: %02d:00\n", horaInicio);
    printf("Termino: %02d:00\n", horaFim);
}


// ============================================================
// CONSULTAR OS HORARIOS DE TRABALHO
// ============================================================


void visualizarHorariosBarbeiro(void) {

    const char *diasSemana[] = {
        "Segunda-feira",
        "Terca-feira",
        "Quarta-feira",
        "Quinta-feira",
        "Sexta-feira",
        "Sabado",
        "Domingo"
    };

    struct HorarioBarbeiro horarios[8] = {0};

    int cadastrados[8] = {0};
    int diaFolgaSemanal = 0;
    int quantidade = 0;

    // ========================================================
    // CONSULTAR A FOLGA SEMANAL
    // ========================================================

    FILE *arquivoFolga = fopen("folga_semanal.dat", "rb");

    if (arquivoFolga != NULL) {

        if (fread(&diaFolgaSemanal,
                  sizeof(int),
                  1,
                  arquivoFolga) != 1 ||
            diaFolgaSemanal < 1 ||
            diaFolgaSemanal > 7) {

            printf("Erro ao ler a folga semanal!\n");
            fclose(arquivoFolga);
            return;
        }

        if (fclose(arquivoFolga) != 0) {
            printf("Erro ao fechar o arquivo de folga semanal!\n");
            return;
        }

    } else if (errno != ENOENT) {

        perror("Erro ao abrir a folga semanal");
        return;
    }

    // ========================================================
    // CONSULTAR HORARIOS CADASTRADOS
    // ========================================================

    FILE *arquivo = fopen(ARQUIVO_HORARIOS, "rb");

    if (arquivo != NULL) {

        struct HorarioBarbeiro horario;

        while (fread(&horario,
                     sizeof(struct HorarioBarbeiro),
                     1,
                     arquivo) == 1) {

            if (horario.dia >= 1 && horario.dia <= 7) {

                horarios[horario.dia] = horario;
                cadastrados[horario.dia] = 1;
            }
        }

        if (ferror(arquivo)) {

            printf("Erro ao ler os horarios!\n");
            fclose(arquivo);
            return;
        }

        fclose(arquivo);

    } else if (errno != ENOENT) {

        perror("Erro ao abrir os horarios");
        return;
    }

    // ========================================================
    // EXIBIR HORARIOS E FOLGA SEMANAL
    // ========================================================

    printf("\n===== HORARIOS DE TRABALHO =====\n");

    for (int dia = 1; dia <= 7; dia++) {

        // Folga semanal tem prioridade sobre o expediente
        if (dia == diaFolgaSemanal) {

            printf("\nDia: %s\n", diasSemana[dia - 1]);
            printf("FOLGA SEMANAL\n");
            printf("---------------------------------------\n");

            quantidade++;
            continue;
        }

        // Exibe os dias com expediente cadastrado
        if (cadastrados[dia] == 1) {

            printf("\nDia: %s\n", diasSemana[dia - 1]);

            printf("Inicio: %02d:00\n",
                   horarios[dia].horaInicio);

            printf("Termino: %02d:00\n",
                   horarios[dia].horaFim);

            printf("---------------------------------------\n");

            quantidade++;
        }
    }

    if (quantidade == 0) {

        printf("Nenhum horario cadastrado!\n");
    }
}
