	.data
x: 	.dword 0,1,2,3,4,5,6,7,8,9
	.dword 10,11,12,13,14,15

y:  .dword 100,100,100,100,100,100,100,100,100,100
	.dword 100,100,100,100,100,100

; Vector z

z:  .space 128

; Escalar a
a:  .dword -10

; El codigo
    .text

start:
	dadd r1,$gp,x
	dadd r4,r1,#128 
	dadd r2,$gp,y
	dadd r3,$gp,z
	ld r10,a($gp)

loop:
	ld r12,0(r1)
	dadd r12,r10,r12
	ld r14,0(r2)
	dadd r14,r12,r14
	sd r14,0(r3)
	dadd r1,r1,#8
	dadd r2,r2,#8
	dadd r3,r3,#8
	seq r5,r4,r1
	beqz r5,loop
	nop
	nop
	trap #0         ; Fin de programa
