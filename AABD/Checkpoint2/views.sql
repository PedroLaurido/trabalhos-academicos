--------------------------------------------------------
--  File created - Segunda-feira-Abril-07-2025   
--------------------------------------------------------
--------------------------------------------------------
--  DDL for View ARMAZEM_MAIS_VISITADO
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."ARMAZEM_MAIS_VISITADO" ("ID_ARMAZEM") AS 
  SELECT id_armazem
FROM (SELECT id_armazem FROM viagem GROUP BY id_armazem ORDER BY COUNT(*) DESC)
WHERE ROWNUM = 1;
--------------------------------------------------------
--  DDL for View VIEW_A
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_A" ("ID_MAQUINA", "DATA_HORA_ABAST", "LOCAL", "QUANT_ABASTECIDA", "NUM_PRODUTOS_DIFERENTES") AS 
  SELECT 
    m.id_maquina,
    r.data_hora AS data_hora_abast,
    m.cidade AS local,
    SUM(r.qtd_abastecimento) AS quant_abastecida,
    COUNT(DISTINCT p.id_produto) AS num_produtos_diferentes
FROM 
    reabastecimento r
JOIN compartimento c ON r.id_reab = c.id_reab
JOIN produto p ON c.id_produto = p.id_produto
JOIN maquina_comp mc ON c.id_comp = mc.id_comp
JOIN maquina m ON mc.id_maquina = m.id_maquina
JOIN estado_maquina em ON m.id_estado = em.id_estado
WHERE 
    p.tipo_produto = 'Snacks'
    AND m.cidade = 'Coimbra'
    AND TRUNC(r.data_hora) = TRUNC(SYSDATE - 1)
    AND em.estado <> 'inativo'
    -- No snack stock in this machine:
    AND NOT EXISTS (
        SELECT 1
        FROM compartimento c2
        JOIN produto p2 ON c2.id_produto = p2.id_produto
        JOIN maquina_comp mc2 ON c2.id_comp = mc2.id_comp
        WHERE 
            p2.tipo_produto = 'Snacks'
            AND c2.qtd_atual > 0
            AND mc2.id_maquina = m.id_maquina
    )
GROUP BY 
    m.id_maquina, r.data_hora, m.cidade
ORDER BY 
    quant_abastecida DESC;
--------------------------------------------------------
--  DDL for View VIEW_B
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_B" ("IDMAQUINA", "LOCAL", "REF_PRODUTO", "PRODUTO", "QUANT_EXISTENTE", "QUANT_ABASTECIDA", "CAPACIDADE") AS 
  SELECT 
    m.id_maquina AS IDMAQUINA,  -- ID da máquina visitada
    m.latitude || ', ' || m.longitude AS LOCAL,  -- Localização da máquina (latitude e longitude)
    p.id_produto AS REF_PRODUTO,  -- Referência do produto
    p.nome AS PRODUTO,  -- Nome do produto
    c.qtd_atual AS QUANT_EXISTENTE,  -- Quantidade existente no compartimento
    r.qtd_abastecimento AS QUANT_ABASTECIDA,  -- Quantidade abastecida durante a visita
    c.qtd_max AS CAPACIDADE  -- Capacidade máxima do compartimento
FROM viagem v
JOIN visita_maquina vm ON v.id_visita = vm.id_visita  -- Relaciona viagem com visitas às máquinas
JOIN maquina m ON vm.id_visita = m.id_visita  -- Relaciona visita com máquina
JOIN maquina_comp mc ON m.id_maquina = mc.id_maquina  -- Relaciona máquina com compartimentos
JOIN compartimento c ON mc.id_comp = c.id_comp  -- Relaciona compartimento com produto
JOIN produto p ON c.id_produto = p.id_produto  -- Obtém informações do produto
JOIN reabastecimento r ON c.id_reab = r.id_reab  -- Obtém informações sobre o reabastecimento
WHERE v.id_viagem = 2025031105  -- Filtra apenas a viagem com código 2025031105
ORDER BY vm.data_hora ASC, r.qtd_abastecimento DESC;
--------------------------------------------------------
--  DDL for View VIEW_C
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_C" ("ID_MAQUINA", "LOCAL", "REF_PRODUTO", "PRODUTO", "QUANT_VENDIDA_MES", "QUANT_VEND_DESDE_ULTIMO") AS 
  SELECT
    M.ID_MAQUINA,
    TO_CHAR(M.LATITUDE) || ' / ' || TO_CHAR(M.LONGITUDE) AS LOCAL, 
    P.ID_PRODUTO AS REF_PRODUTO,
    P.NOME AS PRODUTO,

    -- TOTAL DE PRODUTOS VENDIDOS NO MÊS ANTERIOR
    SUM(CASE 
    WHEN V.DATA_HORA BETWEEN 
        TRUNC(ADD_MONTHS(SYSDATE, -1), 'MM') -- Dá o 1º dia do mês anterior
        AND LAST_DAY(ADD_MONTHS(SYSDATE, -1)) -- Dá o último dia do mês anterior
    THEN V.QUANTIDADE 
    ELSE 0 
    END) AS QUANT_VENDIDA_MES,

    -- TOTAL DESDE O ÚLTIMO REABASTECIMENTO
    SUM(CASE 
        WHEN V.DATA_HORA > R.DATA_HORA 
        THEN V.QUANTIDADE 
        ELSE 0 
    END) AS QUANT_VEND_DESDE_ULTIMO

FROM MAQUINA M
JOIN VENDA V ON V.ID_MAQUINA = M.ID_MAQUINA -- LIGA AS VENDAS À MÁQUINA
JOIN COMP_VENDA CV ON CV.ID_VENDA = V.ID_VENDA -- LIGA A VENDA A UM COMPARTIMENTO
JOIN COMPARTIMENTO C ON C.ID_COMP = CV.ID_COMP -- OBTÉM O COMPARTIMENTO E O PRODUTO ASSOCIADO
JOIN PRODUTO P ON P.ID_PRODUTO = C.ID_PRODUTO
LEFT JOIN REABASTECIMENTO R ON R.ID_REAB = C.ID_REAB -- LIGA AO REABASTECIMENTO PARA SABER A DATA DO ÚLTIMO
JOIN MAQUINA_COMP MC ON MC.ID_COMP = C.ID_COMP AND MC.ID_MAQUINA = M.ID_MAQUINA -- LIGA COM MÁQUINA_COMP CASO PRECISE DA QTD MÁXIMA POR PRODUTO POR MÁQUINA
WHERE C.QTD_ATUAL <= (C.QTD_MAX * 0.5)-- FILTRA PRODUTOS QUE NÃO EXCEDEM 50% DA CAPACIDADE
GROUP BY M.ID_MAQUINA, TO_CHAR(M.LATITUDE) || ' / ' || TO_CHAR(M.LONGITUDE),P.ID_PRODUTO, P.NOME
ORDER BY QUANT_VENDIDA_MES DESC;
--------------------------------------------------------
--  DDL for View VIEW_E
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_E" ("IDMAQUINA", "PRODUTO", "MEDIAMENSAL") AS 
  SELECT v.id_maquina AS IDMAQUINA, p.nome AS PRODUTO, ROUND(AVG(v.quantidade),2) AS MEDIAMENSAL
FROM venda v
JOIN comp_venda cv ON v.id_venda = cv.id_venda
JOIN compartimento c ON cv.id_comp = c.id_comp
JOIN produto p ON c.id_produto = p.id_produto
JOIN maquina_estado me ON v.id_maquina = me.id_maquina
JOIN reabastecimento r ON c.id_reab = r.id_reab
WHERE UPPER(me.estado) = 'OPERACIONAL' -- Só máquinas com estado 'operacional'
AND EXTRACT(YEAR FROM v.data_hora) IN (2023, 2024) -- Só 2023 e 2024
AND r.qtd_abastecimento > (SELECT AVG(qtd_abastecimento) FROM reabastecimento) -- Só reabastecimentos acima da média
GROUP BY v.id_maquina, p.nome
ORDER BY MEDIAMENSAL DESC, p.nome;
--------------------------------------------------------
--  DDL for View VIEW_F
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_F" ("ID_MAQUINA", "REFPRODUTO", "QUANT_VENDIDA", "QUANT_REABASTECIDA", "PERCENTAGEM") AS 
  WITH MAQ_TOP AS (
    SELECT ID_MAQUINA
    FROM (
        SELECT M2.ID_MAQUINA,
               SUM(V2.QUANTIDADE) AS TOTAL_VENDIDA
        FROM MAQUINA M2
        JOIN VENDA V2 ON V2.ID_MAQUINA = M2.ID_MAQUINA -- LIGA AS VENDAS À MÁQUINA
        JOIN COMP_VENDA CV2 ON CV2.ID_VENDA = V2.ID_VENDA -- LIGA A VENDA AO COMPARTIMENTO
        JOIN COMPARTIMENTO C2 ON C2.ID_COMP = CV2.ID_COMP -- LIGA AOS COMPARTIMENTOS
        JOIN PRODUTO P2 ON P2.ID_PRODUTO = C2.ID_PRODUTO -- LIGA AOS PRODUTOS PARA VERIFICAR O TIPO
        WHERE P2.TIPO_PRODUTO = 'AGUA' -- FILTRA SOMENTE PRODUTOS DO TIPO 'AGUA'
          AND V2.DATA_HORA >= SYSDATE - 3 -- CONSIDERA APENAS VENDAS DOS ÚLTIMOS 3 DIAS (72H)
        GROUP BY M2.ID_MAQUINA
        ORDER BY TOTAL_VENDIDA DESC -- ORDENA PELA MAIOR QUANTIDADE VENDIDA
    )
    WHERE ROWNUM = 1 -- ESCOLHE APENAS A PRIMEIRA MÁQUINA (QUE MAIS VENDEU ÁGUA)
),

DADOS AS (
    SELECT
        M.ID_MAQUINA,
        P.ID_PRODUTO,

        -- QUANTIDADE DESTE PRODUTO VENDIDA EM FEVEREIRO
        SUM(CASE 
            WHEN TO_CHAR(V.DATA_HORA, 'MM') = '02' THEN V.QUANTIDADE
            ELSE 0 
        END) AS QUANT_VENDIDA,

        -- QUANTIDADE DESTE PRODUTO REABASTECIDA EM FEVEREIRO
        SUM(CASE 
            WHEN TO_CHAR(R.DATA_HORA, 'MM') = '02' THEN R.QTD_ABASTECIMENTO
            ELSE 0 
        END) AS QUANT_REABASTECIDA

    FROM MAQUINA M
    JOIN VENDA V ON V.ID_MAQUINA = M.ID_MAQUINA -- LIGA AS VENDAS À MÁQUINA
    JOIN COMP_VENDA CV ON CV.ID_VENDA = V.ID_VENDA -- LIGA A VENDA AO COMPARTIMENTO
    JOIN COMPARTIMENTO C ON C.ID_COMP = CV.ID_COMP -- LIGA AOS COMPARTIMENTOS
    JOIN PRODUTO P ON P.ID_PRODUTO = C.ID_PRODUTO -- LIGA AOS PRODUTOS
    LEFT JOIN REABASTECIMENTO R ON R.ID_REAB = C.ID_REAB -- LIGA AOS REABASTECIMENTOS

    WHERE P.TIPO_PRODUTO = 'AGUA' -- CONSIDERA APENAS PRODUTOS DO TIPO 'AGUA'
      AND M.ID_MAQUINA = (SELECT ID_MAQUINA FROM MAQ_TOP) -- FILTRA PELA MÁQUINA QUE MAIS VENDEU

    GROUP BY M.ID_MAQUINA, P.ID_PRODUTO
),

TOTAL AS (
    SELECT
        ID_MAQUINA,
        SUM(QUANT_VENDIDA) AS TOTAL_VENDIDA -- TOTAL DE VENDAS DE TODOS OS PRODUTOS EM FEVEREIRO
    FROM DADOS
    GROUP BY ID_MAQUINA
)

SELECT
    D.ID_MAQUINA,
    D.ID_PRODUTO AS REFPRODUTO,
    D.QUANT_VENDIDA,
    D.QUANT_REABASTECIDA,

    -- PERCENTAGEM DE VENDAS DO PRODUTO FACE AO TOTAL DA MÁQUINA
    ROUND(D.QUANT_VENDIDA * 100 / NULLIF(T.TOTAL_VENDIDA, 0), 0) AS PERCENTAGEM

FROM DADOS D
JOIN TOTAL T ON T.ID_MAQUINA = D.ID_MAQUINA;
--------------------------------------------------------
--  DDL for View VIEW_G
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_G" ("VIAGEM", "TIPO_PRODUTO", "QUANT_ABASTECIDA", "NUM_MAQ_ABASTECIDAS") AS 
  SELECT
    V.ID_VIAGEM AS VIAGEM,                                -- ID DA VIAGEM
    P.TIPO_PRODUTO,                                       -- TIPO DO PRODUTO

    SUM(R.QTD_ABASTECIMENTO) AS QUANT_ABASTECIDA,         -- QUANTIDADE TOTAL ABASTECIDA DESTE PRODUTO

    COUNT(DISTINCT M.ID_MAQUINA) AS NUM_MAQ_ABASTECIDAS   -- NÚMERO TOTAL DE MÁQUINAS ABASTECIDAS

FROM REABASTECIMENTO R

JOIN COMPARTIMENTO C ON C.ID_REAB = R.ID_REAB             -- LIGAÇÃO AO COMPARTIMENTO
JOIN PRODUTO P ON P.ID_PRODUTO = C.ID_PRODUTO             -- LIGAÇÃO AO PRODUTO

JOIN MAQUINA_COMP MC ON MC.ID_COMP = C.ID_COMP            -- LIGAÇÃO COMPARTIMENTO ? MÁQUINA_COMP
JOIN MAQUINA M ON M.ID_MAQUINA = MC.ID_MAQUINA            -- LIGAÇÃO FINAL À MÁQUINA

JOIN VIAGEM V ON V.ID_REAB = R.ID_REAB                    -- LIGAÇÃO DA VIAGEM PARA OBTER O ID

WHERE EXTRACT(YEAR FROM R.DATA_HORA) = EXTRACT(YEAR FROM SYSDATE) - 1  -- SÓ REABASTECIMENTOS DO ANO PASSADO
  AND M.CIDADE LIKE '%COIMBRA%'                           -- APENAS MÁQUINAS LOCALIZADAS EM COIMBRA

GROUP BY V.ID_VIAGEM, P.TIPO_PRODUTO                      -- AGRUPA POR VIAGEM E PRODUTO

HAVING COUNT(DISTINCT M.ID_MAQUINA) > 3                   -- CONSIDERA SÓ PRODUTOS COM MAIS DE 3 MÁQUINAS ABASTECIDAS

ORDER BY QUANT_ABASTECIDA DESC;
--------------------------------------------------------
--  DDL for View VIEW_I
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_I" ("ARMAZEM", "MAQUINA", "N_VISITAS", "QUANT_TOTAL", "QUANT_MEDIA_VISITA", "N_PROD_DIF") AS 
  SELECT "ARMAZEM","MAQUINA","N_VISITAS","QUANT_TOTAL","QUANT_MEDIA_VISITA","N_PROD_DIF"
FROM (
    SELECT
        a.nome AS ARMAZEM,
        vm25.id_maquina AS MAQUINA,
        vm25.num_visitas AS N_VISITAS,
        vm25.total_abastecido AS QUANT_TOTAL,
        vm25.media_abastecido AS QUANT_MEDIA_VISITA,
        ROUND(AVG(num_produtos_reabastecidos(c.id_reab)), 2) AS N_PROD_DIF
    FROM visitas_maquina_2025 vm25
    JOIN viagem v ON vm25.id_maquina = v.id_visita
    JOIN armazem a ON v.id_armazem = (SELECT id_armazem FROM armazem_mais_visitado)
    JOIN maquina_comp mc ON vm25.id_maquina = mc.id_maquina
    JOIN compartimento c ON mc.id_comp = c.id_comp
    GROUP BY a.nome, vm25.id_maquina, vm25.num_visitas, vm25.total_abastecido, vm25.media_abastecido
    ORDER BY vm25.num_visitas DESC
)
WHERE ROWNUM <= 3;
--------------------------------------------------------
--  DDL for View VIEW_J_2021129697
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_J_2021129697" ("CIDADE", "TIPO_PRODUTO", "ID_MAQUINA", "TOTAL_VENDIDO") AS 
  SELECT
    M.CIDADE,                                            -- LOCALIZAÇÃO DA MÁQUINA
    P.TIPO_PRODUTO,                                     -- TIPO DO PRODUTO VENDIDO
    M.ID_MAQUINA,                                       -- MÁQUINA ONDE FOI FEITA A VENDA
    SUM(V.QUANTIDADE) AS TOTAL_VENDIDO                  -- TOTAL DE PRODUTOS VENDIDOS

FROM VENDA V

JOIN COMP_VENDA CV ON V.ID_VENDA = CV.ID_VENDA          -- LIGAÇÃO À COMP_VENDA
JOIN COMPARTIMENTO C ON C.ID_COMP = CV.ID_COMP          -- LIGAÇÃO AO COMPARTIMENTO
JOIN PRODUTO P ON P.ID_PRODUTO = C.ID_PRODUTO           -- LIGAÇÃO AO PRODUTO
JOIN MAQUINA M ON M.ID_MAQUINA = V.ID_MAQUINA           -- LIGAÇÃO À MÁQUINA PARA OBTER A CIDADE

GROUP BY M.CIDADE, P.TIPO_PRODUTO, M.ID_MAQUINA         -- AGRUPA POR LOCALIZAÇÃO, PRODUTO E MÁQUINA

ORDER BY TOTAL_VENDIDO DESC;
--------------------------------------------------------
--  DDL for View VIEW_J_2021130905
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_J_2021130905" ("ID_MAQUINA", "ID_PRODUTO", "MES", "QUANTIDADE", "FATURAMENTO") AS 
  SELECT 
    v.id_maquina,
    p.id_produto,
    TO_CHAR(v.data_hora, 'YYYY-MM') AS mes,
    SUM(v.quantidade) AS quantidade,
    SUM(v.quantidade * c.preco) AS faturamento
FROM venda v
JOIN comp_venda cv ON v.id_venda = cv.id_venda
JOIN compartimento c ON cv.id_comp = c.id_comp
JOIN produto p ON c.id_produto = p.id_produto
GROUP BY v.id_maquina, p.id_produto, TO_CHAR(v.data_hora, 'YYYY-MM')
ORDER BY v.id_maquina, p.id_produto, mes;
--------------------------------------------------------
--  DDL for View VIEW_J_2022161633
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_J_2022161633" ("ID_PRODUTO", "NOME_PRODUTO", "TIPO_PRODUTO", "MEDIA_ESTOQUE_MAQUINAS") AS 
  SELECT
    p.id_produto,
    p.nome AS nome_produto,
    p.tipo_produto,
    AVG(c.qtd_atual) AS media_estoque_maquinas
FROM
    produto p
JOIN compartimento c ON c.id_produto = p.id_produto
JOIN maquina_comp mc ON mc.id_comp = c.id_comp
JOIN maquina m ON m.id_maquina = mc.id_maquina
GROUP BY
    p.id_produto, p.nome, p.tipo_produto;
--------------------------------------------------------
--  DDL for View VIEW_K_2021129697
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_K_2021129697" ("ID_MAQUINA", "ULTIMO_REABASTECIMENTO", "DIAS_SEM_REAB", "PRODUTO_MAIS_VENDIDO") AS 
  SELECT
    MQ.ID_MAQUINA,                                                 -- IDENTIFICADOR DA MÁQUINA
    MQ.ULTIMO_REABASTECIMENTO,                                     -- DATA DO ÚLTIMO REABASTECIMENTO DA MÁQUINA
    TRUNC(SYSDATE - MQ.ULTIMO_REABASTECIMENTO) AS DIAS_SEM_REAB,   -- NÚMERO DE DIAS (INTEIRO) DESDE O ÚLTIMO REABASTECIMENTO
    PV.TIPO_PRODUTO AS PRODUTO_MAIS_VENDIDO                        -- PRODUTO MAIS VENDIDO NA MÁQUINA

-- SUBQUERY MQ: CALCULA A DATA DO ÚLTIMO REABASTECIMENTO POR MÁQUINA
FROM (
    SELECT M.ID_MAQUINA, MAX(R.DATA_HORA) AS ULTIMO_REABASTECIMENTO
    FROM MAQUINA M
    JOIN MAQUINA_COMP MC ON M.ID_MAQUINA = MC.ID_MAQUINA           -- LIGA MÁQUINA AOS COMPARTIMENTOS
    JOIN COMPARTIMENTO C ON C.ID_COMP = MC.ID_COMP                 -- OBTÉM O COMPARTIMENTO DA MÁQUINA
    JOIN REABASTECIMENTO R ON R.ID_REAB = C.ID_REAB                -- OBTÉM OS REABASTECIMENTOS FEITOS NESSES COMPARTIMENTOS
    GROUP BY M.ID_MAQUINA                                          -- AGRUPA POR MÁQUINA PARA CALCULAR O MÁXIMO (ÚLTIMO)
) MQ

-- SUBQUERY PV: CALCULA O PRODUTO MAIS VENDIDO POR MÁQUINA (TOP 1)
LEFT JOIN (
    SELECT ID_MAQUINA, TIPO_PRODUTO
    FROM (
        SELECT V.ID_MAQUINA, P.TIPO_PRODUTO, SUM(V.QUANTIDADE) AS TOTAL, -- TOTAL DE VENDAS DO PRODUTO NA MÁQUINA
               ROW_NUMBER() OVER (PARTITION BY V.ID_MAQUINA ORDER BY SUM(V.QUANTIDADE) DESC) AS RN -- RANKING
        FROM VENDA V
        JOIN COMP_VENDA CV ON V.ID_VENDA = CV.ID_VENDA                   -- LIGA AOS ITENS VENDIDOS
        JOIN COMPARTIMENTO C ON C.ID_COMP = CV.ID_COMP                   -- LIGA AOS COMPARTIMENTOS
        JOIN PRODUTO P ON P.ID_PRODUTO = C.ID_PRODUTO                    -- LIGA AO PRODUTO VENDIDO
        GROUP BY V.ID_MAQUINA, P.TIPO_PRODUTO                            -- AGRUPA POR MÁQUINA E PRODUTO
    )
    WHERE RN = 1                                                         -- FILTRA APENAS O PRODUTO MAIS VENDIDO (TOP 1) POR MÁQUINA
) PV ON PV.ID_MAQUINA = MQ.ID_MAQUINA                                    -- LIGA O RESULTADO AO BLOCO DAS MÁQUINAS E REABASTECIMENTOS

-- ORDENA O RESULTADO POR QUEM ESTÁ HÁ MAIS TEMPO SEM REABASTECIMENTO
ORDER BY DIAS_SEM_REAB DESC;
--------------------------------------------------------
--  DDL for View VIEW_K_2021130905
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_K_2021130905" ("ID_PRODUTO", "NOME", "STOCK_ARMAZEM", "STOCK_COMPARTIMENTO", "DIFERENCA_STOCK") AS 
  SELECT 
    p.id_produto,
    p.nome,
    (SELECT SUM(sa.quantidade_disponivel)
     FROM stock_armazem sa
     WHERE sa.id_produto = p.id_produto) AS stock_armazem,
    (SELECT SUM(c.qtd_atual)
     FROM compartimento c
     WHERE c.id_produto = p.id_produto) AS stock_compartimento,
    ((SELECT SUM(sa.quantidade_disponivel)
      FROM stock_armazem sa
      WHERE sa.id_produto = p.id_produto) -
     (SELECT SUM(c.qtd_atual)
      FROM compartimento c
      WHERE c.id_produto = p.id_produto)) AS diferenca_stock
FROM produto p;
--------------------------------------------------------
--  DDL for View VIEW_K_2022161633
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VIEW_K_2022161633" ("ID_MAQUINA", "EMPRESA", "LATITUDE", "LONGITUDE", "AVG_SALES_PER_DAY", "COMPARTIMENTOS_CRITICOS", "TOTAL_FALTANDO") AS 
  WITH machine_popularity AS (
    SELECT
        m.id_maquina,
        ROUND(COUNT(v.id_venda) / GREATEST(1, TRUNC(SYSDATE - MIN(v.data_hora))), 2) AS avg_sales_per_day
    FROM maquina m
    JOIN venda v ON v.id_maquina = m.id_maquina
    GROUP BY m.id_maquina
)
SELECT
    m.id_maquina,
    e.nome AS empresa,
    m.latitude,
    m.longitude,
    mp.avg_sales_per_day,
    COUNT(c.id_comp) AS compartimentos_criticos,
    SUM(c.qtd_min - c.qtd_atual) AS total_faltando
FROM maquina m
JOIN empresa e ON e.id_empresa = m.id_empresa
JOIN maquina_comp mc ON mc.id_maquina = m.id_maquina
JOIN compartimento c ON c.id_comp = mc.id_comp
JOIN machine_popularity mp ON mp.id_maquina = m.id_maquina
WHERE c.qtd_atual <= c.qtd_min
GROUP BY
    m.id_maquina,
    e.nome,
    m.latitude,
    m.longitude,
    mp.avg_sales_per_day
ORDER BY mp.avg_sales_per_day DESC, total_faltando DESC;
--------------------------------------------------------
--  DDL for View VISITAS_MAQUINA_2025
--------------------------------------------------------

  CREATE OR REPLACE FORCE VIEW "AABDG07"."VISITAS_MAQUINA_2025" ("ID_MAQUINA", "NUM_VISITAS", "TOTAL_ABASTECIDO", "MEDIA_ABASTECIDO") AS 
  SELECT vm.id_maquina,COUNT(*) AS num_visitas,SUM(r.qtd_abastecimento) AS total_abastecido,ROUND(AVG(r.qtd_abastecimento), 2) AS media_abastecido
FROM viagem v
JOIN visita_maquina vm ON v.id_visita = vm.id_visita
JOIN compartimento c ON vm.id_maquina = c.id_comp
JOIN reabastecimento r ON c.id_reab = r.id_reab
WHERE v.data_inicio >= TO_DATE('2025-01-01', 'YYYY-MM-DD')
AND v.id_armazem = (SELECT id_armazem FROM armazem_mais_visitado)
GROUP BY vm.id_maquina;
