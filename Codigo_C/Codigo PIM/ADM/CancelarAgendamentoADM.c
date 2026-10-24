
#include <stdio.h>

// Inclui a estrutura que armazena os dados dos agendamentos
#include "../Agendamento/Agendamento.h"

// Funcao responsavel por cancelar agendamentos pelo administrador
void cancelarAgendamentoADM(void){

    // ID do agendamento que sera cancelado
    int idAgendamento;

    // Indica se o agendamento foi encontrado
    int encontrado = 0;

    // Armazena a confirmacao do administrador
    char confirmar;

    // Ponteiros utilizados para acessar os arquivos
    FILE *arquivo;
    FILE *temporario;

    // Estrutura que recebe cada agendamento lido
    struct Agendamento agendamento;

    // Exibe o titulo da funcao
    printf("\n===== CANCELAR AGENDAMENTO ADM =====\n");

    // Solicita o ID do agendamento
    printf("Digite o ID do agendamento: ");
    scanf("%d", &idAgendamento);

    // Abre o arquivo original para leitura binaria
    arquivo = fopen("../../Dados/agendamentos.dat", "rb");

    // Verifica se o arquivo foi aberto corretamente
    if(arquivo == NULL) {
        printf("Erro ao abrir o arquivo de agendamentos!\n");
        return;
    }

    // Percorre os agendamentos procurando o ID informado
    while (fread(&agendamento, sizeof(struct Agendamento), 1, arquivo) == 1){

        // Verifica se o ID corresponde ao agendamento procurado
        if (agendamento.id == idAgendamento) {

            // Exibe os dados do agendamento encontrado
            printf("Agendamento encontrado!\n");
            printf("Nome: %s\n", agendamento.nomeAtendimento);
            printf("Data: %02d/%02d/%04d\n",
                   agendamento.dia, agendamento.mes, agendamento.ano);
            printf("Horario: %02d:00\n", agendamento.hora);

            // Marca que o agendamento foi encontrado
            encontrado = 1;

            // Interrompe a busca
            break;
        }
    }

    // Executa a confirmacao somente se encontrou o agendamento
    if(encontrado == 1){

        // Pergunta se o administrador deseja cancelar
        printf("Deseja realmente cancelar este agendamento? (S/N): ");
        scanf(" %c", &confirmar);

        // Aceita S maiusculo ou minusculo
        if(confirmar == 'S' || confirmar == 's'){

            // Cria um arquivo temporario para guardar os registros restantes
            temporario = fopen("../../Dados/agendamentos_temp_adm.dat", "wb");

            // Verifica se o arquivo temporario foi criado
            if(temporario == NULL){
                printf("Erro ao criar arquivo temporario!\n");
                fclose(arquivo);
                return;
            }

            // Retorna ao inicio do arquivo original
            rewind(arquivo);

            // Le novamente todos os agendamentos
            while(fread(&agendamento, sizeof(struct Agendamento), 1, arquivo) == 1){

                // Copia somente os agendamentos com ID diferente do escolhido
                if (agendamento.id != idAgendamento){

                    // Grava o registro no arquivo temporario
                    if (fwrite(&agendamento, sizeof(struct Agendamento), 1, temporario) != 1) {
                        printf("Erro ao copiar agendamento!\n");

                        // Fecha os arquivos abertos
                        fclose(temporario);
                        fclose(arquivo);

                        // Remove o arquivo temporario
                        remove("../../Dados/agendamentos_temp_adm.dat");
                        return;
                    }
                }
            }

            // Verifica se ocorreu algum erro durante a leitura
            if (ferror(arquivo)){
                printf("Erro durante a leitura dos agendamentos!\n");

                // Fecha os arquivos
                fclose(temporario);
                fclose(arquivo);

                // Remove o temporario caso a leitura tenha falhado
                remove("../../Dados/agendamentos_temp_adm.dat");
                return;
            }

            // Fecha o arquivo temporario e verifica se houve erro
            if (fclose(temporario) != 0) {
                printf("Erro ao finalizar o arquivo temporario!\n");

                fclose(arquivo);

                remove("../../Dados/agendamentos_temp_adm.dat");
                return;
            }

            // Fecha o arquivo original antes de renomea-lo
            if(fclose(arquivo) != 0){
                printf("Erro ao fechar o arquivo original!\n");

                remove("../../Dados/agendamentos_temp_adm.dat");
                return;
            }

            // Verifica se ja existe um arquivo de backup
            FILE *backup = fopen("../../Dados/agendamentos_backup_adm.dat", "rb");

            if (backup != NULL){
                fclose(backup);

                printf("Ja existe um backup! Verifique a pasta Dados.\n");

                remove("../../Dados/agendamentos_temp_adm.dat");
                return;
            }

            // Renomeia o arquivo original para criar um backup
            if (rename("../../Dados/agendamentos.dat",
                       "../../Dados/agendamentos_backup_adm.dat") != 0){

                printf("Erro ao criar backup dos agendamentos!\n");

                remove("../../Dados/agendamentos_temp_adm.dat");
                return;
            }

            // Coloca o arquivo temporario no lugar do original
            if (rename("../../Dados/agendamentos_temp_adm.dat",
                       "../../Dados/agendamentos.dat") != 0) {

                printf("Erro ao atualizar os agendamentos!\n");

                // Tenta restaurar o backup caso a substituicao falhe
                if(rename("../../Dados/agendamentos_backup_adm.dat",
                          "../../Dados/agendamentos.dat") != 0){

                    printf("Erro ao restaurar backup! Verifique a pasta Dados.\n");
                }

                return;
            }

            // Remove o backup apos a atualizacao
            if(remove("../../Dados/agendamentos_backup_adm.dat") != 0) {
                printf("Agendamento cancelado, mas o backup nao foi removido!\n");
            }else{
                printf("Agendamento cancelado com sucesso!\n");
            }

            // Encerra a funcao para evitar fechar o arquivo novamente
            return;

        }else{
            // Informa que o administrador desistiu do cancelamento
            printf("Cancelamento desistido!\n");
        }
    }

    // Informa caso o ID digitado nao exista
    if (encontrado == 0){
        printf("Agendamento nao encontrado!\n");
    }

    // Fecha o arquivo quando nao houve cancelamento
    fclose(arquivo);
}
