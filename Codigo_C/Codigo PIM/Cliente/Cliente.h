#ifndef CLIENTE_H
#define CLIENTE_H

struct Cliente {
    int id;
    char nome[50];
    char email[100];
    char telefone[20];
    char senha[50];
};

#endif