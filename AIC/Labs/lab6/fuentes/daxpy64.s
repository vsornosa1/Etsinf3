	.data
	; Vector x
x:	.double 1, 2, 3, 4, 5, 6, 7, 8
        .double 11, 12, 13, 14, 15, 16, 17, 18
	.double 21, 22, 23, 24, 25, 26, 27, 28
	.double 31, 32, 33, 34, 35, 36, 37, 38
	.double 41, 42, 43, 44, 45, 46, 47, 48
	.double 51, 52, 53, 54, 55, 56, 57, 58
	.double 61, 62, 63, 64, 65, 66, 67, 68
	.double 71, 72, 73, 74, 75, 76, 77, 78
	; Vector y 
y:	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	.double 100, 100, 100, 100, 100, 100, 100, 100
	; Vector z, 64*8 bytes/elem 
z:	.space 512
a:	.double 2	

	.text

	dadd r1, r0, x 
	dadd r2, r0, y 
	dadd r3, r0, z 
	dadd r4, r1, 512
	l.d f0, a(r0)	
loop:	
	l.d f1, 0(r1)		; Load X
	l.d f2, 0(r2)		; Load Y
	mul.d f3, f1, f0	; a*X
	add.d f4, f2, f3	; a*X +Y
	s.d f4, 0(r3)		; Store Z
	dadd r1, r1, 8
	dadd r2, r2, 8
	dadd r3, r3, 8
	dsub r5, r4, r1
	bnez r5, loop

	trap 0
