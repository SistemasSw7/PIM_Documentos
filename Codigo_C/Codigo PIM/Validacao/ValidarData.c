#include <stdio.h>
#include <time.h>
#include "ValidarData.h"

//Funcao responsavel por validar as datas
int validarData(int dia, int mes, int ano){
    
    // Verifica se a data existe no calendario
    if(validarDataCalendario(dia, mes, ano) == 0){
        return 0;

    }
    
    // Consulta a data atual do computador
    time_t agora = time(NULL);
    struct tm *dataAtual = localtime(&agora);

    //Verifica se foi possivel obter a data atual
    if(dataAtual == NULL){
        return 0;
    }

    //Armazena o dia, mes e o ano atuais
    int diaAtual = dataAtual -> tm_mday;
    int mesAtual = dataAtual -> tm_mon + 1;
    int anoAtual = dataAtual -> tm_year + 1900;

    //Verifica se a data informada ja passou
    if(ano < anoAtual || (ano == anoAtual && mes < mesAtual) || (ano == anoAtual && mes == mesAtual && dia < diaAtual)){
        return 0;
    }
    // Se passou todas as verificacoes, a data e valida
    return 1;
}
//Verifica se a data exist no calendario
//Permite consultar datas passadas
int validarDataCalendario(int dia, int mes, int ano){

    //Verifica o intervalo do ano e do mes
    if(ano < 1900 || ano > 9999 || mes < 1 || mes > 12){
        return 0;
    }

    //Quantidade de dias de cada mes
    int diasMes[] = {
        31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
    };

    //Verifica se o ano e bissexto
    int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);

    //Fevereiro possui 29 dias em ano bissexto
    if(bissexto){
        diasMes[1] = 29;
    }

    //Verifica se o dia existe naquele mes
    if(dia < 1 || dia > diasMes[mes - 1]){
        return 0;
    }

    return 1;
}