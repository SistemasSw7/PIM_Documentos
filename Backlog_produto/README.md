# Backlog Do Produto.

Este diretório serve como forma de documentar detalhadamente sobre o Backlog do produto do sistema de agendamentos da Barbearia Santos.

## User Stories

| ID| USER STORIES |
| --- | --- |
| **US01** | Como dono eu quero poder gerenciar minha agenda com facilidade sem ser por whatsapp. |
| **US02** | Como dono eu quero poder visualizar o histórico de serviços já realizados. |
| **US03** | Como dono eu quero poder cadastrar no sistema novos serviços junto com informações como seus valores. |
| **US04** | Como cliente eu quero poder me cadastrar para poder manter meus dados salvos. |
| **US05** | Como cliente eu quero poder visualizar as datas e horários disponíveis para agendamento para eu poder me programar. |
| **US06** | Como cliente eu quero poder marcar para cortar meu cabelo com praticidade assim como desmarcar também. |
| **US07** | Como cliente eu quero poder visualizar meu histórico de agendamentos, como serviços que já foram realizados, assim como os que vão acontecer ainda. |

## Business Rules

| ID | BUSINESS RULES |
| --- | --- |
| **BR01** | Um agendamento só pode ser cancelado com 2 horas de antecedência. |
| **BR02** | Apenas o Barbeiro pode cancelar um agendamento fora das 2 horas de antecedência determinadas. |
| **BR03** | Apenas o Barbeiro pode alterar, editar ou excluir serviços, horários ou datas da agenda. |
| **BR04** | Um cancelamento só pode ser realizado em um agendamento que ainda não foi concluído. |

## Requisitos Funcionais

| RF01 | GERENCIAR USUÁRIOS |
| --- | ---|
| **T1** | criar Cadastro de usuário (pedir login, senha,nome, telefone, email e data de nascimento). |
|	**T2** | editar usuário. |
| **T3** | Excluir/Arquivar usuário. |
| **T4** | Autenticar-se no sistema. |
|	**T5** | Hierarquia de usuário. |

| RF02 | GERENCIAR AGENDA|
| --- | ---|
| **T1** | criar horários e datas na agenda. |
|	**T2** | visualizar horários e datas disponíveis. --------------------------------------------------|
| **T3** | editar horários. |
| **T4** |  agendar horários. |
|	**T5** | desmarcar horário. |
| **T6** | mostrar serviços disponíveis e informações. |

| RF03 | VISUALIZAR HISTÓRICO DE SERVIÇOS REALIZADOS/AGENDADOS |
| --- | --- |
|	**T1** | mostrar informações sobre quais serviços foram realizados. |
|	**T2 ** |mostrar horário em que foi realizado. |
|	**T3** | mostrar informações de quem realizou o serviço.---------------------------------------------|
| **T4** |mostrar informações sobre o cliente. |
| **T5** |mostrar status do agendamento (não realizado ou realizado) |
| **T6** |histórico geral de serviços (Adm, Barbeiro). |
| **T7** | histórico pessoal (Cliente). |

## Requisitos Não Funcionais

| RNF01| SEGURANÇA |
| --- | ---|
| **T1** |  O sistema deve possuir uma hierarquia de usuário (Adm, Barbeiro e cliente). ---------------- ------------|
| **T2** |  O sistema deve exigir login para realizar as ações dentro dele. |
| **T3** | Caso possua BD criptografar os dados para maior segurança. |


| RNF02 | USABILIDADE |
| --- | --- |
| **T1** | O sistema deve ter uma interface com informações claras e diretas. |
| **T2** |  O sistema deve indicar em que parte ele está (login,cadastro, etc). |
| **T3** | As atualizações da agenda feita pelo adm ou barbeiro devem ser atualizadas para aparecer para os clientes.--|

| RNF03 | ARMAZENAGEM DE DADOS |
| --- |--- |
| **T1** | O sistema deve armazenar os dados dos usuários, serviços e agendamentos em arquivos binários.---------------|
| **T2** | Os dados devem permanecer armazenados mesmo após o encerramento do sistema. |
| **T3** |  O sistema deve permitir a recuperação dos dados armazenados. |



