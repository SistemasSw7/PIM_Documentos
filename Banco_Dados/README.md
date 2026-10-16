# DICIONÁRIO DE DADOS

## TABELA `Usuarios`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| Usuarios | tipo_usuario | VARCHAR | A definir | PK, FK, NOT NULL* | N/D | Identifica o tipo ou perfil do usuário no sistema. |
| Usuarios | id_usuario | INT | 4 bytes | PK, FK, NOT NULL* | N/D | Identificador do usuário no sistema. |
| Usuarios | login | VARCHAR | A definir | UNIQUE, NOT NULL* | N/D | Nome utilizado para acessar o sistema. |
| Usuarios | senha | VARCHAR | A definir | NOT NULL* | N/D | Armazena o hash da senha utilizado para autenticação. |

## TABELA `clientes`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| clientes | id_usuario | INT | 4 bytes | PK, FK, NOT NULL* | N/D | Identificador do usuário associado ao cliente. |
| clientes | tipo_usuario | VARCHAR | A definir | PK, FK, NOT NULL* | N/D | Tipo do usuário associado ao cliente. |
| clientes | nome_clientes | CHAR | A definir | — | N/D | Nome do cliente. |
| clientes | data_nascimento | DATE | 3 bytes | — | N/D | Data de nascimento do cliente. |
| clientes | fk_Usuarios_id_usuario | INT* | A confirmar | FK? | N/D | Campo de relacionamento que precisa ser confirmado no modelo físico. |

## TABELA `adm`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| adm | id_usuario | INT | 4 bytes | PK, FK, NOT NULL* | N/D | Identificador do usuário administrador. |
| adm | tipo_usuario | VARCHAR | A definir | PK, FK, NOT NULL* | N/D | Tipo do usuário associado ao administrador. |
| adm | nome_adm | CHAR | A definir | — | N/D | Nome do administrador. |
| adm | data_nascimento | DATE | 3 bytes | — | N/D | Data de nascimento do administrador. |

## TABELA `barbeiro`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| barbeiro | id_usuario | INT | 4 bytes | PK, FK, NOT NULL* | N/D | Identificador do usuário barbeiro. |
| barbeiro | tipo_usuario | VARCHAR | A definir | PK, FK, NOT NULL* | N/D | Tipo do usuário associado ao barbeiro. |
| barbeiro | nome | CHAR | A definir | — | N/D | Nome do barbeiro. |
| barbeiro | data_nascimento | DATE | 3 bytes | — | N/D | Data de nascimento do barbeiro. |

## TABELA `contatos`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| contatos | id_usuario | INT | 4 bytes | PK, FK, NOT NULL* | N/D | Identifica o usuário ao qual o contato pertence. |
| contatos | telefone | VARCHAR | 20 caracteres | — | N/D | Número de telefone associado ao usuário. |
| contatos | email | VARCHAR | A definir | UNIQUE* | N/D | Endereço de e-mail associado ao usuário. |

## TABELA `agendamentos`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| agendamentos | id_agendamentos | INT | 4 bytes | PK, NOT NULL | N/D | Identificador único do agendamento. |
| agendamentos | status | VARCHAR | A definir | NOT NULL* | N/D | Situação atual do agendamento, como pendente, confirmado, concluído ou cancelado. |
| agendamentos | data | DATE | 3 bytes | NOT NULL* | N/D | Data prevista para a realização do agendamento. |
| agendamentos | horario | TIME | Depende da precisão | NOT NULL* | N/D | Horário previsto para o início do atendimento. |
| agendamentos | id_serviço | INT | 4 bytes | FK, NOT NULL* | N/D | Identifica o serviço associado ao agendamento. |
| agendamentos | id_cliente | INT* | A confirmar | FK? | N/D | Identifica o cliente associado ao agendamento. |

## TABELA `serviço`

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
|---|---|---|---|---|---|---|
| serviço | id_serviço | INT | 4 bytes | PK, NOT NULL | N/D | Identificador único do serviço. |
| serviço | valor | DECIMAL | 10,2 | NOT NULL* | N/D | Valor cobrado pela realização do serviço. |
| serviço | duração | TIME | Depende da precisão | NOT NULL* | N/D | Duração prevista para a realização do serviço. |
| serviço | nome_serviço | VARCHAR | A definir | NOT NULL* | N/D | Nome do serviço oferecido pela barbearia. |
