-- Tabela VENDA
CREATE TABLE venda (
    id_venda                    NUMBER,
    data_hora                   DATE,
    quantidade                  NUMBER,
    metodo_pagamento            VARCHAR2(512),
    id_produto          NUMBER NOT NULL,
    id_maquina          NUMBER NOT NULL,
    id_compartimento NUMBER NOT NULL,
    CONSTRAINT venda_pk PRIMARY KEY (id_venda)
);

-- Tabela COMPARTIMENTO
CREATE TABLE compartimento (
    id_compartimento            NUMBER,
    codigo_compartimento        VARCHAR2(512),
    preco                       FLOAT(8),
    capacidade_max              NUMBER,
    quantidade_atual            NUMBER,
    estado                      VARCHAR2(512),
    id_produto          NUMBER NOT NULL,
    id_maquina          NUMBER NOT NULL,
    CONSTRAINT compartimento_pk PRIMARY KEY (id_compartimento)
);

-- Tabela EMPRESA
CREATE TABLE empresa (
    id_empresa                  NUMBER,
    nome                        VARCHAR2(512),
    contacto                    VARCHAR2(512),
    CONSTRAINT empresa_pk PRIMARY KEY (id_empresa)
);

-- Tabela REABASTECIMENTO
CREATE TABLE reabastecimento (
    id_reabastecimento          NUMBER,
    data_hora                   DATE,
    quantidade_abastecimento    NUMBER,
    viagem_id_viagem            NUMBER NOT NULL,
    id_funcionario  NUMBER NOT NULL,
    id_produto          NUMBER NOT NULL,
    id_compartimento NUMBER NOT NULL,
    CONSTRAINT reabastecimento_pk PRIMARY KEY (id_reabastecimento)
);

-- Tabela PRODUTO
CREATE TABLE produto (
    id_produto                  NUMBER,
    nome                        VARCHAR2(512),
    preco                       NUMBER,
    categoria                   VARCHAR2(512),
    quantidade_total            NUMBER,
    CONSTRAINT produto_pk PRIMARY KEY (id_produto)
);

-- Tabela FUNCIONARIO
CREATE TABLE funcionario (
    id_funcionario              NUMBER,
    nome                        VARCHAR2(512),
    cargo                       VARCHAR2(512),
    contacto_func               VARCHAR2(512),
    CONSTRAINT funcionario_pk PRIMARY KEY (id_funcionario)
);

-- Tabela MAQUINA
CREATE TABLE maquina (
    id_maquina                  NUMBER,
    localizacao                 VARCHAR2(512),
    estado                      VARCHAR2(512),
    coordenadas                 VARCHAR2(512),
    ultima_atualizacao          DATE,
    ultima_venda                DATE,
    id_empresa          NUMBER NOT NULL,
    CONSTRAINT maquina_pk PRIMARY KEY (id_maquina)
);

-- Tabela ARMAZEM
CREATE TABLE armazem (
    id_armazem                  NUMBER,
    localizacao                 VARCHAR2(512),
    capacidade_total            NUMBER,
    coordenadas                 VARCHAR2(512),
    CONSTRAINT armazem_pk PRIMARY KEY (id_armazem)
);

-- Tabela VIAGEM
CREATE TABLE viagem (
    id_viagem                   NUMBER,
    rota_id                     NUMBER,
    data_inicio                 DATE,
    data_fim                    DATE,
    distancia_total             NUMBER,
    id_funcionario  NUMBER NOT NULL,
    veiculo_id_veiculo          NUMBER NOT NULL,
    CONSTRAINT viagem_pk PRIMARY KEY (id_viagem)
);

-- Tabela VEICULO
CREATE TABLE veiculo (
    id_veiculo                  NUMBER,
    capacidade_maxima           NUMBER,
    autonomia_km                NUMBER,
    estado                      VARCHAR2(512),
    CONSTRAINT veiculo_pk PRIMARY KEY (id_veiculo)
);

-- Tabela MANUTENCAO
CREATE TABLE manutencao (
    id_manutencao               NUMBER,
    data_hora                   DATE,
    estado                      VARCHAR2(512),
    id_maquina          NUMBER NOT NULL,
    id_funcionario  NUMBER NOT NULL,
    CONSTRAINT manutencao_pk PRIMARY KEY (id_manutencao)
);

-- Tabela STOCK_ARMAZEM
CREATE TABLE stock_armazem (
    id_stock_armazem            NUMBER,
    quantidade_disponivel       NUMBER,
    data_registo                DATE,
    id_armazem          NUMBER NOT NULL,
    id_produto          NUMBER NOT NULL,
    CONSTRAINT stock_armazem_pk PRIMARY KEY (id_stock_armazem)
);

-- Constraints de integridade referencial

ALTER TABLE venda ADD CONSTRAINT venda_fk1 FOREIGN KEY (id_produto) REFERENCES produto(id_produto);
ALTER TABLE venda ADD CONSTRAINT venda_fk2 FOREIGN KEY (id_maquina) REFERENCES maquina(id_maquina);
ALTER TABLE venda ADD CONSTRAINT venda_fk3 FOREIGN KEY (id_compartimento) REFERENCES compartimento(id_compartimento);

ALTER TABLE compartimento ADD CONSTRAINT compartimento_fk1 FOREIGN KEY (id_produto) REFERENCES produto(id_produto);
ALTER TABLE compartimento ADD CONSTRAINT compartimento_fk2 FOREIGN KEY (id_maquina) REFERENCES maquina(id_maquina);

ALTER TABLE reabastecimento ADD CONSTRAINT reabastecimento_fk1 FOREIGN KEY (viagem_id_viagem) REFERENCES viagem(id_viagem);
ALTER TABLE reabastecimento ADD CONSTRAINT reabastecimento_fk2 FOREIGN KEY (id_funcionario) REFERENCES funcionario(id_funcionario);
ALTER TABLE reabastecimento ADD CONSTRAINT reabastecimento_fk3 FOREIGN KEY (id_produto) REFERENCES produto(id_produto);
ALTER TABLE reabastecimento ADD CONSTRAINT reabastecimento_fk4 FOREIGN KEY (id_compartimento) REFERENCES compartimento(id_compartimento);

ALTER TABLE maquina ADD CONSTRAINT maquina_fk1 FOREIGN KEY (id_empresa) REFERENCES empresa(id_empresa);

ALTER TABLE viagem ADD CONSTRAINT viagem_fk1 FOREIGN KEY (id_funcionario) REFERENCES funcionario(id_funcionario);
ALTER TABLE viagem ADD CONSTRAINT viagem_fk2 FOREIGN KEY (veiculo_id_veiculo) REFERENCES veiculo(id_veiculo);

ALTER TABLE manutencao ADD CONSTRAINT manutencao_fk1 FOREIGN KEY (id_maquina) REFERENCES maquina(id_maquina);
ALTER TABLE manutencao ADD CONSTRAINT manutencao_fk2 FOREIGN KEY (id_funcionario) REFERENCES funcionario(id_funcionario);

ALTER TABLE stock_armazem ADD CONSTRAINT stock_armazem_fk1 FOREIGN KEY (id_armazem) REFERENCES armazem(id_armazem);
ALTER TABLE stock_armazem ADD CONSTRAINT stock_armazem_fk2 FOREIGN KEY (id_produto) REFERENCES produto(id_produto);
