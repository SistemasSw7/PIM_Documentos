#include <stdio.h>
#include <time.h>
#include "ValidarData.h"

//Funcao responsavel por validar as datas
int validarData(int dia, int mes, int ano){
    
    //Verifica se o dia esta entre 1 e 31
    if (dia < 1 || dia > 31){
        return 0;
    }

    //Verifica se o mes esta entre 1 e 12
    if (mes < 1 || mes > 12){
        return 0;
    }

    //Verifica se o ano esta no intervalo permitido
    if(ano < 1900 || ano > 9999){
        return 0;
    }
    
    //Verifica se o ano e bissexto
    int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);

    //Inicialmente considera que o mes possui 31 dias
    int maxDiasMes = 31;

    //Verifica quantos dias fevereiro possui
    if(mes == 2){
        if(bissexto == 1){
            maxDiasMes = 29;
        }else{
            maxDiasMes = 28;
        }
    }

    // Verifica se o dia informado existe naquele mes
    if(dia > maxDiasMes){
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
    int mesAtual = dataAtual -> tm_mon;
    int anoAtual = dataAtual -> tm_year + 1900;

    //Verifica se o ano informado ja passou
    if(ano == anoAtual && mes < mesAtual){
        return 0;
    }

    //Verifica se o dia ja passou no mes atual
    if(ano == anoAtual && mes == mesAtual && dia < diaAtual){
        return 0;
    }

    // Se passou todas as verificacoes, a data e valida
    return 1;
}