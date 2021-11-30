	.data              ; Comienzo de la zona de datos (apuntado por $gp)
a:	.double 10.5
b:	.double 2
c:	.double 20

s1:	.space 8
s2:	.space 8

	.text              ; Comienzo del fragmento de código
inicio:
	l.d f0, a($gp)     ; Carga a
	l.d f1, b($gp)     ; Carga b
	l.d f2, c($gp)     ; Carga c
	add.d f4, f0, f1   ; t1= a + b
	mul.d f5, f2, f4   ; t2= c * t1 
	s.d f4, s1($gp)    ; Guarda t1
	s.d f5, s2($gp)    ; Guarda t2
final:
	trap 0             ; Final del programa
