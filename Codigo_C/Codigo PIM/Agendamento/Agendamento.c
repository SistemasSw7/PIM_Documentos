#include <stdio.h>
#include <time.h>
#include <string.h>
#include "Agendamento.h"
#include "../Cliente/Cliente.h"

// Faz um novo agendamento para o cliente logado.
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

    // Consulta a data e a hora atuais.
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
    }while (tipoAtendimento != 1 && tipoAtendimento != 2);

    // Permite agendar para outra pessoa.
    if (tipoAtendimento == 2) {
        printf("Digite o nome e o sobre nome da pessoa: ");
        scanf(" %49[^\n]", nomeAtendimento);
    }

    // Busca o nome do cliente, caso o atendimento seja para ele.
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

        // Pede e valida a data escolhida.
        dataValida = 1;
        do{
            printf("Digite o dia do agendamento: ");
            if(scanf("%d", &dia) != 1) {
                printf("Digite apenas numeros!\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF){
                }
                dia = 0;
            }
            if (dia < 1 || dia > 31) {
                printf("Dia invalido!\n");
            }
        }while (dia < 1 || dia > 31);
        do{
            printf("Digite o mes do agendamento: ");
            if(scanf("%d", &mes) != 1) {
                printf("Digite apenas numeros!\n");
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {
                }
                mes = 0;
            }
            if(mes < 1 || mes > 12){
                printf("Mes invalido!\n");
            }
        }while (mes < 1 || mes > 12);
        do{
            printf("Digite o ano do agendamento: ");
            if(scanf("%d", &ano) != 1) {
                printf("Digite apenas numeros!\n");
                int c;
                while((c = getchar()) != '\n' && c != EOF){
                }
                ano = 0;
            }
            if (ano < anoAtual){
                printf("Ano invalido!\n");
            }
        }while (ano < anoAtual);

        // Verifica se o ano e bissexto.
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

        // Le os horarios ja reservados e identifica os ocupados.
        arquivo = fopen("../../Dados/agendamentos.dat", "rb");
        if (arquivo != NULL){
            while (fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1) {
                if (agendamentoSalvo.dia == dia && agendamentoSalvo.mes == mes && agendamentoSalvo.ano == ano && agendamentoSalvo.hora >= 9 && agendamentoSalvo.hora <= 18) {
                    horariosOcupados[agendamentoSalvo.hora - 9] = 1;
                }
            }
            fclose(arquivo);
        }

        // Mostra os horarios livres e indisponiveis.
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

            // Verifica se o horario escolhido pode ser reservado.
            horaValida = 1;
            printf("Digite o horario do agendamento (9 as 18): ");
            if(scanf("%d", &hora) != 1){
                printf("Digite apenas numeros!\n");
                int c;
                while((c = getchar()) != '\n' && c != EOF) {
                }
                hora = 0;
            }
            if (hora < 9 || hora > 18){
                printf("Barbearia fechada!\n");
                horaValida = 0;
            }
            if(hora >= 9 && hora <= 18) {
                if (horariosOcupados[hora - 9] == 1){
                    printf("Esse horario ja esta ocupado! Escolha outro.\n");
                    horaValida = 0;
                }
            }
            if (ano == anoAtual && mes == mesAtual && dia == diaAtual && hora <= horaAtual) {
                printf("Esse horario ja passou!\n");
                horaValida = 0;
            }
        }while (horaValida == 0);

        // Confere se o horario esta livre e calcula o proximo ID.
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

        // Preenche os dados da nova reserva.
        novoAgendamento.id = maiorId + 1;
        novoAgendamento.idCliente = idCliente;
        strcpy(novoAgendamento.nomeAtendimento, nomeAtendimento);
        novoAgendamento.dia = dia;
        novoAgendamento.mes = mes;
        novoAgendamento.ano = ano;
        novoAgendamento.hora = hora;

        // Salva a reserva no arquivo.
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

        // Exibe o resumo para o cliente.
        printf("\n===== RESUMO DO AGENDAMENTO =====\n");
        printf("Agendamento ID: %d\n", novoAgendamento.id);
        printf("Cliente ID: %d\n", novoAgendamento.idCliente);
        printf("Nome de quem sera atendido: %s\n", novoAgendamento.nomeAtendimento);
        printf("Data: %02d/%02d/%d\n", novoAgendamento.dia, novoAgendamento.mes, novoAgendamento.ano);
        printf("Horario: %02d:00\n", novoAgendamento.hora);
        printf("Agendamento realizado com sucesso!\n");
}

// Mostra apenas os agendamentos do cliente logado.
void meusAgendamentos(int idCliente) {
    FILE *arquivo;
    struct Agendamento agendamentoSalvo;
    int encontrou = 0;
    arquivo = fopen("../../Dados/agendamentos.dat", "rb");
    if (arquivo == NULL) {
        printf("Nenhum agendamento encontrado!\n");
        return;
    }

    // Le as reservas e filtra pelo ID do cliente.
    while(fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1) {
        if (agendamentoSalvo.idCliente == idCliente) {
            encontrou = 1;
            printf("\n ===== AGENDAMENTO =====\n");
            printf("Agendamento ID: %d\n", agendamentoSalvo.id);
            printf("Cliente responsavel ID: %d\n", agendamentoSalvo.idCliente);
            printf("Nome: %s\n", agendamentoSalvo.nomeAtendimento);
            printf("Data: %02d/%02d/%d\n", agendamentoSalvo.dia, agendamentoSalvo.mes, agendamentoSalvo.ano);
            printf("Horario: %02d:00\n", agendamentoSalvo.hora);
        }
    }
    fclose(arquivo);
    if(encontrou == 0) {
        printf("Voce ainda nao possui agendamento!\n");
    }
}

// Cancela uma reserva, se ela pertencer ao cliente e estiver no prazo.
void cancelarAgendamento(int idCliente) {
    int idAgendamento;
    FILE *arquivo;
    FILE *arquivoTemporario;
    struct Agendamento agendamentoSalvo;
    int encontrou = 0;
    int autorizado = 0;
    int confirmar = 0;

    // Pede o ID da reserva que sera cancelada.
    printf("\n===== CANCELAR AGENDAMENTO =====\n");
    printf("Digite o ID do agendamento que deseja cancelar: ");
    if(scanf("%d", &idAgendamento) != 1) {
        printf("Digite apenas numeros!\n");
        int c;
        while((c = getchar()) != '\n' && c != EOF) {
        }
        return;
    }
     if(idAgendamento <= 0) {
        printf("ID do agedamento invalido!\n");
        return;
    }
    printf("ID escolhido: %d\n", idAgendamento);
    arquivo = fopen("../../Dados/agendamentos.dat", "rb");
    if (arquivo == NULL) {
        printf("Nenhum agnedamento encontrado!\n");
        return;
    }

    // Procura a reserva e confere se pertence ao cliente.
    while (fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1) {
        if(agendamentoSalvo.id == idAgendamento){
            encontrou = 1;
            if (agendamentoSalvo.idCliente == idCliente) {
                autorizado = 1;
                printf("Agendamento encontrado!\n");
                printf("Nome: %s\n", agendamentoSalvo.nomeAtendimento);
                printf("Data: %02d/%02d/%d\n", agendamentoSalvo.dia, agendamentoSalvo.mes, agendamentoSalvo.ano);
                printf("Horario: %02d:00\n", agendamentoSalvo.hora);
            }else{
                printf("Esse agendamento nao pertence a sua conta!\n");
            }
            break;
        }
    }
    fclose(arquivo);
    if(encontrou == 0) {
        printf("Agendamento nao encontrado!\n");
        return;
    }
    if(autorizado == 0) {
        printf("Voce nao tem permissao para cancelar esse agendamento!\n");
        return;
    }

    // Calcula quanto tempo falta para o atendimento.
    struct tm dataAgendamento = {0};
dataAgendamento.tm_mday = agendamentoSalvo.dia;
dataAgendamento.tm_mon = agendamentoSalvo.mes - 1;
dataAgendamento.tm_year = agendamentoSalvo.ano - 1900;
dataAgendamento.tm_hour = agendamentoSalvo.hora;
dataAgendamento.tm_isdst = -1;
time_t horarioAgendado = mktime(&dataAgendamento);
time_t horarioAtual = time(NULL);
if (horarioAgendado == (time_t)-1 || horarioAtual == (time_t)-1) {
    printf("Erro ao verificar a data do agendamento!\n");
    return;
}
double segundosRestantes = difftime(horarioAgendado, horarioAtual);

// So permite cancelar com pelo menos 2 horas de antecedencia.
if (segundosRestantes < 7200) {
    printf("Cancelamento nao permitido!\n");
    printf("O agendamento deve ser cancelado com pelo menos 2 horas de antecedencia.\n");
    return;
}

    // Pede confirmacao antes de alterar o arquivo.
    printf("\nDeseja realmente cancelar esse agendamento?\n");
    printf("1 - Sim, cancelar\n");
    printf("2 - Nao, voltar\n");
    printf("Escolha uma opcao: ");
    if(scanf("%d", &confirmar) != 1) {
        printf("Digite apenas numeros!\n");
        int c;
        while((c = getchar()) != '\n' && c != EOF){
        }
        return;
    }
    if (confirmar != 1) {
        printf("Cancelamento nao realizado!\n");
        return;
    }
    printf("Processando cancelamento...\n");
    arquivo = fopen("../../Dados/agendamentos.dat", "rb");
    if(arquivo == NULL) {
        printf("Erro ao abrir os agendamentos!\n");
        return;
    }

    // Cria um arquivo temporario para guardar as outras reservas.
    arquivoTemporario = fopen("../../Dados/agendamentos_temp.dat", "wb");
    if(arquivoTemporario == NULL) {
        printf("Erro ao criar o arquivo temporario!\n");
        fclose(arquivo);
        return;
    }

    // Copia tudo, menos o agendamento escolhido.
    while(fread(&agendamentoSalvo, sizeof(struct Agendamento), 1, arquivo) == 1){
        if(agendamentoSalvo.id != idAgendamento){
            if(fwrite(&agendamentoSalvo,sizeof(struct Agendamento), 1, arquivoTemporario) != 1){
                printf("Erro ao copiar agendamento!\n");
                fclose(arquivo);
                fclose(arquivoTemporario);
    remove("../../Dados/agendamentos_temp.dat");
    return;
            }
        }
    }

    // Evita trocar o arquivo caso a leitura tenha falhado.
    if(ferror(arquivo)){
        printf("Erro ao ler os agendamentos!\n");
        fclose(arquivo);
        fclose(arquivoTemporario);
    remove("../../Dados/agendamentos_temp.dat");
    return;
    }

    // Fecha os arquivos e verifica se a gravacao terminou.
    int erroOriginal = fclose(arquivo);
    int erroTemporario = fclose(arquivoTemporario);
    if (erroOriginal != 0 || erroTemporario != 0){
        printf("Erro ao fechar os arquivos!\n");
    remove("../../Dados/agendamentos_temp.dat");
        return;
    }

    // Verifica se ja existe um backup antigo.
    FILE *backup = fopen("../../Dados/agendamentos_cancelamento_backup.dat", "rb");
    if(backup != NULL){
        fclose(backup);
        printf("Ja existe um backup. Resolva isso antes de cancelar!\n");
        remove("../../Dados/agendamentos_temp.dat");
        return;
    }

    // Guarda o arquivo original como backup.
    if (rename("../../Dados/agendamentos.dat", "../../Dados/agendamentos_cancelamento_backup.dat") != 0){
        printf("Erro ao criar backup!\n");
        return;
    }

    // Substitui o original pelo arquivo sem a reserva cancelada.
    if(rename("../../Dados/agendamentos_temp.dat", "../../Dados/agendamentos.dat") != 0){
        printf("Erro ao finalizar o cancelamento!\n");
        if(rename("../../Dados/agendamentos_cancelamento_backup.dat", "../../Dados/agendamentos.dat") != 0){
            printf("Erro: restaure o arquivo original pelo backup!\n");
        }
        return;
    }

    // Remove o backup apos o cancelamento dar certo.
    if(remove("../../Dados/agendamentos_cancelamento_backup.dat") != 0){
        printf("Aviso: nao foi possivel remover o backup!\n");
    }
    printf("Agendamento cancelado com sucesso!\n");
}
