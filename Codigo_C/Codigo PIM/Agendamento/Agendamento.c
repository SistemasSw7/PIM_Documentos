
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Agendamento.h"
#include "../Cliente/Cliente.h"

void agendarHorario( int idCliente) {

    int dia;
    int mes;
    int ano;
    int bissexto;
    int hora;
    int horaValida;

    int dataValida = 0;
    int horarioOcupado = 0;
    int maiorId = 0;
    int horariosOcupados[10] = {0};
    int temHOrarioLivre = 0;
    int tipoAtendimento = 0;
    int encontrouCliente = 0;
    char nomeAtendimento[50];

    
    FILE *arquivo;
    struct Agendamento novoAgendamento;
    struct Agendamento agendamentoSalvo;
    struct Cliente clienteEncontrado;

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

    printf("Data atual: %02d/%02d/%d\n", dataAtual->tm_mday, dataAtual->tm_mon + 1, dataAtual->tm_year + 1900);

    printf("\n===== AGENDAR HORARIO =====\n");
    printf("Cliente ID: %d\n", idCliente);
    do{
        printf("\nPara quem sera o atendimento?\n");
        printf("1 - Para mim\n");
        printf("2 - Para outra pessoa\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &tipoAtendimento);

        if (tipoAtendimento != 1 && tipoAtendimento != 2) {
            printf("Opcao invalida!\n");
        }
    }while (tipoAtendimento != 1 && tipoAtendimento != 2);

    if (tipoAtendimento == 2) {
        printf("Digite o nome e o sobre nome da pessoa: ");
        scanf(" %49[^\n]", nomeAtendimento);
    }

    if (tipoAtendimento == 1) {
        arquivo = fopen("../../Dados/cliente.dat", "rb");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo de Cliente!\n");
            return;
        }
        while (fread(&clienteEncontrado, sizeof(struct Cliente), 1, arquivo) == 1){
            if (clienteEncontrado.id == idCliente){
                strcpy(nomeAtendimento, clienteEncontrado.nome);
                printf("Cliente encontrado: %s\n", clienteEncontrado.nome);
                encontrouCliente = 1;
                break;
            }
        }
        fclose(arquivo);

        if(encontrouCliente == 0){
            printf("Cliente nao encontrado!\n");
            return;
        }
    }


    do{
        dataValida = 1;

        do{
            printf("Digite o dia do agendamento: ");
            scanf("%d", &dia);
            if (dia < 1 || dia > 31) {
                printf("Dia invalido!\n");
            }
        }while (dia < 1 || dia > 31);

        do{
            printf("Digite o mes do agendamento: ");
            scanf("%d", &mes);
            if (mes < 1 || mes > 12) {
                printf("Mes invalido!\n");
            }
        }while (mes < 1 || mes > 12);

        do{
            printf("Digite o ano do agendamento: ");
            scanf("%d", &ano);
            if (ano < anoAtual) {
                printf("Ano invalido\n");

            }
        }while (ano < anoAtual);

        bissexto = (ano % 4 == 0 && ano % 100 != 0 ) || ano % 400 == 0;

        if (dia == 31 && (mes == 4 || mes == 6 || mes == 9 || mes == 11)) {
            printf("Data invalida!\n");
            dataValida = 0;

        }

        if(mes == 2) {
            if (dia > 29){
                printf("Data invalida!\n");
                dataValida = 0;
            }

            if (dia == 29 && bissexto == 0) {
                printf("Data invalida!\n");
                dataValida = 0;
            }
        }
        if (ano == anoAtual && mes < mesAtual){
            printf("Essa data ja passou!\n");
            dataValida = 0;
        }
        if (ano == anoAtual && mes == mesAtual && dia < diaAtual){
            printf("Esse dia ja passou!\n");
            dataValida = 0;
        }
        }while (dataValida == 0);

        arquivo = fopen("../../Dados/agendamentos.dat", "rb");
        if (arquivo != NULL){
            while (fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1) {
                if (agendamentoSalvo.dia == dia && agendamentoSalvo.mes == mes && agendamentoSalvo.ano == ano && agendamentoSalvo.hora >= 9 && agendamentoSalvo.hora <= 18) {
                    horariosOcupados[agendamentoSalvo.hora - 9] = 1;
                }
            }
            fclose(arquivo);
        }

        printf("\n===== HORARIOS DA BARBEARIA =====\n");

        for (int i = 9; i <= 18; i++) {
            if (ano == anoAtual && mes == mesAtual && dia == diaAtual && i <= horaAtual) {
                printf("%02d:00 - INDISPONIVEL\n", i);

            }else if (horariosOcupados[i - 9] == 1){
                printf("%02d:00 - OCUPADO\n", i);

            }else{
                printf("%02d:00 - LIVRE\n", i);
                temHOrarioLivre = 1;
            }   
        }
        if (temHOrarioLivre == 0) {
            printf("Nao existe horarios disponiveis nessa data!\n");
            return;
        }

        do{
            horaValida = 1;

            printf("Digite o horario do agendamento (9 as 18): ");
            scanf("%d", &hora);

            if (hora < 9 || hora > 18) {
                printf("Barbearia fechada!\n");
                horaValida = 0;
            }
            if (hora >= 9 && hora <= 18) {
                if (horariosOcupados[hora - 9] == 1) {
                    printf("Esse horario ja esta ocupado! Escolha outro.\n");
                    horaValida = 0;
                }
            }
            if (ano == anoAtual && mes == mesAtual && dia == diaAtual && hora <= horaAtual) {
                printf("Esse horario ja passou!\n");
                horaValida = 0;
            }
        }while (horaValida == 0);
        arquivo = fopen("../../Dados/agendamentos.dat", "rb");

        if (arquivo == NULL) {
            printf("Nenhum agendamento salvo ainda.\n");
        }else {
            printf("Arquivo de agendamentos encontrado!\n");

            while (fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1) {
                printf("Agendamento encontrado: %02d/%02d/%d as %02d:00\n", agendamentoSalvo.dia, agendamentoSalvo.mes, agendamentoSalvo.ano, agendamentoSalvo.hora);
                if(agendamentoSalvo.dia == dia && agendamentoSalvo.mes == mes && agendamentoSalvo.ano == ano && agendamentoSalvo.hora == hora) {
                    printf("Esse horario ja esta ocupado!\n");
                    horarioOcupado = 1;
                }
                if (agendamentoSalvo.id > maiorId) {
                    maiorId = agendamentoSalvo.id;
                }
            }
            fclose(arquivo);
        }
        if (horarioOcupado == 1){
            printf("Nao foi possivel realizar o agendamento!\n");
            return;
        }

        novoAgendamento.id = maiorId + 1;   
        novoAgendamento.idCliente = idCliente;
        strcpy(novoAgendamento.nomeAtendimento, nomeAtendimento);
        novoAgendamento.dia = dia;
        novoAgendamento.mes = mes;
        novoAgendamento.ano = ano;
        novoAgendamento.hora = hora;
        arquivo = fopen("../../Dados/agendamentos.dat", "ab");
        if (arquivo == NULL) {
            printf("Erro ao abrir o arquivo de agendamentos!\n");
            return;
        }
        if (fwrite(&novoAgendamento, sizeof(struct Agendamento), 1, arquivo) != 1) {
            printf("Erro ao salvar agendamento!\n");
            fclose(arquivo);
            return;
        }
        fclose(arquivo);

        printf("\n===== RESUMO DO AGENDAMENTO =====\n");
        printf("Agendamento ID: %d\n", novoAgendamento.id);
        printf("Cliente ID: %d\n", novoAgendamento.idCliente);
        printf("Nome de quem sera atendido: %s\n", novoAgendamento.nomeAtendimento);
        printf("Data: %02d/%02d/%d\n", novoAgendamento.dia, novoAgendamento.mes, novoAgendamento.ano);
        printf("Horario: %02d:00\n", novoAgendamento.hora);
        printf("Agendamento realizado com sucesso!\n");

        
        

}