		.data    
zero:           .double 0.0               ; Comienzo de los datos de memoria
filas_a:        .dword 5
cols_a:         .dword 5 ; filas_b
cols_b:         .dword 5
                ; Matriz A
a:		.double 1,2,3,4,5
		.double 10,20,30,40,50
                .double 100,200,300,400,500
                .double 0.1,0.2,0.3,0.4,0.5
                .double 0.11,0.12,0.13,0.14,0.15
		; Matriz B
b:		.double 1,0,0,0,0
		.double 0,1,0,0,0
                .double 0,0,1,0,0
                .double 0,0,0,1,0
                .double 0,0,0,0,1
		; Matriz C 25x8
c:		.space 200 

		.text                   ; Comienzo del fragmento de código

		dadd r1,$gp,A
		dadd r2,$gp,B
		dadd r3,$gp,C
	
		ld r6,filas_a($gp)	; Filas A
		ld r7,cols_a($gp)	; Columnas A, Filas B
		ld r8,cols_b($gp)	; Columnas B

                dadd r10,r0,r0
bucle_i:
                dadd r11,r0,r0
 bucle_j:
                l.d f3,zero($gp)
	
                dmul r21,r10,r7 ; i*cols
                dadd r21,r21,r11 ; i*cols+j                
                dsll r21,r21,#3
                dadd r12,r0,r0 
bucle_k:		
                ; a[i,k]
                dmul r20,r10,r7 ; i*cols
                dadd r20,r20,r12 ; i*cols+k                
                dsll r20,r20,#3
		l.d f0,A(r20)
                ; b[k,j]
                dmul r20,r12,r8 ; k*cols
                dadd r20,r20,r11 ; i*cols+j                
                dsll r20,r20,#3
		l.d f1,B(r20)
                ; c[i,j]
                mul.d f2,f0,f1
                add.d f3,f3,f2
		s.d f3,C(r21)

                dadd r12,r12,#1
		bne r12,r7,bucle_k
end_k:

		dadd r11,r11,#1
		bne r11,r8,bucle_j
end_j:
		dadd r10,r10,#1
		bne r10,r6,bucle_i
end_i:
                trap #0
    
