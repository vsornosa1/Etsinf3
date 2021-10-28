-- 7.4 CONSULTAS CON CUANTIFICACION UNIVERSAL

-- 20. Nombre de amigos que han leído todas las obras del autor de identificador ‘RUKI’.
SELECT distinct a.nombre
FROM amigo a, leer l, escribir e
WHERE EXISTS 
    (SELECT *
    FROM escribir e1
    WHERE e1.autor_id = 'RUKI')
    AND NOT EXISTS
        (SELECT *
        FROM escribir e
        WHERE e.autor_id = 'RUKI'
            AND NOT EXISTS
                (SELECT * 
                FROM leer l
                WHERE l.num = a.num
                    AND l.cod_ob = e.cod_ob)
        )
;


-- 21. Resolver de nuevo la consulta anterior pero para el autor de identificador ‘GUAP’.
SELECT distinct a.nombre
FROM amigo a, leer l, escribir e
WHERE EXISTS 
    (SELECT *
    FROM escribir e1
    WHERE e1.autor_id = 'GUAP')
    AND NOT EXISTS
    (SELECT *
    FROM escribir e
    WHERE e.autor_id = 'GUAP'
        AND NOT EXISTS
            (SELECT * 
            FROM leer l
            WHERE l.num = a.num
                AND l.cod_ob = e.cod_ob)
    )
;


-- 24. Nombre de amigos que sólo han leído obras del autor ‘CAMA’.
SELECT a.nombre
FROM amigo a
WHERE NOT EXISTS 
    (SELECT *
    FROM leer l
    WHERE l.num = a.num
        AND NOT EXISTS
            (SELECT *
            FROM escribir e
            WHERE e.cod_ob = l.cod_ob
                AND e.autor_id = 'CAMA')
    )
    AND EXISTS 
        (SELECT *
        FROM leer l1
        WHERE l1.num = a.num)
;


--26. Nombre de amigos tales que todas las obras que han leído son del mismo -autor.
SELECT a.nombre
FROM amigo a
WHERE EXISTS
    (SELECT *
    FROM autor au
    WHERE NOT EXISTS 
        (SELECT *
        FROM leer l
        WHERE l.num = a.num
        AND NOT EXISTS
            (SELECT *
            FROM escribir e
            WHERE e.cod_ob = l.cod_ob
                AND e.autor_id = au.autor_id)
        )
    AND EXISTS 
        (SELECT *
        FROM leer l1
        WHERE l1.num = a.num)
    )
;

