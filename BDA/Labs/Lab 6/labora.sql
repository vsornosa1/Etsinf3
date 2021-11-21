-- 29. T�tulo + id libros que tengan t�tulo y m�s de una obra.
--     Resolver sin utilizar num_obras (Usar cl�usula GROUP BY)
SELECT l.titulo, l.id_lib
FROM libro l, esta_en ee
WHERE l.id_lib = ee.id_lib
    AND l.titulo IS NOT null
GROUP BY l.id_lib, l.titulo
HAVING count(*) > 1;


-- 30. Nombre amigos que han le�do m�s de 3 obras
--     indicando tambien la cantidad de obras le�das.
SELECT a.nombre, count(*)
FROM amigo a , leer l
WHERE l.num = a.num
GROUP BY a.num, a.nombre
HAVING count(*) > 3;


-- 31. Obtener de los temas con alguna obra, la tem�tica 
--     y la cantidad de obras con esa tema.
SELECT t.tematica, count(*)
FROM obra o, tema t
WHERE t.tematica = o.tematica
GROUP BY t.tematica;