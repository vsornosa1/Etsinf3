-- 33. Nombre del autor/autores que más obras han escrito.
SELECT au.nombre NUM_OBRAS
FROM autor au, escribir e
WHERE au.autor_id = e.autor_id
GROUP BY au.nombre
HAVING count(*) = 
    (SELECT MAX(count(*))
    FROM escribir e1
    GROUP BY e1.autor_id)
;


-- 33. Nombre del autor/autores que más obras han escrito usando:
--    (FROM t1 JOIN t2 ON <condición>)
SELECT au.nombre NUM_OBRAS
FROM autor au JOIN escribir e ON au.autor_id = e.autor_id
GROUP BY au.nombre
HAVING count(*) = 
    (SELECT MAX(count(*))
    FROM escribir e1
    GROUP BY e1.autor_id)
;


-- 31. Temas con alguna obra, 
--     temática y la cantidad de obras con ese tema.
SELECT t.tematica, count(*)
FROM tema t JOIN obra o ON t.tematica = o.tematica
GROUP BY t.tematica
ORDER BY t.tematica;


-- 32. Obtener, de todos los temas de la base de datos, 
--     la temática y la cantidad de obras con ese tema.
SELECT t.tematica, count(o.cod_ob)
FROM tema t LEFT JOIN obra o ON t.tematica = o.tematica
GROUP BY t.tematica
ORDER BY t.tematica;

-- 23. Obtener el número de canciones que ha grabado
--     cada discografía y su dirección.
SELECT c.nombre, c.dir, count(e.can)
FROM companyia c LEFT JOIN (disco d JOIN esta e ON d.cod = e.cod)
    ON c.cod = d.cod_comp
GROUP BY c.nombre, c.dir;
