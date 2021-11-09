; Proyecto de prácticas realizada por Miguel Palacios y Vicent Sornosa. Grupo 3G L2

(defglobal ?*nod-gen* = 0)
(defglobal ?*prof* = 60)


(deffacts mapa
	(limite_X 1 8)
	(limite_Y 1 5)
	(triturador 1 3)
	(triturador 1 5)
	(triturador 2 1)
	(triturador 4 3)
	(triturador 4 5)
	(triturador 6 1)
	(triturador 7 5)
	(triturador 8 1)
	(triturador 8 3)
	(juego pos 2 3 latas o 3 1 o 3 3 o 6 4 nivel 0) 
)


(defrule robot_izquierda
	(juego pos ?x ?y latas $?z nivel ?n)
	(limite_X ?min ?max)
	(test (> ?x ?min))

	(test (not (member$ (create$ o (- ?x 1) ?y) $?z)))

	(not (triturador =(- ?x 1) ?y))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (- ?x 1) ?y latas $?z nivel (+ ?n 1)))
)


(defrule robot_derecha
	(juego pos ?x ?y latas $?z nivel ?n)
	(limite_X ?min ?max)
	(test (< ?x ?max))

	(test (not (member$ (create$ o (+ ?x 1) ?y) $?z)))

	(not (triturador =(+ ?x 1) ?y))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (+ ?x 1) ?y latas $?z nivel (+ ?n 1)))
)


(defrule robot_arriba
	(juego pos ?x ?y latas $?z nivel ?n)
	(limite_Y ?min ?max)
	(test (< ?y ?max))

	(test (not (member$ (create$ o ?x (+ ?y 1)) $?z)))

	(not (triturador ?x =( + ?y 1)))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (+ ?y 1) latas $?z nivel (+ ?n 1)))
)


(defrule robot_abajo
	(juego pos ?x ?y latas $?z nivel ?n)
	(limite_Y ?min ?max)
	(test (> ?y ?min))

	(test (not (member$ (create$ o ?x (- ?y 1)) $?z)))

	(not (triturador ?x =(- ?y 1)))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (- ?y 1) latas $?z nivel (+ ?n 1)))
)


(defrule lata_izquierda
	(juego pos ?x ?y $?pre o ?w ?y $?post nivel ?n)
	(limite_X ?min ?max)
	(test (> (- ?x 1) ?min))

	(test (not (member$ (create$ o (- ?x 2) ?y) $?pre)))
	(test (not (member$ (create$ o (- ?x 2) ?y) $?post)))
	(test (= (- ?x 1) ?w))

	(not (triturador =(- ?x 2) ?y))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (- ?x 1) ?y $?pre o (- ?w 1)  ?y $?post nivel (+ ?n 1)))
)


(defrule lata_derecha
	(juego pos ?x ?y $?pre o ?w ?y $?post nivel ?n)
	(limite_X ?min ?max)
	(test (< (+ ?x 1) ?max))

	(test (not (member$ (create$ o (+ ?x 2) ?y) $?pre)))
	(test (not (member$ (create$ o (+ ?x 2) ?y) $?post)))
	(test (= (+ ?x 1) ?w))

	(not (triturador = (+ ?x 2) ?y))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (+ ?x 1) ?y $?pre o (+ ?w 1) ?y $?post nivel (+ ?n 1)))
)


(defrule lata_arriba
	(juego pos ?x ?y $?pre o ?x ?w $?post nivel ?n)
	(limite_Y ?min ?max)
	(test (< (+ ?y 1) ?max))

	(test (= (+ ?y 1) ?w))
	(test (not (member$ (create$ o ?x (+ ?y 2)) $?pre)))
	(test (not (member$ (create$ o ?x (+ ?y 2)) $?post)))

	(not (triturador ?x =(+ ?y 2)))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (+ ?y 1) $?pre o ?x (+ ?w 1) $?post nivel (+ ?n 1)))
)


(defrule lata_abajo
	(juego pos ?x ?y $?pre o ?x ?w $?post nivel ?n)
	(limite_Y ?min ?max)
	(test (> (- ?y 1) ?min))
	(test (= (- ?y 1) ?w))
	(test (not (member$ (create$ o ?x (- ?y 2)) $?pre)))
	(test (not (member$ (create$ o ?x (- ?y 2)) $?post)))

	(not (triturador ?x =(- ?y 2)))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (- ?y 1) $?pre o ?x (- ?w 1) $?post nivel (+ ?n 1)))
)


(defrule tritura_izquierda
	(juego pos ?x ?y $?pre o ?w ?y $?post nivel ?n)
	(limite_X ?min ?max)
	(test (> (- ?x 1) ?min))
		
	(test (= (- ?x 1) ?w))

	(triturador =(- ?x 2) ?y)

	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (- ?x 1) ?y $?pre $?post nivel (+ ?n 1)))
)


(defrule tritura_derecha
	(juego pos ?x ?y $?pre o ?w ?y $?post nivel ?n)
	(limite_X ?min ?max)
	(test (< (+ ?x 1) ?max))
	
	(test (= (+ ?x 1) ?w))

	(triturador = (+ ?x 2) ?y)
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos (+ ?x 1) ?y $?pre $?post nivel (+ ?n 1)))
)


(defrule tritura_arriba
	(juego pos ?x ?y $?pre o ?x ?w $?post nivel ?n)
	(limite_Y ?min ?max)
	(test (< (+ ?y 1) ?max))

	(test (= (+ ?y 1) ?w))

	(triturador ?x =(+ ?y 2))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (+ ?y 1) $?pre $?post nivel (+ ?n 1)))
)


(defrule tritura_abajo
	(juego pos ?x ?y $?pre o ?x ?w $?post nivel ?n)
	(limite_Y ?min ?max)
	(test (> (- ?y 1) ?min))

	(test (= (- ?y 1) ?w))
	(triturador ?x =(- ?y 2))
		
	(profundidad-maxima ?prof)
        (test (< ?n ?prof))
	=>
	(bind ?*nod-gen* (+ ?*nod-gen* 1))
	(assert (juego pos ?x (- ?y 1) $?pre $?post nivel (+ ?n 1)))
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