// Evita que este arquivo seja incluido mais de uma vez
#ifndef HORARIO_BARBEIRO_H
#define HORARIO_BARBEIRO_H

// Estrutura responsavel por armazenar
// os dias e horarios de trabalho do barbeiro
struct HorarioBarbeiro {

    // Dia da semana: 1 (segunda) ate 7 (domingo)
    int dia;

    // Horario em que o barbeiro inicia o expediente
    int horaInicio;

    // Horario em que o barbeiro encerra o expediente
    int horaFim;
};

// Finaliza a protecao contra inclusoes duplicadas
#endif
