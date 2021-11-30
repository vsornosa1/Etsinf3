        .ireg 10,20,0,0,0
        .data
a:		.dword 10
        .text
        ld r3,a($gp)
        dsub r4,r3,#5
        dadd r5,r3,#5
        ; el resultado debe ser r4=5 y r5=15
	trap #0
