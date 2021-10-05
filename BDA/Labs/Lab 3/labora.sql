-- APARTADO 7.3

-- 14. T�tulo + autor obras escritas por un autor franc�s.
SELECT o.titulo, a.nombre
FROM autor a, escribir e, obra o
WHERE o.cod_ob = e.cod_ob
    AND e.autor_id = a.autor_id
    AND a.nacionalidad = 'Francesa'
    AND
        (SELECT count(*)
        FROM escribir e1
        WHERE e1.cod_ob = o.cod_ob) = 1
;        


-- 15. N� autores de los que no se tiene ninguna obra.
-- NOT IN inlcuye valores nulos
SELECT count(*)
FROM autor a
WHERE a.autor_id NOT IN
    (SELECT e.autor_id
    FROM escribir e)
;
-- NOT EXISTS 
SELECT count(*)
FROM autor a
WHERE NOT EXISTS
    (SELECT *
    FROM escribir e
    WHERE e.autor_id = a.autor_id)
;
-- CONTANDO
SELECT count(*)
FROM autor a
WHERE
    (SELECT count(*)
    FROM escribir e
    WHERE e.autor_id = a.autor_id) = 0
;


-- 16. Nombre autores de los que no se tiene ninguna obra.
SELECT a.nombre
FROM autor a
WHERE
    (SELECT count(*)
    FROM escribir e
    WHERE e.autor_id = a.autor_id) = 0
;


-- 17. Nombre autores de nacionalidad "Espa�ola" con 2 o m�s obras.
SELECT a.nombre
FROM autor a
WHERE
    a.nacionalidad = 'Espa�ola'
    AND
        (SELECT count(*)
        FROM escribir e
        WHERE e.autor_id = a.autor_id) >= 2
;


-- 18. Nombre autores de nacionalidad "Espa�ola" que han escrito alguna 
--     obra que est� en 2 o m�s libros.
SELECT DISTINCT a.nombre
FROM autor a, escribir e
WHERE a.nacionalidad = 'Espa�ola'
    AND a.autor_id = e.autor_id
    AND 
        (SELECT count(*)
        FROM esta_en ee
        WHERE ee.cod_ob = e.cod_ob) >= 2
;


-- 19. T�tulo y c�digo de las obras con m�s de un autor
SELECT o.cod_ob, o.titulo
FROM obra o
WHERE 
    (SELECT count(*)
    FROM escribir e
    WHERE o.cod_ob = e.cod_ob) > 1
;
