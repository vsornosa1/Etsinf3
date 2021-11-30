        .ireg 10,20,0,0,0
        .text
        dadd r3,r1,r2
        dsub r4,r3,#5
        dadd r5,r3,#5
        ; el resultado debe ser r4=25 y r5=35
	trap #0
