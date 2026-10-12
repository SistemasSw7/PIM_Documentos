
#ifndef AGENDAMENTO_H
#define AGENDAMENTO_H

struct Agendamento {
    int id;
    int idCliente;
    char nomeAtendimento[50];
    int dia;
    int mes;
    int ano;
    int hora;

};
#endif