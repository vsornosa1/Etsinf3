-- 1. Títulos de libros que contienen la palabra 'cuento'
SELECT titulo
FROM libro 
WHERE titulo like '%cuento%';

-- 2. Id libros publicados entre 1996 y 1999 (con BETWEEN)
SELECT id_lib
FROM libro
WHERE año BETWEEN 1996 AND 1999;

-- 3. Id libros publicados entre 1996 y 1999 (con IN)
SELECT id_lib
FROM libro
WHERE año IN (1996, 1997, 1998, 1999);

-- 4. Nº libros sin título
SELECT count(*)
FROM libro
WHERE titulo IS null;

-- 5. Títulos de libros sin año
SELECT titulo
FROM libro
WHERE año IS null AND titulo IS NOT null;

-- 6. Año del libro sin título más antiguo
SELECT MIN(año)
FROM libro
WHERE titulo IS null;


----------------------------------------------------------------------


-- 9. Cuántos autores han escrito alguna obra con la palabra “ciudad” en su título
select count(distinct a.nombre) "autores"
from obra o, autor a, escribir e
where a.autor_id = e.autor_id
    and o.cod_ob = e.cod_ob
    and o.titulo like '%ciudad%';

-- 10. Títulos de obras escritos por el autor 'Camús, Albert' (Manera 1)
SELECT o.titulo
FROM autor a, escribir e, obra o
WHERE a.autor_id = e.autor_id
    AND e.cod_ob = o.cod_ob
    AND a.nombre = 'Camús, Albert';
    
-- 10. Títulos de obras escritos por el autor 'Camús, Albert' (Manera 2)
SELECT o.titulo
FROM obra o
WHERE o.cod_ob IN
    (SELECT e.cod_ob
    FROM escribir e
    WHERE e.autor_id IN
        (SELECT a.autor_id
        FROM autor a
        WHERE a.nombre = 'Camús, Albert')
    );

-- 11. Autor de la obra con título 'La Tata'
SELECT a.nombre
FROM autor a, escribir e, obra o
WHERE a.autor_id = e.autor_id
    AND e.cod_ob = o.cod_ob
    AND o.titulo = 'La tata';

-- 12. Nombre de amigos que han leído alguna obra de autor con identificador 'RUKI'
SELECT distinct am.nombre
FROM autor au, escribir e, obra o, leer l, amigo am
WHERE au.autor_id = e.autor_id
    AND e.cod_ob = l.cod_ob
    AND l.num = am.num
    AND au.autor_id = 'RUKI';
    
-- 13.    
SELECT distinct l.id_lib, l.titulo
FROM libro l, esta_en e1, esta_en e2
WHERE l.id_lib = e1.id_lib
    AND l.id_lib = e2.id_lib
    AND e1.cod_ob <> e2.cod_ob
    AND l.titulo IS NOT null;