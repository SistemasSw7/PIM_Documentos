
#include <stdio.h>
#include <time.h>
#include <string.h>

// Inclui a estrutura e as declaracoes utilizadas nos agendamentos
#include "Agendamento.h"

// Inclui a estrutura que armazena os dados dos clientes
#include "../Cliente/Cliente.h"

// Inclui a estrutura dos horarios de trabalho do barbeiro
#include "../Barbeiro/HorarioBarbeiro.h"

// Inclui a estrutura dos intervalos cadastrados pelo barbeiro
#include "../Barbeiro/IntervaloBarbeiro.h"


// ============================================================
// REALIZAR AGENDAMENTO
// ============================================================

// Faz um novo agendamento para o cliente logado.
// Recebe o ID do cliente para identificar o responsavel.
void agendarHorario(int idCliente) {

    // Variaveis que armazenam a data escolhida pelo cliente
    int dia;
    int mes;
    int ano;

    // Indica se o ano escolhido e bissexto
    int bissexto;

    // Armazena o horario escolhido para o atendimento
    int hora;

    // Controla se a hora escolhida pode ser utilizada
    int horaValida;

    // Controla a validade da data informada
    int dataValida = 0;

    // Indica se o horario ja foi reservado
    int horarioOcupado = 0;

    // Guarda o maior ID de agendamento encontrado
    int maiorId = 0;

    // Cada posicao representa uma hora do dia
    // 0 = horario nao ocupado | 1 = horario ocupado
    int horariosOcupados[24] = {0};

    // Identifica os horarios bloqueados por intervalos
    // 0 = fora do intervalo | 1 = dentro do intervalo
    int horariosIntervalo[24] = {0};

    // Indica se existe pelo menos um horario livre
    int temHOrarioLivre = 0;

    // Define para quem sera realizado o atendimento
    int tipoAtendimento = 0;

    // Indica se o cliente foi encontrado no cadastro
    int encontrouCliente = 0;

    // Armazena o nome da pessoa que sera atendida
    char nomeAtendimento[50];

    // Ponteiro utilizado para abrir os arquivos binarios
    FILE *arquivo;

    // Estrutura utilizada para armazenar a nova reserva
    struct Agendamento novoAgendamento;

    // Estrutura utilizada para ler as reservas existentes
    struct Agendamento agendamentoSalvo;

    // Estrutura utilizada para consultar os dados do cliente
    struct Cliente clienteEncontrado;


    // ========================================================
    // CONSULTAR A DATA E A HORA ATUAIS
    // ========================================================

    // Obtem o instante atual do sistema
    time_t agora = time(NULL);

    // Converte o instante atual para a data e hora locais
    struct tm *dataAtual = localtime(&agora);

    // Verifica se a consulta da data atual falhou
    if (dataAtual == NULL) {
        printf("Erro ao obter a data atual!\n");
        return;
    }

    // Extrai o ano, dia, mes e hora atuais
    int anoAtual = dataAtual->tm_year + 1900;
    int diaAtual = dataAtual->tm_mday;
    int mesAtual = dataAtual->tm_mon + 1;
    int horaAtual = dataAtual->tm_hour;

    // Mostra a data atual para o cliente
    printf("Data atual: %02d/%02d/%d\n",
           dataAtual->tm_mday,
           dataAtual->tm_mon + 1,
           dataAtual->tm_year + 1900);

    // Exibe a identificacao da tela de agendamento
    printf("\n===== AGENDAR HORARIO =====\n");
    printf("Cliente ID: %d\n", idCliente);


    // ========================================================
    // ESCOLHER QUEM RECEBERA O ATENDIMENTO
    // ========================================================

    // Repete a pergunta ate receber uma opcao valida
    do {

        printf("\nPara quem sera o atendimento?\n");
        printf("1 - Para mim\n");
        printf("2 - Para outra pessoa\n");
        printf("Escolha uma opcao: ");

        // Verifica se o usuario digitou um numero inteiro
        if (scanf("%d", &tipoAtendimento) != 1) {
            printf("Digite apenas numeros!\n");

            // Descarta os caracteres invalidos da entrada
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            // Reinicia a opcao quando a leitura falha
            tipoAtendimento = 0;
        }

        // Informa quando a opcao digitada nao existe
        if (tipoAtendimento != 1 && tipoAtendimento != 2) {
            printf("Opcao invalida!\n");
        }

    } while (tipoAtendimento != 1 && tipoAtendimento != 2);


    // ========================================================
    // ATENDIMENTO PARA OUTRA PESSOA
    // ========================================================

    // Permite informar o nome de outra pessoa
    if (tipoAtendimento == 2) {

        printf("Digite o nome e o sobre nome da pessoa: ");

        // Le o nome com espacos, respeitando o tamanho do vetor
        scanf(" %49[^\n]", nomeAtendimento);
    }


    // ========================================================
    // ATENDIMENTO PARA O PROPRIO CLIENTE
    // ========================================================

    // Busca o nome do cliente, caso o atendimento seja para ele
    if (tipoAtendimento == 1) {

        // Abre o arquivo com os clientes cadastrados
        arquivo = fopen("../Dados/cliente.dat", "rb");

        // Verifica se o arquivo foi aberto
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo de Cliente!\n");
            return;
        }

        // Le os clientes cadastrados, um registro por vez
        while (fread(&clienteEncontrado,
                     sizeof(struct Cliente), 1, arquivo) == 1) {

            // Compara o ID do registro com o ID do cliente logado
            if (clienteEncontrado.id == idCliente) {

                // Copia o nome cadastrado para o atendimento
                strcpy(nomeAtendimento, clienteEncontrado.nome);

                // Mostra o nome encontrado
                printf("Cliente encontrado: %s\n",
                       clienteEncontrado.nome);

                // Indica que o cliente foi encontrado
                encontrouCliente = 1;

                // Interrompe a busca apos encontrar o cliente
                break;
            }
        }

        // Fecha o arquivo depois da consulta
        fclose(arquivo);

        // Impede continuar se o cliente nao foi encontrado
        if (encontrouCliente == 0) {
            printf("Cliente nao encontrado!\n");
            return;
        }
    }


    // ========================================================
    // ESCOLHER E VALIDAR A DATA DO AGENDAMENTO
    // ========================================================

    do {

        // Inicialmente considera a data valida
        dataValida = 1;


        // ====================================================
        // VALIDAR O DIA
        // ====================================================

        do {

            printf("Digite o dia do agendamento: ");

            // Verifica se o valor digitado e um numero
            if (scanf("%d", &dia) != 1) {
                printf("Digite apenas numeros!\n");

                // Limpa os caracteres invalidos
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                }

                dia = 0;
            }

            // Impede dias menores que 1 ou maiores que 31
            if (dia < 1 || dia > 31) {
                printf("Dia invalido!\n");
            }

        } while (dia < 1 || dia > 31);


        // ====================================================
        // VALIDAR O MES
        // ====================================================

        do {

            printf("Digite o mes do agendamento: ");

            // Verifica se a entrada e numerica
            if (scanf("%d", &mes) != 1) {
                printf("Digite apenas numeros!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                }

                mes = 0;
            }

            // Impede meses fora do intervalo de 1 a 12
            if (mes < 1 || mes > 12) {
                printf("Mes invalido!\n");
            }

        } while (mes < 1 || mes > 12);


        // ====================================================
        // VALIDAR O ANO
        // ====================================================

        do {

            printf("Digite o ano do agendamento: ");

            // Verifica se o ano informado e numerico
            if (scanf("%d", &ano) != 1) {
                printf("Digite apenas numeros!\n");

                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                }

                ano = 0;
            }

            // Impede escolher um ano anterior ao ano atual
            if (ano < anoAtual) {
                printf("Ano invalido!\n");
            }

        } while (ano < anoAtual);


        // ====================================================
        // VERIFICAR ANO BISSEXTO E DIAS DOS MESES
        // ====================================================

        // Identifica anos bissextos pelas regras do calendario
        bissexto = (ano % 4 == 0 && ano % 100 != 0)
                   || ano % 400 == 0;

        // Impede dia 31 nos meses que possuem apenas 30 dias
        if (dia == 31 &&
            (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {

            printf("Data invalida!\n");
            dataValida = 0;
        }

        // Aplica as regras especificas de fevereiro
        if (mes == 2) {

            // Fevereiro nunca possui mais de 29 dias
            if (dia > 29) {
                printf("Data invalida!\n");
                dataValida = 0;
            }

            // Dia 29 so existe em fevereiro de ano bissexto
            if (dia == 29 && bissexto == 0) {
                printf("Data invalida!\n");
                dataValida = 0;
            }
        }


        // ====================================================
        // IMPEDIR AGENDAMENTOS EM DATAS PASSADAS
        // ====================================================

        // Verifica se o mes informado ja passou neste ano
        if (ano == anoAtual && mes < mesAtual) {
            printf("Essa data ja passou!\n");
            dataValida = 0;
        }

        // Verifica se o dia ja passou no mes atual
        if (ano == anoAtual &&
            mes == mesAtual &&
            dia < diaAtual) {

            printf("Esse dia ja passou!\n");
            dataValida = 0;
        }

    } while (dataValida == 0);


    // ========================================================
    // DESCOBRIR O DIA DA SEMANA
    // ========================================================

    // Guarda a data escolhida pelo cliente
    struct tm dataEscolhida = {0};

    // Preenche a estrutura com a data informada
    dataEscolhida.tm_mday = dia;
    dataEscolhida.tm_mon = mes - 1;
    dataEscolhida.tm_year = ano - 1900;
    dataEscolhida.tm_isdst = -1;

    // Calcula a data e preenche o dia da semana
    mktime(&dataEscolhida);

    // tm_wday retorna 0 para domingo, 1 para segunda etc.
    int diaSemana = dataEscolhida.tm_wday;

    // Converte domingo de 0 para 7
    // Segue o padrao utilizado nos horarios do barbeiro
    if (diaSemana == 0) {
        diaSemana = 7;
    }

    // Mostra o numero correspondente ao dia da semana
    printf("Dia de semana: %d\n", diaSemana);


    // ========================================================
    // CONSULTAR HORARIOS JA RESERVADOS
    // ========================================================

    // Le os horarios ja reservados e identifica os ocupados
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    // Se o arquivo existir, percorre as reservas salvas
    if (arquivo != NULL) {

        while (fread(&agendamentoSalvo,
                     sizeof(struct Agendamento), 1, arquivo) == 1) {

            // Considera apenas reservas feitas para a data escolhida
            // Tambem verifica se a hora esta entre 0 e 23
            if (agendamentoSalvo.dia == dia &&
                agendamentoSalvo.mes == mes &&
                agendamentoSalvo.ano == ano &&
                agendamentoSalvo.hora >= 0 &&
                agendamentoSalvo.hora <= 23) {

                // Marca a hora encontrada como ocupada
                horariosOcupados[agendamentoSalvo.hora] = 1;
            }
        }

        // Fecha o arquivo de reservas
        fclose(arquivo);
    }


    // ========================================================
    // CONSULTAR O EXPEDIENTE DO BARBEIRO
    // ========================================================

    // Abre o arquivo que guarda os dias e horarios de trabalho
    FILE *arquivoHorario = fopen("horario.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivoHorario == NULL) {
        printf("Erro ao abrir horarios!\n");
        return;
    }

    // Estrutura utilizada para ler cada expediente cadastrado
    struct HorarioBarbeiro horariolido;

    // Controla se existe expediente no dia escolhido
    int encontrouHorario = 0;

    // Guarda o inicio e o fim do expediente encontrado
    int horaInicio = 0;
    int horaFim = 0;

    // Percorre os registros de expediente do barbeiro
    while (fread(&horariolido,
                 sizeof(struct HorarioBarbeiro),
                 1, arquivoHorario) == 1) {

        // Verifica se o registro pertence ao dia escolhido
        if (horariolido.dia == diaSemana) {

            // Confirma que encontrou expediente para o dia
            encontrouHorario = 1;

            // Guarda os limites do horario de trabalho
            horaInicio = horariolido.horaInicio;
            horaFim = horariolido.horaFim;

            // Mostra o expediente encontrado
            printf("Horario desse dia encontrado!\n");

            printf("Dia: %d\n", horariolido.dia);
            printf("Inicio: %02d:00\n", horariolido.horaInicio);
            printf("Termino: %02d:00\n", horariolido.horaFim);
        }
    }

    // Fecha o arquivo depois da leitura dos expedientes
    fclose(arquivoHorario);

    // Verifica se o barbeiro trabalha nesse dia
    if (encontrouHorario == 0) {
        printf("O barbeiro nao trabalha nesse dia!\n");
        return;
    }


    // ========================================================
    // CONSULTAR OS INTERVALOS DO BARBEIRO
    // ========================================================

    // Abre o arquivo que armazena os intervalos cadastrados
    FILE *arquivoIntervalo = fopen("intervalos.dat", "rb");

    // Se o arquivo nao existir, informa que nao ha intervalos
    if (arquivoIntervalo == NULL) {
        printf("Nenhum intervalo cadastrado!\n");

    } else {

        // Estrutura que recebe cada intervalo lido do arquivo
        struct IntervaloBarbeiro intervalolido;

        // Percorre todos os intervalos salvos
        while (fread(&intervalolido,
                     sizeof(struct IntervaloBarbeiro),
                     1, arquivoIntervalo) == 1) {

            // Verifica se o intervalo pertence ao dia escolhido
            if (intervalolido.dia == diaSemana) {

                // Percorre as horas desde o inicio ate antes do fim
                for (int i = intervalolido.horaInicio;
                     i < intervalolido.horaFim;
                     i++) {

                    // Impede acessar posicoes fora do vetor
                    if (i >= 0 && i < 24) {

                        // Bloqueia a hora correspondente
                        horariosIntervalo[i] = 1;
                    }
                }

                // Confirma que encontrou um intervalo nesse dia
                printf("Intervalo encontrado!\n");
            }
        }

        // Fecha o arquivo apos ler todos os intervalos
        fclose(arquivoIntervalo);
    }


    // ========================================================
    // MOSTRAR HORARIOS LIVRES E INDISPONIVEIS
    // ========================================================

    printf("\n===== HORARIOS DA BARBEARIA =====\n");

    // Percorre todas as horas do expediente
    // O horario final nao entra nas opcoes de atendimento
    for (int i = horaInicio; i < horaFim; i++) {

        // Bloqueia horarios que ja passaram no dia atual
        if (ano == anoAtual &&
            mes == mesAtual &&
            dia == diaAtual &&
            i <= horaAtual) {

            printf("%02d:00 - INDISPONIVEL\n", i);

        // Mostra quando ja existe uma reserva naquela hora
        } else if (horariosOcupados[i] == 1) {

            printf("%02d:00 - OCUPADO\n", i);

        // Mostra os horarios de intervalo do barbeiro
        } else if (horariosIntervalo[i] == 1) {

            printf("%02d:00 - INTERVALO DO BARBEIRO\n", i);

        } else {

            // Mostra os horarios que passaram pelas verificacoes
            printf("%02d:00 - LIVRE\n", i);

            // Indica que existe pelo menos uma hora livre
            temHOrarioLivre = 1;
        }
    }

    // Encerra caso nao exista nenhum horario disponivel
    if (temHOrarioLivre == 0) {
        printf("Nao existe horarios disponiveis nessa data!\n");
        return;
    }


    // ========================================================
    // ESCOLHER O HORARIO DO ATENDIMENTO
    // ========================================================

    // Repete a escolha ate receber uma hora permitida
    do {

        // Inicialmente considera a hora valida
        horaValida = 1;

        // Mostra os limites do expediente
        printf("Escolha um horario entre %02d:00 e %02d:00: ",
               horaInicio, horaFim - 1);

        // Verifica se o usuario digitou um numero
        if (scanf("%d", &hora) != 1) {
            printf("Digite apenas numeros!\n");

            // Descarta caracteres invalidos
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {
            }

            // Reinicia a hora e sinaliza a entrada invalida
            hora = 0;
            horaValida = 0;
        }

        // Impede escolher horario fora do expediente
        if (hora < horaInicio || hora >= horaFim) {
            printf("Barbearia fechada nesse horario!\n");
            horaValida = 0;
        }

        // Realiza as consultas somente para horas do expediente
        if (hora >= horaInicio && hora < horaFim) {

            // Impede escolher horario reservado por outro cliente
            if (horariosOcupados[hora] == 1) {
                printf("Esse horario ja esta ocupado! Escolha outro.\n");
                horaValida = 0;
            }

            // Impede escolher horario de intervalo do barbeiro
            if (horariosIntervalo[hora] == 1) {
                printf("Esse horario e intervalo do barbeiro!\n");
                horaValida = 0;
            }
        }

        // Impede agendar em uma hora que ja passou hoje
        if (ano == anoAtual &&
            mes == mesAtual &&
            dia == diaAtual &&
            hora <= horaAtual) {

            printf("Esse horario ja passou!\n");
            horaValida = 0;
        }

    } while (horaValida == 0);


    // ========================================================
    // CONFERIR NOVAMENTE O HORARIO E CALCULAR O PROXIMO ID
    // ========================================================

    // Abre o arquivo com os agendamentos existentes
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    // Informa se nao encontrou nenhuma reserva anterior
    if (arquivo == NULL) {
        printf("Nenhum agendamento salvo ainda.\n");

    } else {

        printf("Arquivo de agendamentos encontrado!\n");

        // Percorre todos os agendamentos salvos
        while (fread(&agendamentoSalvo,
                     sizeof(struct Agendamento), 1, arquivo) == 1) {

            // Mostra cada agendamento encontrado
            printf("Agendamento encontrado: %02d/%02d/%d as %02d:00\n",
                   agendamentoSalvo.dia,
                   agendamentoSalvo.mes,
                   agendamentoSalvo.ano,
                   agendamentoSalvo.hora);

            // Verifica novamente se a hora escolhida esta ocupada
            if (agendamentoSalvo.dia == dia &&
                agendamentoSalvo.mes == mes &&
                agendamentoSalvo.ano == ano &&
                agendamentoSalvo.hora == hora) {

                printf("Esse horario ja esta ocupado!\n");
                horarioOcupado = 1;
            }

            // Guarda o maior ID encontrado para criar o proximo
            if (agendamentoSalvo.id > maiorId) {
                maiorId = agendamentoSalvo.id;
            }
        }

        // Fecha o arquivo apos a consulta
        fclose(arquivo);
    }

    // Impede continuar se o horario ja estiver ocupado
    if (horarioOcupado == 1) {
        printf("Nao foi possivel realizar o agendamento!\n");
        return;
    }


    // ========================================================
    // PREENCHER OS DADOS DA NOVA RESERVA
    // ========================================================

    // Gera um ID para o novo agendamento
    novoAgendamento.id = maiorId + 1;

    // Associa a reserva ao cliente logado
    novoAgendamento.idCliente = idCliente;

    // Copia o nome de quem recebera o atendimento
    strcpy(novoAgendamento.nomeAtendimento, nomeAtendimento);

    // Guarda a data e a hora escolhidas
    novoAgendamento.dia = dia;
    novoAgendamento.mes = mes;
    novoAgendamento.ano = ano;
    novoAgendamento.hora = hora;


    // ========================================================
    // SALVAR A RESERVA NO ARQUIVO BINARIO
    // ========================================================

    // Abre o arquivo em modo de adicao binaria
    // "ab" acrescenta registros sem apagar os anteriores
    arquivo = fopen("../Dados/agendamentos.dat", "ab");

    // Verifica se o arquivo foi aberto corretamente
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de agendamentos!\n");
        return;
    }

    // Grava os dados completos da nova reserva
    if (fwrite(&novoAgendamento,
               sizeof(struct Agendamento), 1, arquivo) != 1) {

        printf("Erro ao salvar agendamento!");

        // Fecha o arquivo antes de sair
        fclose(arquivo);
        return;
    }

    // Fecha o arquivo apos a gravacao
    fclose(arquivo);


    // ========================================================
    // EXIBIR O RESUMO DO AGENDAMENTO
    // ========================================================

    // Mostra os dados da reserva recem-criada
    printf("\n===== RESUMO DO AGENDAMENTO =====\n");

    printf("Agendamento ID: %d\n", novoAgendamento.id);
    printf("Cliente ID: %d\n", novoAgendamento.idCliente);

    printf("Nome de quem sera atendido: %s\n",
           novoAgendamento.nomeAtendimento);

    printf("Data: %02d/%02d/%d\n",
           novoAgendamento.dia,
           novoAgendamento.mes,
           novoAgendamento.ano);

    printf("Horario: %02d:00\n", novoAgendamento.hora);

    printf("Agendamento realizado com sucesso!\n");
}


// ============================================================
// CONSULTAR OS AGENDAMENTOS DO CLIENTE
// ============================================================

// Mostra apenas os agendamentos do cliente logado
void meusAgendamentos(int idCliente) {

    // Ponteiro utilizado para consultar o arquivo binario
    FILE *arquivo;

    // Estrutura usada para ler cada reserva
    struct Agendamento agendamentoSalvo;

    // Identifica se alguma reserva do cliente foi encontrada
    int encontrou = 0;

    // Abre o arquivo de agendamentos para leitura
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    // Verifica se o arquivo existe e foi aberto
    if (arquivo == NULL) {
        printf("Nenhum agendamento encontrado!\n");
        return;
    }

    // Le todos os agendamentos armazenados no arquivo
    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento), 1, arquivo) == 1) {

        // Mostra apenas os registros do cliente logado
        if (agendamentoSalvo.idCliente == idCliente) {

            // Indica que encontrou uma reserva desse cliente
            encontrou = 1;

            // Exibe as informacoes do agendamento
            printf("\n ===== AGENDAMENTO =====\n");
            printf("Agendamento ID: %d\n", agendamentoSalvo.id);
            printf("Cliente responsavel ID: %d\n",
                   agendamentoSalvo.idCliente);
            printf("Nome: %s\n", agendamentoSalvo.nomeAtendimento);

            printf("Data: %02d/%02d/%d\n",
                   agendamentoSalvo.dia,
                   agendamentoSalvo.mes,
                   agendamentoSalvo.ano);

            printf("Horario: %02d:00\n", agendamentoSalvo.hora);
        }
    }

    // Fecha o arquivo depois de consultar todos os registros
    fclose(arquivo);

    // Avisa quando o cliente nao possui reservas cadastradas
    if (encontrou == 0) {
        printf("Voce ainda nao possui agendamento!\n");
    }
}


// ============================================================
// CANCELAR AGENDAMENTO
// ============================================================

// Cancela uma reserva, se ela pertencer ao cliente e estiver no prazo
void cancelarAgendamento(int idCliente) {

    // Armazena o ID da reserva que o cliente deseja cancelar
    int idAgendamento;

    // Ponteiro para ler o arquivo original de agendamentos
    FILE *arquivo;

    // Ponteiro usado para criar e gravar o arquivo temporario
    FILE *arquivoTemporario;

    // Estrutura usada para ler as reservas cadastradas
    struct Agendamento agendamentoSalvo;

    // Controla se o agendamento informado foi encontrado
    int encontrou = 0;

    // Controla se o cliente tem permissao para cancelar
    int autorizado = 0;

    // Armazena a confirmacao do cancelamento
    int confirmar = 0;


    // ========================================================
    // SOLICITAR O ID DO AGENDAMENTO
    // ========================================================

    printf("\n===== CANCELAR AGENDAMENTO =====\n");
    printf("Digite o ID do agendamento que deseja cancelar: ");

    // Verifica se foi digitado um numero inteiro
    if (scanf("%d", &idAgendamento) != 1) {
        printf("Digite apenas numeros!\n");

        // Limpa os caracteres invalidos
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    // Impede IDs iguais ou menores que zero
    if (idAgendamento <= 0) {
        printf("ID do agedamento invalido!\n");
        return;
    }

    // Mostra o ID escolhido pelo cliente
    printf("ID escolhido: %d\n", idAgendamento);


    // ========================================================
    // LOCALIZAR O AGENDAMENTO E VERIFICAR O RESPONSAVEL
    // ========================================================

    // Abre o arquivo contendo os agendamentos
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {
        printf("Nenhum agnedamento encontrado!\n");
        return;
    }

    // Procura a reserva e confere se pertence ao cliente
    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento), 1, arquivo) == 1) {

        // Verifica se encontrou o ID informado
        if (agendamentoSalvo.id == idAgendamento) {

            encontrou = 1;

            // Confere se a reserva pertence ao cliente logado
            if (agendamentoSalvo.idCliente == idCliente) {

                autorizado = 1;

                // Mostra as informacoes da reserva encontrada
                printf("Agendamento encontrado!\n");
                printf("Nome: %s\n", agendamentoSalvo.nomeAtendimento);

                printf("Data: %02d/%02d/%d\n",
                       agendamentoSalvo.dia,
                       agendamentoSalvo.mes,
                       agendamentoSalvo.ano);

                printf("Horario: %02d:00\n", agendamentoSalvo.hora);

            } else {

                // Informa quando a reserva pertence a outra conta
                printf("Esse agendamento nao pertence a sua conta!\n");
            }

            // Interrompe a busca depois de localizar o ID
            break;
        }
    }

    // Fecha o arquivo apos a consulta
    fclose(arquivo);

    // Verifica se o ID informado existe
    if (encontrou == 0) {
        printf("Agendamento nao encontrado!\n");
        return;
    }

    // Impede o cancelamento por outro cliente
    if (autorizado == 0) {
        printf("Voce nao tem permissao para cancelar esse agendamento!\n");
        return;
    }


    // ========================================================
    // VERIFICAR O PRAZO PARA CANCELAMENTO
    // ========================================================

    // Estrutura que armazena a data e a hora da reserva
    struct tm dataAgendamento = {0};

    // Preenche a data e a hora do agendamento encontrado
    dataAgendamento.tm_mday = agendamentoSalvo.dia;
    dataAgendamento.tm_mon = agendamentoSalvo.mes - 1;
    dataAgendamento.tm_year = agendamentoSalvo.ano - 1900;
    dataAgendamento.tm_hour = agendamentoSalvo.hora;
    dataAgendamento.tm_isdst = -1;

    // Converte a data do agendamento para um instante de tempo
    time_t horarioAgendado = mktime(&dataAgendamento);

    // Consulta a data e a hora atuais
    time_t horarioAtual = time(NULL);

    // Verifica se houve erro na conversao das datas
    if (horarioAgendado == (time_t)-1 ||
        horarioAtual == (time_t)-1) {

        printf("Erro ao verificar a data do agendamento!\n");
        return;
    }

    // Calcula a diferenca em segundos entre a reserva e agora
    double segundosRestantes = difftime(horarioAgendado, horarioAtual);

    // So permite cancelar com pelo menos 2 horas de antecedencia
    // 7200 segundos correspondem a 2 horas
    if (segundosRestantes < 7200) {

        printf("Cancelamento nao permitido!\n");
        printf("O agendamento deve ser cancelado com pelo menos 2 horas de antecedencia.\n");
        return;
    }


    // ========================================================
    // CONFIRMAR O CANCELAMENTO
    // ========================================================

    // Pede confirmacao antes de alterar o arquivo
    printf("\nDeseja realmente cancelar esse agendamento?\n");

    printf("1 - Sim, cancelar\n");
    printf("2 - Nao, voltar\n");
    printf("Escolha uma opcao: ");

    // Verifica se a confirmacao e um numero
    if (scanf("%d", &confirmar) != 1) {
        printf("Digite apenas numeros!\n");

        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
        }

        return;
    }

    // Se nao confirmar com a opcao 1, encerra a operacao
    if (confirmar != 1) {
        printf("Cancelamento nao realizado!\n");
        return;
    }

    printf("Processando cancelamento...\n");


    // ========================================================
    // ABRIR O ARQUIVO ORIGINAL
    // ========================================================

    // Abre o arquivo que contem todas as reservas
    arquivo = fopen("../Dados/agendamentos.dat", "rb");

    if (arquivo == NULL) {
        printf("Erro ao abrir os agendamentos!\n");
        return;
    }


    // ========================================================
    // CRIAR O ARQUIVO TEMPORARIO
    // ========================================================

    // Cria um arquivo temporario para guardar as outras reservas
    // "wb" abre o arquivo para escrita binaria
    arquivoTemporario = fopen("../Dados/agendamentos_temp.dat", "wb");

    // Verifica se o arquivo temporario foi criado
    if (arquivoTemporario == NULL) {

        printf("Erro ao criar o arquivo temporario!\n");

        // Fecha o original antes de encerrar a operacao
        fclose(arquivo);
        return;
    }


    // ========================================================
    // COPIAR AS RESERVAS QUE DEVEM SER MANTIDAS
    // ========================================================

    // Copia tudo, menos o agendamento escolhido
    while (fread(&agendamentoSalvo,
                 sizeof(struct Agendamento), 1, arquivo) == 1) {

        // Nao copia o registro que sera cancelado
        if (agendamentoSalvo.id != idAgendamento) {

            // Grava no temporario a reserva que deve permanecer
            if (fwrite(&agendamentoSalvo,
                       sizeof(struct Agendamento),
                       1, arquivoTemporario) != 1) {

                printf("Erro ao copiar agendamento!\n");

                // Fecha os arquivos em caso de falha
                fclose(arquivo);
                fclose(arquivoTemporario);

                // Remove o arquivo temporario incompleto
                remove("../Dados/agendamentos_temp.dat");
                return;
            }
        }
    }


    // ========================================================
    // VERIFICAR ERROS DE LEITURA
    // ========================================================

    // Evita trocar o arquivo caso a leitura tenha falhado
    if (ferror(arquivo)) {

        printf("Erro ao ler os agendamentos!\n");

        fclose(arquivo);
        fclose(arquivoTemporario);

        // Remove o arquivo temporario
        remove("../Dados/agendamentos_temp.dat");
        return;
    }


    // ========================================================
    // FECHAR OS ARQUIVOS
    // ========================================================

    // Fecha o original e o temporario
    // Guarda o resultado de cada fclose para verificar falhas
    int erroOriginal = fclose(arquivo);
    int erroTemporario = fclose(arquivoTemporario);

    // Verifica se o fechamento dos arquivos apresentou erros
    if (erroOriginal != 0 || erroTemporario != 0) {

        printf("Erro ao fechar os arquivos!\n");

        // Remove o temporario quando nao foi possivel finalizar
        remove("../Dados/agendamentos_temp.dat");
        return;
    }


    // ========================================================
    // VERIFICAR SE JA EXISTE UM BACKUP
    // ========================================================

    // Abre o caminho do backup anterior somente para consulta
    FILE *backup = fopen(
        "../Dados/agendamentos_cancelamento_backup.dat", "rb");

    // Se existe backup, nao realiza outra substituicao
    if (backup != NULL) {

        fclose(backup);

        printf("Ja existe um backup. Resolva isso antes de cancelar!\n");

        // Remove o arquivo temporario criado nesta tentativa
        remove("../Dados/agendamentos_temp.dat");
        return;
    }


    // ========================================================
    // CRIAR BACKUP DO ARQUIVO ORIGINAL
    // ========================================================

    // Renomeia o arquivo original para preservar uma copia
    if (rename("../Dados/agendamentos.dat",
               "../Dados/agendamentos_cancelamento_backup.dat") != 0) {

        printf("Erro ao criar backup!\n");
        return;
    }


    // ========================================================
    // SUBSTITUIR O ARQUIVO ORIGINAL
    // ========================================================

    // Renomeia o temporario para o nome do arquivo principal
    // Assim o arquivo novo nao contem a reserva cancelada
    if (rename("../Dados/agendamentos_temp.dat",
               "../Dados/agendamentos.dat") != 0) {

        printf("Erro ao finalizar o cancelamento!\n");

        // Tenta restaurar o arquivo original usando o backup
        if (rename("../Dados/agendamentos_cancelamento_backup.dat",
                   "../Dados/agendamentos.dat") != 0) {

            printf("Erro: restaure o arquivo original pelo backup!\n");
        }

        return;
    }


    // ========================================================
    // REMOVER O BACKUP APOS O CANCELAMENTO
    // ========================================================

    // Remove o backup quando o arquivo novo foi criado
    if (remove("../Dados/agendamentos_cancelamento_backup.dat") != 0) {
        printf("Aviso: nao foi possivel remover o backup!\n");
    }

    // Confirma a conclusao do cancelamento
    printf("Agendamento cancelado com sucesso!\n");
}
