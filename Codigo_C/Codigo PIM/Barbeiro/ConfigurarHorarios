
#include <stdio.h>

// Inclui a estrutura HorarioBarbeiro,
// que armazena o dia e os horarios de trabalho
#include "HorarioBarbeiro.h"

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
