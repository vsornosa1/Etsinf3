SELECT a.nombre
FROM amigo a
WHERE EXISTS 
    (SELECT *
    FROM autor au
    WHERE NOT EXISTS 
        (SELECT *
        FROM leer 
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