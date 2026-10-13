# DICIONÁRIO DE DADOS.

| Tabela | Nome da coluna | Tipo de dados | Comprimento | Restrições | Valor | Descrição |
| --- | --- | --- | --- | --- | --- | --- |
| Usuarios | Id_usuario | INT | 4 bytes | PK, NOT NULL | N/D | Identificador do usuário no sistema |
| Usuarios | Tipo_usuario | VARCHAR | 20 caracteres | NOT NULL | N/D | Identifica o nível hierárquico do usuário no sistema |
| Usuarios | nome | VARCHAR | 100 caracteres | NOT NULL | N/D | Nome do usuário dentro do sistema |
| Usuarios | email | VARCHAR | 254 caracteres | NOT NULL, UNIQUE | N/D | Endereço de e-mail do usuário |
| Usuarios | Data_nascimento | DATE | 3 bytes | NOT NULL | N/D | Data de nascimento do usuário |
| Usuarios | login | VARCHAR | 50 caracteres | NOT NULL, UNIQUE | N/D | Nome utilizado para acessar o sistema |
| Usuarios | senha | VARCHAR | 255 caracteres | NOT NULL | N/D | Armazena a senha que autoriza o usuário a entrar no sistema |
