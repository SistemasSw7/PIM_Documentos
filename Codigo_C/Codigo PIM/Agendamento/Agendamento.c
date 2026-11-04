
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <errno.h>

// Estruturas e declaracoes dos agendamentos
#include "Agendamento.h"

// Dados dos clientes
#include "../Cliente/Cliente.h"

// Horarios de trabalho do barbeiro
#include "../Barbeiro/HorarioBarbeiro.h"

// Intervalos do barbeiro
#include "../Barbeiro/IntervaloBarbeiro.h"

// Folgas cadastradas por data
#include "../Barbeiro/FolgaBarbeiro.h"

// ============================================================
// VERIFICAR FOLGA SEMANAL FIXA
// ============================================================

// Retornos:
// 1 = barbeiro esta de folga nesse dia
// 0 = nao existe folga semanal nesse dia
// -1 = erro ao consultar o arquivo
//
// Dias da semana:
// 1 - Segunda
// 2 - Terca
// 3 - Quarta
// 4 - Quinta
// 5 - Sexta
// 6 - Sabado
// 7 - Domingo

static int verificarFolgaSemanal(int diaSemana) {

    FILE *arquivoFolga = fopen("folga_semanal.dat", "rb");

    // Se o arquivo nao existe, nao ha folga semanal fixa
    if (arquivoFolga == NULL) {

        if (errno == ENOENT) {
            return 0;
        }

        perror("Erro ao abrir a folga semanal");
        return -1;
    }

    // Armazena o dia salvo pelo barbeiro
    int diaFolga = 0;

    // Le o inteiro salvo no arquivo binario
    int leituraOk = fread(
        &diaFolga,
        sizeof(int),
        1,
        arquivoFolga
    ) == 1;

    int erroLeitura = ferror(arquivoFolga);

    int erroFechamento = fclose(arquivoFolga) != 0;

    // Confere se o arquivo foi lido corretamente
    if (!leituraOk ||
        erroLeitura ||
        erroFechamento ||
        diaFolga < 1 ||
        diaFolga > 7) {

        printf("Erro ao consultar a folga semanal cadastrada!\n");
        return -1;
    }

    // Compara o dia escolhido com a folga semanal
    if (diaSemana == diaFolga) {
        return 1;
    }

    return 0;
}


// ============================================================
// VERIFICAR DIA DE TRABALHO
// ============================================================

// Verifica se o barbeiro possui expediente no dia da semana

int verificarDiaTrabalho(int diaSemana) {

    FILE *arquivoHorario = fopen("horario.dat", "rb");

    // Retorna -1 se nao conseguir abrir o arquivo
    if (arquivoHorario == NULL) {
        return -1;
    }

    struct HorarioBarbeiro horario;

    int encontrou = 0;

    // Percorre os expedientes cadastrados
    while (fread(&horario,
                 sizeof(struct HorarioBarbeiro),
                 1,
                 arquivoHorario) == 1) {

        if (horario.dia == diaSemana) {

            encontrou = 1;
            break;
        }
    }

    fclose(arquivoHorario);

    return encontrou;
}


// ============================================================
// REALIZAR AGENDAMENTO
// ============================================================

// Faz um novo agendamento para o cliente logado
// Recebe o ID do cliente responsavel

void agendarHorario(int idCliente) {

    // Data escolhida pelo cliente
    int dia;
    int mes;
    int ano;

    // Ano bissexto
    int bissexto;

    // Horario do atendimento
    int hora;

    // Validacoes
    int horaValida;
    int dataValida = 0;

    // Horarios ocupados
    int horarioOcupado = 0;

    // Maior ID de agendamento
    int maiorId = 0;

    // 0 = livre | 1 = ocupado
    int horariosOcupados[24] = {0};

    // 0 = fora do intervalo | 1 = intervalo
    int horariosIntervalo[24] = {0};

    // Verifica disponibilidade de horarios
    int temHOrarioLivre = 0;

    // Tipo de atendimento
    int tipoAtendimento = 0;

    // Cliente encontrado
    int encontrouCliente = 0;

    // Folga cadastrada por data
    int encontrouFolga = 0;

    // Nome de quem sera atendido
    char nomeAtendimento[50];

    // Ponteiro para arquivos
    FILE *arquivo;

    // Estruturas
    struct Agendamento novoAgendamento;
    struct Agendamento agendamentoSalvo;
    struct Cliente clienteEncontrado;


    // ========================================================
    // CONSULTAR DATA E HORA ATUAIS
    // ========================================================

    time_t agora = time(NULL);

    struct tm *dataAtual = localtime(&agora);

    if (dataAtual == NULL) {

        printf("Erro ao obter a data atual!\n");
        return;
    }

    int anoAtual = dataAtual->tm_year + 1900;
    int diaAtual = dataAtual->tm_mday;
    int mesAtual = dataAtual->tm_mon + 1;
    int horaAtual = dataAtual->tm_hour;

    printf("Data atual: %02d/%02d/%d\n",
           diaAtual, mesAtual, anoAtual);

    printf("\n===== AGENDAR HORARIO =====\n");
    printf("Cliente ID: %d\n", idCliente);


    // ========================================================
    // ESCOLHER QUEM RECEBERA O ATENDIMENTO
    // ========================================================

    do {

        printf("\nPara quem sera o atendimento?\n");
        printf("1 - Para mim\n");
        printf("2 - Para outra pessoa\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &tipoAtendimento) != 1) {

            printf("Digite apenas numeros!\n");

            int c;

            while ((c = getchar()) != '\n' && c != EOF) {
            }

            tipoAtendimento = 0;
        }

        if (tipoAtendimento != 1 && tipoAtendimento != 2) {

            printf("Opcao invalida!\n");
        }

    } while (tipoAtendimento != 1 && tipoAtendimento != 2);


    // ========================================================
    // ATENDIMENTO PARA OUTRA PESSOA
    // ========================================================

    if (tipoAtendimento == 2) {

        printf("Digite o nome e o sobre nome da pessoa: ");

        scanf(" %49[^\n]", nomeAtendimento);
    }


    // ========================================================
    // ATENDIMENTO PARA O PROPRIO CLIENTE
    // ========================================================

    if (tipoAtendimento == 1) {

        arquivo = fopen("../Dados/cliente.dat", "rb");

        if (arquivo == NULL) {

            printf("Erro ao abrir o arquivo de Cliente!\n");
            return;
        }

        while (fread(&clienteEncontrado,
                     sizeof(struct Cliente),
                     1,
                     arquivo) == 1) {

            if (clienteEncontrado.id == idCliente) {

                strcpy(nomeAtendimento, clienteEncontrado.nome);

                printf("Cliente encontrado: %s\n",
                       clienteEncontrado.nome);

                encontrouCliente = 1;
                break;
            }
        }

        fclose(arquivo);

        if (encontrouCliente == 0) {

            printf("Cliente nao encontrado!\n");
            return;
        }
    }


    // ========================================================
    // ESCOLHER E VALIDAR A DATA DO AGENDAMENTO
    // ========================================================

    do {

        dataValida = 1;


        // ====================================================
        // VALIDAR O DIA
        // ====================================================

        do {

            printf("Digite o dia do agendamento: ");

            if (scanf("%d", &dia) != 1) {

                printf("Digite apenas numeros!\n");

                int c;

                while ((c = getchar()) != '\n' && c != EOF) {
                }

                dia = 0;
            }

            if (dia < 1 || dia > 31) {

                printf("Dia invalido!\n");
            }

        } while (dia < 1 || dia > 31);


        // ====================================================
        // VALIDAR O MES
        // ====================================================

        do {

            printf("Digite o mes do agendamento: ");

            if (scanf("%d", &mes) != 1) {

                printf("Digite apenas numeros!\n");

                int c;

                while ((c = getchar()) != '\n' && c != EOF) {
                }

                mes = 0;
            }

            if (mes < 1 || mes > 12) {

                printf("Mes invalido!\n");
            }

        } while (mes < 1 || mes > 12);


        // ====================================================
        // VALIDAR O ANO
        // ====================================================

        do {

            printf("Digite o ano do agendamento: ");

            if (scanf("%d", &ano) != 1) {

                printf("Digite apenas numeros!\n");

                int c;

                while ((c = getchar()) != '\n' && c != EOF) {
                }

                ano = 0;
            }

            if (ano < anoAtual) {

                printf("Ano invalido!\n");
            }

        } while (ano < anoAtual);


        // ====================================================
        // VERIFICAR ANO BISSEXTO
        // ====================================================

        bissexto = (ano % 4 == 0 && ano % 100 != 0)
                   || ano % 400 == 0;

        // Meses com apenas 30 dias
        if (dia == 31 &&
            (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {

            printf("Data invalida!\n");
            dataValida = 0;
        }

        // Fevereiro
        if (mes == 2) {

            if (dia > 29) {

                printf("Data invalida!\n");
                dataValida = 0;
            }

            if (dia == 29 && bissexto == 0) {

                printf("Data invalida!\n");
                dataValida = 0;
            }
        }


        // ====================================================
        // IMPEDIR AGENDAMENTOS EM DATAS PASSADAS
        // ====================================================

        if (ano == anoAtual && mes < mesAtual) {

            printf("Essa data ja passou!\n");
            dataValida = 0;
        }

        if (ano == anoAtual &&
            mes == mesAtual &&
            dia < diaAtual) {

            printf("Esse dia ja passou!\n");
            dataValida = 0;
        }


        // ====================================================
        // VERIFICAR FOLGAS CADASTRADAS POR DATA
        // ====================================================

        if (dataValida == 1) {

            encontrouFolga = 0;

            FILE *arquivoFolga = fopen("folgas.dat", "rb");

            if (arquivoFolga != NULL) {

                struct FolgaBarbeiro folgalida;

                while (fread(&folgalida,
                             sizeof(struct FolgaBarbeiro),
                             1,
                             arquivoFolga) == 1) {

                    if (folgalida.dia == dia &&
                        folgalida.mes == mes &&
                        folgalida.ano == ano) {

                        encontrouFolga = 1;
                        break;
                    }
                }

                fclose(arquivoFolga);
            }

            if (encontrouFolga == 1) {

                printf("\nO barbeiro esta de folga nessa data!\n");
                printf("Por favor, escolha outra data.\n");

                dataValida = 0;
            }


            // ====================================================
            // VERIFICAR FOLGA SEMANAL E DIA DE TRABALHO
            // ====================================================

            if (dataValida == 1) {

                // Monta a data escolhida pelo cliente
                struct tm dataVerificacao = {0};

                dataVerificacao.tm_mday = dia;
                dataVerificacao.tm_mon = mes - 1;
                dataVerificacao.tm_year = ano - 1900;
                dataVerificacao.tm_isdst = -1;

                // Calcula o dia da semana
                if (mktime(&dataVerificacao) == (time_t)-1) {

                    printf("Erro ao verificar a data escolhida!\n");
                    dataValida = 0;

                } else {

                    int diaSemanaVerificacao =
                        dataVerificacao.tm_wday;

                    // Converte domingo de 0 para 7
                    if (diaSemanaVerificacao == 0) {

                        diaSemanaVerificacao = 7;
                    }


                    // ============================================
                    // CONSULTAR FOLGA SEMANAL FIXA
                    // ============================================

                    int resultadoFolgaSemanal =
                        verificarFolgaSemanal(diaSemanaVerificacao);

                    if (resultadoFolgaSemanal == 1) {

                        printf("\nO barbeiro esta de FOLGA SEMANAL nesse dia!\n");
                        printf("Por favor, escolha outra data.\n");

                        dataValida = 0;

                    } else if (resultadoFolgaSemanal == -1) {

                        // Erro na leitura da folga semanal
                        dataValida = 0;
                    }


                    // ============================================
                    // VERIFICAR EXPEDIENTE DO BARBEIRO
                    // ============================================

                    if (dataValida == 1) {

                        int resultado =
                            verificarDiaTrabalho(diaSemanaVerificacao);

                        if (resultado == -1) {

                            printf("Erro ao consultar os horarios do barbeiro!\n");
                            dataValida = 0;

                        } else if (resultado == 0) {

                            printf("O barbeiro nao trabalha nesse dia!\n");
                            printf("Por favor, escolha outra data.\n");

                            dataValida = 0;
                        }
                    }
                }
            }
        }

    } while (dataValida == 0);


    // ========================================================
    // DESCOBRIR O DIA DA SEMANA
    // ========================================================

    struct tm dataEscolhida = {0};

    dataEscolhida.tm_mday = dia;
    dataEscolhida.tm_mon = mes - 1;
    dataEscolhida.tm_year = ano - 1900;
    dataEscolhida.tm_isdst = -1;

    mktime(&dataEscolhida);

    int diaSemana = dataEscolhida.tm_wday;

    if (diaSemana == 0) {
        diaSemana = 7;
    }

    printf("Dia de semana: %d\n", diaSemana);


    // ========================================================
    // CONSULTAR HORARIOS JA RESERVADOS
    // ========================================================

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo != NULL) {

        while (fread(&agendamentoSalvo,
                     sizeof(struct Agendamento),
                     1,
                     arquivo) == 1) {

            if (agendamentoSalvo.dia == dia &&
                agendamentoSalvo.mes == mes &&
                agendamentoSalvo.ano == ano &&
                agendamentoSalvo.hora >= 0 &&
                agendamentoSalvo.hora <= 23) {

                horariosOcupados[agendamentoSalvo.hora] = 1;
            }
        }

        fclose(arquivo);
    }


    // ========================================================
    // CONSULTAR O EXPEDIENTE DO BARBEIRO
    // ========================================================

    FILE *arquivoHorario = fopen("horario.dat", "rb");

    if (arquivoHorario == NULL) {

        printf("Erro ao abrir horarios!\n");
        return;
    }

    struct HorarioBarbeiro horariolido;

    int encontrouHorario = 0;

    int horaInicio = 0;
    int horaFim = 0;

    while (fread(&horariolido,
                 sizeof(struct HorarioBarbeiro),
                 1,
                 arquivoHorario) == 1) {

        if (horariolido.dia == diaSemana) {

            encontrouHorario = 1;

            horaInicio = horariolido.horaInicio;
            horaFim = horariolido.horaFim;

            printf("Horario desse dia encontrado!\n");

            printf("Dia: %d\n", horariolido.dia);

            printf("Inicio: %02d:00\n",
                   horariolido.horaInicio);

            printf("Termino: %02d:00\n",
                   horariolido.horaFim);
        }
    }

    fclose(arquivoHorario);

    if (encontrouHorario == 0) {

        printf("O barbeiro nao trabalha nesse dia!\n");
        return;
    }


    // ========================================================
    // CONSULTAR INTERVALOS DO BARBEIRO
    // ========================================================

    FILE *arquivoIntervalo = fopen("intervalos.dat", "rb");

    if (arquivoIntervalo == NULL) {

        printf("Nenhum intervalo cadastrado!\n");

    } else {

        struct IntervaloBarbeiro intervalolido;

        while (fread(&intervalolido,
                     sizeof(struct IntervaloBarbeiro),
                     1,
                     arquivoIntervalo) == 1) {

            if (intervalolido.dia == diaSemana) {

                for (int i = intervalolido.horaInicio;
                     i < intervalolido.horaFim;
                     i++) {

                    if (i >= 0 && i < 24) {

                        horariosIntervalo[i] = 1;
                    }
                }

                printf("Intervalo encontrado!\n");
            }
        }

        fclose(arquivoIntervalo);
    }


    // ========================================================
    // MOSTRAR HORARIOS LIVRES E INDISPONIVEIS
    // ========================================================

    printf("\n===== HORARIOS DA BARBEARIA =====\n");

    for (int i = horaInicio; i < horaFim; i++) {

        // Horarios que ja passaram no dia atual
        if (ano == anoAtual &&
            mes == mesAtual &&
            dia == diaAtual &&
            i <= horaAtual) {

            printf("%02d:00 - INDISPONIVEL\n", i);

        // Horarios ja reservados
        } else if (horariosOcupados[i] == 1) {

            printf("%02d:00 - OCUPADO\n", i);

        // Intervalos cadastrados
        } else if (horariosIntervalo[i] == 1) {

            printf("%02d:00 - INTERVALO DO BARBEIRO\n", i);

        } else {

            printf("%02d:00 - LIVRE\n", i);

            temHOrarioLivre = 1;
        }
    }

    if (temHOrarioLivre == 0) {

        printf("Nao existe horarios disponiveis nessa data!\n");
        return;
    }


    // ========================================================
    // ESCOLHER O HORARIO DO ATENDIMENTO
    // ========================================================

    do {

        horaValida = 1;

        printf("Escolha um horario entre %02d:00 e %02d:00: ",
               horaInicio, horaFim - 1);

        if (scanf("%d", &hora) != 1) {

            printf("Digite apenas numeros!\n");

            int c;

            while ((c = getchar()) != '\n' && c != EOF) {
            }

            hora = 0;
            horaValida = 0;
        }

        // Verifica se a hora pertence ao expediente
        if (hora < horaInicio || hora >= horaFim) {

            printf("Barbearia fechada nesse horario!\n");
            horaValida = 0;
        }

        if (hora >= horaInicio && hora < horaFim) {

            // Verifica se o horario esta ocupado
            if (horariosOcupados[hora] == 1) {

                printf("Esse horario ja esta ocupado! Escolha outro.\n");
                horaValida = 0;
            }

            // Verifica os intervalos cadastrados
            if (horariosIntervalo[hora] == 1) {

                printf("Esse horario e intervalo do barbeiro!\n");
                horaValida = 0;
            }
        }

        // Verifica se o horario ja passou
        if (ano == anoAtual &&
            mes == mesAtual &&
            dia == diaAtual &&
            hora <= horaAtual) {

            printf("Esse horario ja passou!\n");
            horaValida = 0;
        }

    } while (horaValida == 0);


    // ========================================================
    // CONFERIR HORARIO E CALCULAR O PROXIMO ID
    // ========================================================

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {

        printf("Nenhum agendamento salvo ainda.\n");

    } else {

        printf("Arquivo de agendamentos encontrado!\n");

        while (fread(&agendamentoSalvo,
                     sizeof(struct Agendamento),
                     1,
                     arquivo) == 1) {

            printf("Agendamento encontrado: %02d/%02d/%d as %02d:00\n",
                   agendamentoSalvo.dia,
                   agendamentoSalvo.mes,
                   agendamentoSalvo.ano,
                   agendamentoSalvo.hora);

            // Verifica novamente se a hora esta ocupada
            if (agendamentoSalvo.dia == dia &&
                agendamentoSalvo.mes == mes &&
                agendamentoSalvo.ano == ano &&
                agendamentoSalvo.hora == hora) {

                printf("Esse horario ja esta ocupado!\n");
                horarioOcupado = 1;
            }

            // Identifica o maior ID
            if (agendamentoSalvo.id > maiorId) {

                maiorId = agendamentoSalvo.id;
            }
        }

        fclose(arquivo);
    }

    if (horarioOcupado == 1) {

        printf("Nao foi possivel realizar o agendamento!\n");
        return;
    }


    // ========================================================
    // PREENCHER OS DADOS DA NOVA RESERVA
    // ========================================================

    novoAgendamento.id = maiorId + 1;

    novoAgendamento.idCliente = idCliente;

    strcpy(novoAgendamento.nomeAtendimento,
           nomeAtendimento);

    novoAgendamento.dia = dia;
    novoAgendamento.mes = mes;
    novoAgendamento.ano = ano;
    novoAgendamento.hora = hora;


    // ========================================================
    // SALVAR AGENDAMENTO NO ARQUIVO BINARIO
    // ========================================================

    arquivo = fopen("../Dados/agendamentos.dat", "ab");

    if (arquivo == NULL) {

        printf("Erro ao abrir o arquivo de agendamentos!\n");
        return;
    }

    if (fwrite(&novoAgendamento,
               sizeof(struct Agendamento),
               1,
               arquivo) != 1) {

        printf("Erro ao salvar agendamento!\n");

        fclose(arquivo);
        return;
    }

    fclose(arquivo);


    // ========================================================
    // EXIBIR RESUMO DO AGENDAMENTO
    // ========================================================

    printf("\n===== RESUMO DO AGENDAMENTO =====\n");

    printf("Agendamento ID: %d\n",
           novoAgendamento.id);

    printf("Cliente ID: %d\n",
           novoAgendamento.idCliente);

    printf("Nome de quem sera atendido: %s\n",
           novoAgendamento.nomeAtendimento);

    printf("Data: %02d/%02d/%d\n",
           novoAgendamento.dia,
           novoAgendamento.mes,
           novoAgendamento.ano);

    printf("Horario: %02d:00\n",
           novoAgendamento.hora);

    printf("Agendamento realizado com sucesso!\n");
}


// ============================================================
// CONSULTAR AGENDAMENTOS DO CLIENTE
// ============================================================

void meusAgendamentos(int idCliente) {

    FILE *arquivo;

    struct Agendamento agendamentoSalvo;

    int encontrou = 0;

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {

        printf("Nenhum agendamento encontrado!\n");
        return;
    }

    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento),
                 1,
                 arquivo) == 1) {

        if (agendamentoSalvo.idCliente == idCliente) {

            encontrou = 1;

            printf("\n ===== AGENDAMENTO =====\n");

            printf("Agendamento ID: %d\n",
                   agendamentoSalvo.id);

            printf("Cliente responsavel ID: %d\n",
                   agendamentoSalvo.idCliente);

            printf("Nome: %s\n",
                   agendamentoSalvo.nomeAtendimento);

            printf("Data: %02d/%02d/%d\n",
                   agendamentoSalvo.dia,
                   agendamentoSalvo.mes,
                   agendamentoSalvo.ano);

            printf("Horario: %02d:00\n",
                   agendamentoSalvo.hora);
        }
    }

    fclose(arquivo);

    if (encontrou == 0) {

        printf("Voce ainda nao possui agendamento!\n");
    }
}


// ============================================================
// CANCELAR AGENDAMENTO
// ============================================================

// Permite cancelar apenas reservas do cliente logado
// Exige pelo menos 2 horas de antecedencia

void cancelarAgendamento(int idCliente) {

    int idAgendamento;

    FILE *arquivo;
    FILE *arquivoTemporario;

    struct Agendamento agendamentoSalvo;

    int encontrou = 0;
    int autorizado = 0;
    int confirmar = 0;


    // ========================================================
    // SOLICITAR ID DO AGENDAMENTO
    // ========================================================

    printf("\n===== CANCELAR AGENDAMENTO =====\n");

    printf("Digite o ID do agendamento que deseja cancelar: ");

    if (scanf("%d", &idAgendamento) != 1) {

        printf("Digite apenas numeros!\n");

        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (idAgendamento <= 0) {

        printf("ID do agendamento invalido!\n");
        return;
    }

    printf("ID escolhido: %d\n", idAgendamento);


    // ========================================================
    // LOCALIZAR AGENDAMENTO
    // ========================================================

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {

        printf("Nenhum agendamento encontrado!\n");
        return;
    }

    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento),
                 1,
                 arquivo) == 1) {

        if (agendamentoSalvo.id == idAgendamento) {

            encontrou = 1;

            if (agendamentoSalvo.idCliente == idCliente) {

                autorizado = 1;

                printf("Agendamento encontrado!\n");

                printf("Nome: %s\n",
                       agendamentoSalvo.nomeAtendimento);

                printf("Data: %02d/%02d/%d\n",
                       agendamentoSalvo.dia,
                       agendamentoSalvo.mes,
                       agendamentoSalvo.ano);

                printf("Horario: %02d:00\n",
                       agendamentoSalvo.hora);

            } else {

                printf("Esse agendamento nao pertence a sua conta!\n");
            }

            break;
        }
    }

    fclose(arquivo);

    if (encontrou == 0) {

        printf("Agendamento nao encontrado!\n");
        return;
    }

    if (autorizado == 0) {

        printf("Voce nao tem permissao para cancelar esse agendamento!\n");
        return;
    }


    // ========================================================
    // VERIFICAR PRAZO DE CANCELAMENTO
    // ========================================================

    struct tm dataAgendamento = {0};

    dataAgendamento.tm_mday = agendamentoSalvo.dia;
    dataAgendamento.tm_mon = agendamentoSalvo.mes - 1;
    dataAgendamento.tm_year = agendamentoSalvo.ano - 1900;
    dataAgendamento.tm_hour = agendamentoSalvo.hora;
    dataAgendamento.tm_isdst = -1;

    time_t horarioAgendado = mktime(&dataAgendamento);

    time_t horarioAtual = time(NULL);

    if (horarioAgendado == (time_t)-1 ||
        horarioAtual == (time_t)-1) {

        printf("Erro ao verificar a data do agendamento!\n");
        return;
    }

    double segundosRestantes =
        difftime(horarioAgendado, horarioAtual);

    // 7200 segundos = 2 horas
    if (segundosRestantes < 7200) {

        printf("Cancelamento nao permitido!\n");

        printf("O agendamento deve ser cancelado com pelo menos 2 horas de antecedencia.\n");

        return;
    }


    // ========================================================
    // CONFIRMAR CANCELAMENTO
    // ========================================================

    printf("\nDeseja realmente cancelar esse agendamento?\n");

    printf("1 - Sim, cancelar\n");
    printf("2 - Nao, voltar\n");
    printf("Escolha uma opcao: ");

    if (scanf("%d", &confirmar) != 1) {

        printf("Digite apenas numeros!\n");

        int c;

        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    if (confirmar != 1) {

        printf("Cancelamento nao realizado!\n");
        return;
    }

    printf("Processando cancelamento...\n");


    // ========================================================
    // ABRIR ARQUIVO ORIGINAL
    // ========================================================

    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {

        printf("Erro ao abrir os agendamentos!\n");
        return;
    }


    // ========================================================
    // CRIAR ARQUIVO TEMPORARIO
    // ========================================================

    arquivoTemporario = fopen(
        "../Dados/agendamentos_temp.dat", "wb");

    if (arquivoTemporario == NULL) {

        printf("Erro ao criar o arquivo temporario!\n");

        fclose(arquivo);
        return;
    }


    // ========================================================
    // COPIAR AGENDAMENTOS QUE DEVEM SER MANTIDOS
    // ========================================================

    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento),
                 1,
                 arquivo) == 1) {

        // Nao copia o agendamento cancelado
        if (agendamentoSalvo.id != idAgendamento) {

            if (fwrite(&agendamentoSalvo,
                       sizeof(struct Agendamento),
                       1,
                       arquivoTemporario) != 1) {

                printf("Erro ao copiar agendamento!\n");

                fclose(arquivo);
                fclose(arquivoTemporario);

                remove("../Dados/agendamentos_temp.dat");

                return;
            }
        }
    }


    // ========================================================
    // VERIFICAR ERROS DE LEITURA
    // ========================================================

    if (ferror(arquivo)) {

        printf("Erro ao ler os agendamentos!\n");

        fclose(arquivo);
        fclose(arquivoTemporario);

        remove("../Dados/agendamentos_temp.dat");

        return;
    }


    // ========================================================
    // FECHAR ARQUIVOS
    // ========================================================

    int erroOriginal = fclose(arquivo);

    int erroTemporario = fclose(arquivoTemporario);

    if (erroOriginal != 0 || erroTemporario != 0) {

        printf("Erro ao fechar os arquivos!\n");

        remove("../Dados/agendamentos_temp.dat");

        return;
    }


    // ========================================================
    // VERIFICAR BACKUP EXISTENTE
    // ========================================================

    FILE *backup = fopen(
        "../Dados/agendamentos_cancelamento_backup.dat", "rb");

    if (backup != NULL) {

        fclose(backup);

        printf("Ja existe um backup. Resolva isso antes de cancelar!\n");

        remove("../Dados/agendamentos_temp.dat");

        return;
    }


    // ========================================================
    // CRIAR BACKUP DO ARQUIVO ORIGINAL
    // ========================================================

    if (rename("../Dados/agendamentos.dat",
               "../Dados/agendamentos_cancelamento_backup.dat") != 0) {

        printf("Erro ao criar backup!\n");
        return;
    }


    // ========================================================
    // SUBSTITUIR ARQUIVO ORIGINAL
    // ========================================================

    if (rename("../Dados/agendamentos_temp.dat",
               "../Dados/agendamentos.dat") != 0) {

        printf("Erro ao finalizar o cancelamento!\n");

        // Restaura o original usando o backup
        if (rename(
                "../Dados/agendamentos_cancelamento_backup.dat",
                "../Dados/agendamentos.dat") != 0) {

            printf("Erro: restaure o arquivo original pelo backup!\n");
        }

        return;
    }


    // ========================================================
    // REMOVER BACKUP
    // ========================================================

    if (remove(
            "../Dados/agendamentos_cancelamento_backup.dat") != 0) {

        printf("Aviso: nao foi possivel remover o backup!\n");
    }

    printf("Agendamento cancelado com sucesso!\n");
}
