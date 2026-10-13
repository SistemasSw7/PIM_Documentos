# DICIONÁRIO DE DADOS.

## TABELA USUARIOS

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
| --- | --- | --- | --- | --- | --- | --- |
| Usuarios | Id_usuario | INT | 4 bytes | PK, NOT NULL | N/D | Identificador do usuário no sistema |
| Usuarios | Tipo_usuario | VARCHAR | 20 caracteres | NOT NULL | N/D | Identifica o nível hierárquico do usuário no sistema |
| Usuarios | nome | VARCHAR | 100 caracteres | NOT NULL | N/D | Nome do usuário dentro do sistema |
| Usuarios | email | VARCHAR | 254 caracteres | NOT NULL, UNIQUE | N/D | Endereço de e-mail do usuário |
| Usuarios | Data_nascimento | DATE | 3 bytes | NOT NULL | N/D | Data de nascimento do usuário |
| Usuarios | login | VARCHAR | 50 caracteres | NOT NULL, UNIQUE | N/D | Nome utilizado para acessar o sistema |
| Usuarios | senha | VARCHAR | 255 caracteres | NOT NULL | N/D | Armazena a senha que autoriza o usuário a entrar no sistema |

## TABELA TELEFONES

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
| --- | --- | --- | --- | --- | --- | --- |
| Telefones | Id_usuario | INT | 4 bytes | PK, FK, NOT NULL | N/D | Identifica o usuário ao qual o telefone pertence |
| Telefones | Telefone | VARCHAR | 20 caracteres | NOT NULL | N/D | Número de telefone associado ao usuário |

## TABELA AGENDAMENTOS

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
| --- | --- | --- | --- | --- | --- | --- |
| Agendamentos | id_agendamento | INT | 4 bytes | PK, NOT NULL | N/D | Identificador único do agendamento |
| Agendamentos | Id_usuario | INT | 4 bytes | FK, NOT NULL | N/D | Identifica o usuário associado ao agendamento |
| Agendamentos | id_servico | INT | 4 bytes | FK, NOT NULL | N/D | Identifica o serviço associado ao agendamento |
| Agendamentos | data | DATE | 3 bytes | NOT NULL | N/D | Data prevista para a realização do agendamento |
| Agendamentos | hora | INT | 4 bytes | NOT NULL | N/D | Horário previsto para a realização do agendamento |
| Agendamentos | status | VARCHAR | 20 caracteres | NOT NULL | N/D | Indica a situação atual do agendamento |

## TABELA SERVIÇOS

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
| --- | --- | --- | --- | --- | --- | --- |
| serviços | id_serviços | INT | 4 bytes | PK, NOT NULL | N/D | Identificador único do serviço |
| serviços | valor | FLOAT | 8 bytes | NOT NULL | N/D | Valor cobrado pela realização do serviço |
| serviços | duracao | TIME | Tipo TIME | NOT NULL | N/D | Duração prevista para a realização do serviço |
| serviços | nome_serviço | VARCHAR | 100 caracteres | NOT NULL | N/D | Nome do serviço oferecido pela barbearia |
