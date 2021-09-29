;; ============================
;; ===     P U Z Z L E     ====
;; ============================

(defglobal ?*nod-gen* = 0)


(defrule derecha
  ?f<-(puzzle $?x 0 ?y $?z nivel ?nivel movimiento ?mov hecho ?)
  (profundidad-maxima ?prof)
  (test (and (<> (length$ $?x) 2) (<> (length$ $?x) 5)))
  (test (neq ?mov izquierda))
  (test (< ?nivel ?prof))
  =>
  (assert (puzzle $?x ?y 0 $?z nivel (+ ?nivel 1) movimiento derecha hecho ?f))
  (bind ?*nod-gen* (+ ?*nod-gen* 1)))


(defrule izquierda
  ?f<-(puzzle $?x ?y 0 $?z nivel ?nivel movimiento ?mov hecho ?)
  (profundidad-maxima ?prof)
  (test (and (<> (length$ $?x) 2) (<> (length$ $?x) 5)))
  (test (neq ?mov derecha))
  (test (< ?nivel ?prof))
  =>
  (assert (puzzle $?x 0 ?y $?z nivel (+ ?nivel 1) movimiento izquierda hecho ?f))
  (bind ?*nod-gen* (+ ?*nod-gen* 1)))


(defrule abajo
  ?f<-(puzzle $?x 0 ?a ?b ?c $?z nivel ?nivel movimiento ?mov hecho ?)
  (profundidad-maxima ?prof)
  (test (neq ?mov arriba))
  (test (< ?nivel ?prof))
  =>
  (assert (puzzle $?x ?c ?a ?b 0 $?z nivel (+ ?nivel 1) movimiento abajo hecho ?f))
  (bind ?*nod-gen* (+ ?*nod-gen* 1)))


(defrule arriba
  ?f<-(puzzle $?x ?a ?b ?c 0 $?y nivel ?nivel movimiento ?mov hecho ?)
  (profundidad-maxima ?prof)
  (test (neq ?mov abajo))
  (test (< ?nivel ?prof))
  =>
  (assert (puzzle $?x 0 ?b ?c ?a $?y nivel (+ ?nivel 1) movimiento arriba hecho ?f))
  (bind ?*nod-gen* (+ ?*nod-gen* 1)))


;; ========================================================
;; =========      ESTRATEGIA DE CONTROL DE BUSQUEDA    ========
;; ========================================================
;; La regla 'objetivo' se utiliza para detectar cuando se ha alcanzado el estado objetivo

(defrule objetivo
    (declare (salience 100))
    ?f<-(puzzle 1 2 3 8 0 4 7 6 5 nivel ?n movimiento ?mov hecho ?)
    
   =>
    (printout t "SOLUCION ENCONTRADA EN EL NIVEL " ?n crlf)
    (printout t "NUMERO DE NODOS EXPANDIDOS O REGLAS DISPARADAS " ?*nod-gen* crlf)
    (printout t "HECHO OBJETIVO " ?f crlf)
    
    (halt))

(defrule no_solucion
    (declare (salience -99))
    (puzzle $? nivel ?n $?)
    
=>
    (printout t "SOLUCION NO ENCONTRADA" crlf)
    (printout t "NUMERO DE NODOS EXPANDIDOS O REGLAS DISPARADAS " ?*nod-gen* crlf)
    
    (halt))		


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
	;;(assert (puzzle 2 8 3 1 6 4 7 0 5 nivel 0 movimiento nulo hecho 0))
	(assert (puzzle 8 1 3 7 2 5 4 0 6 nivel 0 movimiento nulo hecho 0))
	(assert (profundidad-maxima ?prof))
	
)

(deffunction camino
	(?f)
	(bind ?lista (fact-slot-value ?f implied))
	(bind ?l2 (member$ nivel ?lista))
	(bind ?n (nth (+ ?l2 1) ?lista)) 
	;;(printout t "Nivel=" ?n crlf)
	(bind ?dir (nth (length ?lista) ?lista))
	(bind ?mov (subseq$ ?lista (+ ?l2 3) (- (length ?lista) 2))) 
	(bind ?path (create$ ?dir ?mov))
	;;(printout t ?dir "    " ?mov crlf)

	(loop-for-count (- ?n 1) 
		(bind ?lista (fact-slot-value (fact-index ?dir) implied))
		(bind ?dir (nth (length ?lista) ?lista))
		(bind ?l2 (member$ nivel ?lista))
		(bind ?mov (subseq$ ?lista (+ ?l2 3) (- (length ?lista) 2)))
		(bind ?path (create$ ?dir ?mov ?path)) 
	)

	(printout t "Camino: " ?path crlf)
)
