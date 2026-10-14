CREATE DATABASE barbearia_santos;

go

USE barbearia_santos;

CREATE TABLE usuarios (
	id_usuarios INT identity(1,1) PRIMARY KEY,
	tipo_usuarios VARCHAR(20) NOT NULL,
	nome VARCHAR(150)  NOT NULL,
	email VARCHAR(250) NOT NULL UNIQUE,
	data_nascimento DATE NOT NULL,
	login_ VARCHAR(50) NOT NULL UNIQUE,
	senha VARCHAR(50) NOT NULL,

	CONSTRAINT senha_adequada
		CHECK (
		LEN(senha) > 6
		AND senha LIKE '%[A-Z]%'
		AND senha LIKE '%[0-9]%'
		AND senha LIKE '%[^a-zA-Z0-9]%'
		)
	);

CREATE TABLE telefones (
	id_usuario INT REFERENCES usuarios(id_usuarios) PRIMARY KEY,
	telefones VARCHAR(20) UNIQUE );

CREATE TABLE servicos(
	id_servicos INT IDENTITY(1,1) PRIMARY KEY,
	nome_servicos VARCHAR(50) NOT NULL,
	valor MONEY NOT NULL,
	duracao TIME NOT NULL);

CREATE TABLE agendamentos(
	id_agendamentos INT IDENTITY(1000,1) PRIMARY KEY,
	id_usuario INT REFERENCES usuarios(id_usuarios),
	id_servicos INT REFERENCES servicos(id_servicos),
	data_agendamento DATE NOT NULL,
	hora TIME NOT NULL,
	status_ VARCHAR(20) NOT NULL );

	