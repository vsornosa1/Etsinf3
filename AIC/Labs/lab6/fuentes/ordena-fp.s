	.fpreg 0.0,0.0,0.0
        .data
	; Matriz A
a:	.double 3.00, 2.00, 1.00, 0.00, 5.00, 0.5, 3.2, 1.2, -1.0, 1.9, 1.8, -2.0, -3.0, -4.0, -5.0, -6.0, 0.1, 0.3, 0.2, 0.9, 0.8, 0.6, 0.7, 0.5
afin:

	.text

start:	dadd r1,$gp,a
	dadd r5,$gp,afin
	dsub r4,r5,#8

loopi:	
	l.d f0,0(r1)
	dadd r2,r1,#8
loopj:
	l.d f1,0(r2)
        dsub r3,r6,r7
	c.gt.d f0, f1
        bc1f endif
then:	
	s.d f1,0(r1)
	s.d f0,0(r2)
	add.d f0,f1,f3
endif:	
	dadd r2,r2,#8
	bne r2,r5, loopj
endj:	
	dadd r1,r1,#8
	bne r1,r4, loopi
endi:
	trap #0

