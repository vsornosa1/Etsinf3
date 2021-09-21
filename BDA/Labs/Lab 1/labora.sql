SELECT COUNT(cod)
FROM disco;

SELECT nombre
FROM grupo
WHERE pais != 'España';

SELECT titulo
FROM cancion
WHERE duracion>5;

SELECT distinct funcion
FROM pertenece
ORDER BY funcion;

SELECT nombre, num
FROM club
ORDER BY num;

SELECT nombre, sede
FROM club
WHERE num > 500;