# SPRINT PLANNING

O sprint planning foi dividido em 7 sprints com a duração estimada de 1 semana cada.

## SPRINT 1

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Elicitar informações e objetivos do produto. | DONE |
| T2 | Definir as user stories. | DONE |
| T3 | Levantar requisitos funcionais. | DONE |
| T4 | Levantar requisitos não funcionais. | DONE |
| T5 | Definir as regras de negócio junto do cliente. | DONE |
| T6 | Criar product backlog. | DONE |

No final da primeira sprint o esperado é que tenhamos as informações necessárias para que possamos começar a construir os diagramas que auxiliarão na fase de implementação.

## SPRINT 2

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Criação dos diagramas de caso de uso. | DONE |
| T2 | Criação do modelo entidade relacionamento. | DONE |
| T3 | Criação dos diagramas de classe estendido. | DONE |
| T4 | Criação do modelo lógico do Banco de dados. | DONE |

No final da segunda sprint esperamos ter uma visão macro do que teremos de implementar para que o sistema rode de acordo com o planejado.

## SPRINT 3 (INÍCIO DA IMPLEMENTAÇÃO)

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Criar função cadastro. US05 | DONE |
| T2 | Solicitar nome, telefone, email e data de nascimento. US05 | DONE |
| T3 | Criar login e senha. US05 | DONE |
| T4 | Criar tela de login. US05 | DONE |
| T5 | Permitir edição dos dados de usuário. US05 | DONE |
| T6 | Criar hierarquia de usuário dono, cliente. US01 | DONE |
| T7 | Testar cadastro e login. US05 - A fazer. | DONE |

## SPRINT 4

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Criar tela de cadastro de serviços. US04 | DONE |
| T2 | Deve permitir cadastrar nome e valor do serviço. US04 | DONE |
| T3 | Permitir edições em serviços. US04 | DONE |
| T4 | Exibir serviços cadastrados. US04 | DONE |
| T5 | Criar agenda. US01 | DONE |
| T6 | Permitir cadastrar datas e horários. US01 | DONE  |
| T7 | Permitir editar datas e horários. US01 | DONE  |
| T8 | Exibir horários disponíveis da agenda. US01 | DONE |
| T9 | Testar cadastros de serviços e agenda. US01/US04 | DONE |

Ao final da Sprint 4, o esperado do sistema é que permita o dono criar e editar a agenda, assim como cadastrar e editar serviços como valores e nomes.

## SPRINT 5

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Criar tela de visualização da agenda para o cliente. US06 | DONE |
| T2 | Exibir datas disponíveis para agendamento. US06 | DONE |
| T3 | Exibir horários disponíveis para cada data. US06 | DONE |
| T4 | Ocultar horários que já estejam ocupados. US06 | DONE |
| T5 | Permitir que o cliente selecione um serviço. US07. | DONE |
| T6 | Permitir que o cliente selecione uma data e horário. US07. | DONE |
| T7 | Confirmar o agendamento realizado pelo cliente. US07 | DONE |
| T8 | Registrar o agendamento no sistema. US07 | DONE |
| T9 | Permitir que o cliente visualize seus agendamentos. US07 | DONE |
| T10 | Permitir que o cliente desmarque um horário. US07 | DONE |
| T11 | Atualizar a disponibilidade do horário após o agendamento ou cancelamento. US06/US07 | DONE |
| T12 | Testar o sistema de agendamento e cancelamento. US06/US07 | DONE |

Ao final da Sprint 5, o esperado é que o cliente consiga visualizar os serviços, datas e horários disponíveis, realizar um agendamento e também visualizar ou desmarcar seus próprios horários. A agenda deverá ser atualizada automaticamente após um agendamento ou cancelamento.

## SPRINT 6

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Criar tela de histórico de serviços realizados. US02 | DOING |
| T2 | Registrar o serviço realizado no histórico. US02 | DOING |
| T3 | Exibir qual serviço foi realizado. US02 | DOING  |
| T4 | Exibir data e horário em que o serviço foi realizado. US02 | DOING |
| T5 | Exibir informações do barbeiro responsável pelo serviço. US02 | DOING |
| T6 | Exibir informações do cliente atendido. US02 | DOING |
| T7 | Testar o estoque e o histórico de serviços. US02/US03 | DOING |

Ao final da Sprint 6, o esperado é que o dono deverá conseguir visualizar o histórico dos serviços realizados, contendo informações sobre o serviço, cliente, barbeiro, data e horário do atendimento.

## SPRINT 7

| TAREFAS | DESCRIÇÃO | CHECK |
| --- | --- | --- |
| T1 | Controle de acesso do dono, cliente e administrador. US01/US05 | TO DO |
| T2 | Impedir que clientes acessem funções do dono ou administrador. US01 | TO DO |
| T3 | Exigir login para acessar as funcionalidades do sistema. US01/US05 | TO DO |
| T4 | Garantir que o cliente só possa cancelar seus próprios agendamentos. US07 | TO DO |
| T5 | Implementar armazenamento de dados (usuários, serviços, produtos, agendamentos). RNF03 | TO DO |
| T6 | Permitir recuperação dos dados e backup. RNF03 | TO DO |
| T7 | Verificar se as informações exibidas na agenda estão atualizadas para os clientes. RNF02 | TO DO |
| T8 | Realizar testes gerais do sistema. Todos | TO DO |
| T9 | Corrigir erros encontrados durante os testes. Todos | TO DO |
| T10 | Testar cadastro, login, agenda, agendamento, cancelamento, serviços e histórico. Todos | TO DO |
| T11 | Realizar teste final de segurança e acesso. RNF01 | TO DO |

Ao final da Sprint 7, o esperado é que o sistema esteja pronto para ser utilizado via terminal, com os dados testados e armazenados em arquivos `.dat`.
