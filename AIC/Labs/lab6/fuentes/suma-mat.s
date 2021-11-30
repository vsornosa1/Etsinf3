	.data
filas: 	.dword 10
cols: 	.dword 8

a:  .double 1, 2, 3, 4, 5, 6, 7, 8
    .double 11, 12, 13, 14, 15, 16, 17, 18
	.double 21, 22, 23, 24, 25, 26, 27, 28
	.double 31, 32, 33, 34, 35, 36, 37, 38
	.double 41, 42, 43, 44, 45, 46, 47, 48
	.double 51, 52, 53, 54, 55, 56, 57, 58
	.double 61, 62, 63, 64, 65, 66, 67, 68
	.double 71, 72, 73, 74, 75, 76, 77, 78
	.double 81, 82, 83, 84, 85, 86, 87, 88
	.double 91, 92, 93, 94, 95, 96, 97, 98

b:  .double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	
	
c: 	.space 	640

	.text
        dadd r1, r0, filas
        dadd r2, r0, cols
        dadd r3, r0, a
        dadd r4, r0, b
        dadd r5, r0, c

        ld r1, 0(r1)	; Filas
        ld r2, 0(r2)	; Columnas

        dadd r6, r0, r0 ; ptr
        dadd r7, r0, r1 ; i

	;Bucle i
buclei: dadd r8, r0, r2 ; j

	;Bucle j
buclej: l.d f0, a(r6)
        l.d f1, b(r6)
        add.d f2, f0, f1
        s.d f2, c(r6)
        daddi r6, r6, 8
        dsubi r8, r8, 1
        bnez r8, buclej
;Fin bucle j

        dsubi r7, r7, 1
        bnez r7, buclei
;Fin bucle i

        trap 0
