
#include <stdio.h>

// Inclui a estrutura HorarioBarbeiro,
// que armazena o dia e os horarios de trabalho
#include "HorarioBarbeiro.h"

#include <time.h>
#include "../Agendamento/Agendamento.h"
#include <errno.h>
#include "IntervaloBarbeiro.h"


// Verifica se existem agendamentos que ficariam fora do novo expediente
int verificarConflitoAgendamentos(int diaSemana, int horaInicio, int horaFim) {

    FILE *arquivo;
    struct Agendamento agendamento;

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
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

   if (arquivo == NULL) {

    // Se o arquivo nao existe, ainda nao ha agendamentos
    if (errno == ENOENT) {
        return 0;
    }

    // Outros erros impedem a verificacao
    printf("Erro ao acessar os agendamentos!\n");
    return -1;
}

    // Le um agendamento por vez
    while (fread(&agendamento, sizeof(struct Agendamento), 1, arquivo) == 1) {

        // Ignora agendamentos de datas passadas
        if (agendamento.ano < anoAtual ||
            (agendamento.ano == anoAtual && agendamento.mes < mesAtual) ||
            (agendamento.ano == anoAtual && agendamento.mes == mesAtual &&
             agendamento.dia < diaAtual)) {
            continue;
        }

        // Descobre o dia da semana do agendamento
        struct tm data = {0};

        data.tm_mday = agendamento.dia;
        data.tm_mon = agendamento.mes - 1;
        data.tm_year = agendamento.ano - 1900;
        data.tm_isdst = -1;

        if (mktime(&data) == (time_t)-1) {
            printf("Erro ao verificar a data do agendamento!\n");
            fclose(arquivo);
            return -1;
        }

        // Converte domingo de 0 para 7
        int diaAgendamento = data.tm_wday;

        if (diaAgendamento == 0) {
            diaAgendamento = 7;
        }

        // Verifica se o agendamento pertence ao dia alterado
        if (diaAgendamento == diaSemana) {

            // Confere se o horario ficaria fora do expediente
            if (agendamento.hora < horaInicio ||
                agendamento.hora >= horaFim) {

                printf("\nExiste um agendamento que impede essa alteracao!\n");
                printf("Data: %02d/%02d/%04d\n",
                       agendamento.dia, agendamento.mes, agendamento.ano);
                printf("Horario: %02d:00\n", agendamento.hora);

                fclose(arquivo);
                return 1;
            }
        }
    }

    // Verifica se houve erro durante a leitura
    if (ferror(arquivo)) {
        printf("Erro ao ler os agendamentos!\n");
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);

    // Nenhum conflito encontrado
    return 0;
}


 // Verifica se algum intervalo ficaria fora do novo expediente
int verificarConflitoIntervalos(int diaSemana, int horaInicio, int horaFim) {

    FILE *arquivo;
    struct IntervaloBarbeiro intervalo;

    // Abre os intervalos cadastrados
    arquivo = fopen("intervalos.dat", "rb");

    if (arquivo == NULL) {

        // Se o arquivo nao existe, nao ha intervalos cadastrados
        if (errno == ENOENT) {
            return 0;
        }

        printf("Erro ao acessar os intervalos!\n");
        return -1;
    }

    // Percorre os intervalos salvos
    while (fread(&intervalo, sizeof(struct IntervaloBarbeiro),
                 1, arquivo) == 1) {

        // Verifica se o intervalo pertence ao dia escolhido
        if (intervalo.dia == diaSemana) {

            // Confere se ficaria fora do novo expediente
            if (intervalo.horaInicio < horaInicio ||
                intervalo.horaFim > horaFim) {

                printf("\nExiste um intervalo que impede essa alteracao!\n");
                printf("Intervalo: %02d:00 as %02d:00\n",
                       intervalo.horaInicio, intervalo.horaFim);

                fclose(arquivo);
                return 1;
            }
        }
    }

    // Verifica erros durante a leitura
    if (ferror(arquivo)) {
        printf("Erro ao ler os intervalos!\n");
        fclose(arquivo);
        return -1;
    }

    fclose(arquivo);

    // Nenhum conflito encontrado
    return 0;
}



// Funcao responsavel por configurar os horarios do barbeiro
void configurarHorarios(void) {

    // Cria uma variavel da estrutura HorarioBarbeiro
    // para armazenar o dia, a hora de inicio e a hora de termino
    struct HorarioBarbeiro horario;

    // Exibe o menu com os dias da semana
    printf("\n===== CONFIGURAR HORARIOS =====\n");

    printf("1 - Segunda-feira\n");
    printf("2 - Terca-feira\n");
    printf("3 - Quarta-feira\n");
    printf("4 - Quinta-feira\n");
    printf("5 - Sexta-feira\n");
    printf("6 - Sabado\n");
    printf("7 - Domingo\n");

    // Solicita o dia de trabalho do barbeiro
    printf("Escolha o dia da semana: ");

    // Verifica se o usuario digitou um numero inteiro
    if (scanf("%d", &horario.dia) != 1) {
        printf("Entrada invalida!\n");
        return;
    }

    // Verifica se o dia escolhido esta entre 1 e 7
    if (horario.dia < 1 || horario.dia > 7) {
        printf("Dia invalido!\n");
        return;
    }

    // Solicita a hora de inicio do expediente
    printf("Digite a hora de inicio (0 a 23): ");

    // Verifica se a hora digitada e um numero inteiro
    if (scanf("%d", &horario.horaInicio) != 1) {
        printf("Entrada invalida!\n");
        return;
    }

    // Impede horarios menores que 0 ou maiores que 23
    if (horario.horaInicio < 0 || horario.horaInicio > 23) {
        printf("Hora invalida!\n");
        return;
    }

    // Solicita a hora de termino do expediente
    printf("Digite a hora de termino (0 a 23): ");

    // Verifica se a hora de termino e um numero inteiro
    if (scanf("%d", &horario.horaFim) != 1) {
        printf("Entrada invalida!\n");
        return;
    }

    // Verifica se a hora de termino esta entre 0 e 23
    if (horario.horaFim < 0 || horario.horaFim > 23) {
        printf("Hora invalida!\n");
        return;
    }

    // Impede que o expediente termine antes
    // ou no mesmo horario em que comecou
    if (horario.horaFim <= horario.horaInicio) {
        printf("A hora do termino deve ser maior que a do inicio!\n");
        return;
    }


    // Verifica se o novo expediente prejudica algum agendamento
    int conflito = verificarConflitoAgendamentos(horario.dia, horario.horaInicio, horario.horaFim);

    // Existe um agendamento fora do novo expediente
    if (conflito == 1) {
        printf("Alteracao cancelada por conflito de agendamento!\n");
        return;
    }

    // Nao foi possivel verificar os agendamentos
    if (conflito == -1) {
        printf("Nao foi possivel verificar os agendamentos!\n");
        return;
    }

    // Verifica se o novo expediente prejudica algum intervalo
int conflitoIntervalo = verificarConflitoIntervalos( horario.dia, horario.horaInicio, horario.horaFim);
   

    // Existe um intervalo fora do novo expediente
    if (conflitoIntervalo == 1) {
        printf("Alteracao cancelada por conflito de intervalo!\n");
        return;
    }

    // Nao foi possivel verificar os intervalos
    if (conflitoIntervalo == -1) {
        printf("Nao foi possivel verificar os intervalos!\n");
        return;
    }



    // Abre o arquivo para leitura e atualizacao
    FILE *arquivoConsulta = fopen("horario.dat", "rb+");

    // Estrutura utilizada para ler os horarios existentes
    struct HorarioBarbeiro horarioSalvo;

    // Verifica se o arquivo existe
    if (arquivoConsulta != NULL) {

        // Percorre os horarios cadastrados
        while (fread(&horarioSalvo, sizeof(struct HorarioBarbeiro),
                    1, arquivoConsulta) == 1) {

            // Verifica se o dia escolhido ja esta cadastrado
            if (horarioSalvo.dia == horario.dia) {

                // Volta ao inicio do registro encontrado
                if (fseek(arquivoConsulta,
                        -(long)sizeof(struct HorarioBarbeiro),
                        SEEK_CUR) != 0) {

                    printf("Erro ao localizar horario!\n");
                    fclose(arquivoConsulta);
                    return;
                }

                // Substitui o horario antigo pelo novo
                if (fwrite(&horario, sizeof(struct HorarioBarbeiro),
                        1, arquivoConsulta) != 1) {

                    printf("Erro ao atualizar horario!\n");
                    fclose(arquivoConsulta);
                    return;
                }

                fclose(arquivoConsulta);

                printf("Horario atualizado com sucesso!\n");
                return;
            }
        }

        fclose(arquivoConsulta);
    }


    // Abre o arquivo binario para adicionar os horarios
    // O modo "ab" cria o arquivo se ele nao existir
    // ou adiciona novos registros ao final
    FILE *arquivo = fopen("horario.dat", "ab");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    // Grava a estrutura completa no arquivo binario
    // O retorno 1 indica que um registro foi gravado
    if (fwrite(&horario, sizeof(struct HorarioBarbeiro), 1, arquivo) != 1) {
        printf("Erro ao salvar horario!\n");

        // Fecha o arquivo antes de encerrar a funcao
        fclose(arquivo);
        return;
    }

    // Fecha o arquivo apos realizar a gravacao
    fclose(arquivo);

    // Exibe uma mensagem confirmando a configuracao
    printf("Horario confirmado com sucesso!\n");

    // Mostra os horarios configurados pelo barbeiro
    // %02d exibe os numeros com dois digitos
    printf("Inicio : %02d:00\n", horario.horaInicio);
    printf("Termino: %02d:00\n", horario.horaFim);
}

//funcao responsavel por visualizar os horarios cadastrados
void visualizarHorariosBarbeiro(void){

    FILE *arquivo;
    struct HorarioBarbeiro horario;
    int quantidade = 0;

    // Nomes dos dias da semana
    const char *diasSemana[] = {
        "Segunda-feira", "Terca-feira", "Quarta-feira", "Quinta-feira", "Sexta-feira", "Sabado", "Domingo"
    };

    //Abre o arquivo no modo leitura binaria
    arquivo = fopen("horario.dat", "rb");

    if (arquivo == NULL){
        printf("Nenhum horario cadastrado!\n");
        return;
    }
    printf("\n===== HORARIOS DE TRABALHO =====\n");

    //Le os horarios salvos no arquivo
    while(fread(&horario, sizeof(struct HorarioBarbeiro), 1, arquivo) == 1){

        //Verifica se o dia armazenado e valido
        if(horario.dia >= 1 && horario.dia <= 7){
            quantidade++;

            printf("\nDia: %s\n", diasSemana[horario.dia - 1]);
            printf("Inicio: %02d:00\n", horario.horaInicio);
            printf("Termino: %02d:00\n", horario.horaFim);
            printf("---------------------------------------\n");
        }
    }
    if(quantidade == 0){
        printf("Nenhum horario cadastrado!\n");
    }
    fclose(arquivo);

    
}
