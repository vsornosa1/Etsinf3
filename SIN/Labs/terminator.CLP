(defglobal ?*nod-gen* = 0)
(defglobal ?*prof* = 60)



(deffacts mapa
	(limites 1 11)
	(hueco 5 2 )
	(hueco 3 3 )
	(hueco 8 3 )
        (hueco 4 4)
	(hueco 6 4)
	(escalera 3 1)
	(escalera 2 2 )
	(escalera 1 3 )
	(escalera 10 2 )
	(escalera 7 1)
	(escalera 7 3)
	(escalera 11 3)
	(juego pos 1 1 enemigos e 4 2 e 8 2 e 8 4 objetos o 4 3 o 3 4 o 11 2 disparos 2 nivel 0)
)



(defrule subir
	(juego pos ?x ?y $?z nivel ?n)
	(escalera ?x ?y)
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert  (juego pos ?x (+ ?y 1) ?z  nivel (+ ?n 1)))
)

(defrule bajar
	(juego pos ?x ?y $?z nivel ?n)
	(escalera ?x ?w)
	(test (= ?y (+ ?w 1)))
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x ?w ?z nivel (+ ?n 1)))
)

(defrule izquierda
	(juego pos ?x ?y enemigos $?w objetos $?z nivel ?n)
	(limites ?a ?)
	(test (not (member$ (create$ e (- ?x 1) ?y) $?w)))
	
	(not (hueco =(- ?x 1) ?y ))
	(test (> ?x ?a))
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (- ?x 1) ?y enemigos $?w objetos ?z nivel (+ ?n 1)))
)

(defrule derecha
	(juego pos ?x ?y enemigos $?w objetos $?z nivel ?n)
	(limites ? ?b)
	(test (not (member$ (create$ e (+ ?x 1) ?y) $?w)))
	(not (hueco =(+ ?x 1) ?y ))
	(test (< ?x ?b))
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (+ ?x 1) ?y enemigos $?w objetos ?z nivel (+ ?n 1)))
)

(defrule recoger
	(juego pos ?x ?y $?pre o ?x ?y $?post nivel ?n)
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x ?y $?pre $?post nivel (+ ?n 1)))
)

(defrule disparar
	(juego pos ?x ?y $?pre e ?w ?y $?post disparos ?d nivel ?n)
	(test (or (= ?x (- ?w 1))(= ?x (+ ?w 1))))
	(test (> ?d 0))
        (profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
        (bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x ?y $?pre $?post disparos (- ?d 1) nivel (+ ?n 1)))
)

(defrule parar
	(declare (salience 100))
	(juego pos ?x ?y $?resto nivel ?n)
	(test (not (member$ o $?resto)))
	=>
	(printout t "Acabe en " ?x "  " ?y crlf)
	(halt)
        (printout t "SOLUCION ENCONTRADA EN EL NIVEL " ?n crlf)
        (printout t "NUMERO DE NODOS EXPANDIDOS O REGLAS DISPARADAS " ?*nod-gen* crlf)

)
	
(deffunction inicio ()
        (reset)
	(printout t "Profundidad Maxima:= " )
	(bind ?prof (read))
	(printout t "Tipo de Busqueda " crlf "    1.- Anchura" crlf "    2.- Profundidad" crlf )
	(bind ?a (read))
	(if (= ?a 1)
	       then    (set-strategy breadth)
	       else   (set-strategy depth))
        (printout t " Ejecuta run para poner en marcha el programa " crlf)
	(assert (profundidad-maxima ?prof))
	
)