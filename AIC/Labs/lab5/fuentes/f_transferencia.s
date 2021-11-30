	.file	"f_transferencia.c"
	.text
	.p2align 4
	.globl	fase_WB
	.type	fase_WB, @function
fase_WB:
.LFB43:
	.cfi_startproc
	endbr64
	jmp	fase_WB_alum@PLT
	.cfi_endproc
.LFE43:
	.size	fase_WB, .-fase_WB
	.ident	"GCC: (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	 1f - 0f
	.long	 4f - 1f
	.long	 5
0:
	.string	 "GNU"
1:
	.align 8
	.long	 0xc0000002
	.long	 3f - 2f
2:
	.long	 0x3
3:
	.align 8
4:
