	.file	"prediccion.c"
	.text
	.p2align 4
	.type	cambia_estado_saturacion.part.0, @function
cambia_estado_saturacion.part.0:
.LFB59:
	.cfi_startproc
	movl	%esi, %ecx
	movl	$1, %eax
	movl	(%rdi), %edx
	sall	%cl, %eax
	subl	$1, %eax
	cmpl	%eax, %edx
	jge	.L1
	addl	$1, %edx
	movl	%edx, (%rdi)
.L1:
	ret
	.cfi_endproc
.LFE59:
	.size	cambia_estado_saturacion.part.0, .-cambia_estado_saturacion.part.0
	.p2align 4
	.type	cambia_estado_histeresis, @function
cambia_estado_histeresis:
.LFB55:
	.cfi_startproc
	movl	$1, %eax
	movl	%edi, %r9d
	leal	-1(%rdx), %ecx
	movl	%eax, %r8d
	sall	%cl, %r8d
	testl	%r9d, %r9d
	movl	(%rsi), %edi
	je	.L5
	movl	%edx, %ecx
	subl	$1, %r8d
	sall	%cl, %eax
	subl	$1, %eax
	cmpl	%r8d, %edi
	je	.L9
	cmpl	%eax, %edi
	jge	.L4
	addl	$1, %edi
	movl	%edi, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L5:
	cmpl	%r8d, %edi
	je	.L10
	testl	%edi, %edi
	.p2align 4,,2
	jle	.L4
	subl	$1, %edi
	movl	%edi, (%rsi)
.L4:
	.p2align 4,,1
	ret
	.p2align 4,,7
	.p2align 3
.L10:
	movl	$0, (%rsi)
	ret
	.p2align 4,,7
	.p2align 3
.L9:
	movl	%eax, (%rsi)
	ret
	.cfi_endproc
.LFE55:
	.size	cambia_estado_histeresis, .-cambia_estado_histeresis
	.p2align 4
	.globl	inicializa_prediccion
	.type	inicializa_prediccion, @function
inicializa_prediccion:
.LFB56:
	.cfi_startproc
	endbr64
	movq	branch_mask_global@GOTPCREL(%rip), %rax
	pxor	%xmm0, %xmm0
	movl	$0, (%rax)
	movq	branch_mask_local@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	movq	Global@GOTPCREL(%rip), %rax
	movdqu	%xmm0, (%rax)
	movdqu	%xmm0, 16(%rax)
	movdqu	%xmm0, 32(%rax)
	movdqu	%xmm0, 48(%rax)
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	testl	%edx, %edx
	jle	.L11
	movq	BTB@GOTPCREL(%rip), %rcx
	leal	-1(%rdx), %edx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$5, %rdx
	leaq	24(%rcx), %rax
	leaq	120(%rcx,%rdx), %rdx
	.p2align 4
	.p2align 3
.L13:
	pxor	%xmm0, %xmm0
	addq	$96, %rax
	movq	$-1, -120(%rax)
	movl	$0, -112(%rax)
	movdqu	%xmm0, -96(%rax)
	movdqu	%xmm0, -80(%rax)
	movdqu	%xmm0, -64(%rax)
	movq	$0, -104(%rax)
	movl	$0, -32(%rax)
	movl	$0, -28(%rax)
	movdqu	%xmm0, -48(%rax)
	cmpq	%rdx, %rax
	jne	.L13
.L11:
	ret
	.cfi_endproc
.LFE56:
	.size	inicializa_prediccion, .-inicializa_prediccion
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%u %ld %u %ld\n"
.LC1:
	.string	"prediccion.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Error: %s; funci\303\263n:%s; l\303\255nea:%d\n"
	.section	.rodata.str1.1
.LC3:
	.string	"Final de la traza de saltos\n"
	.section	.rodata.str1.8
	.align 8
.LC4:
	.string	"ERROR (%s:%d): Predictor no implementado\n"
	.text
	.p2align 4
	.globl	obtener_prediccion
	.type	obtener_prediccion, @function
obtener_prediccion:
.LFB57:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-40(%rsp), %rsp
	.cfi_def_cfa_offset 96
	movq	branch_mask_global@GOTPCREL(%rip), %rax
	movq	$0, (%rcx)
	movq	tipo_predictor@GOTPCREL(%rip), %r15
	movl	(%rax), %eax
	movq	$0, 8(%r8)
	movl	%eax, (%r8)
	movq	branch_mask_local@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, 4(%r8)
	movl	(%r15), %eax
	cmpl	$10, %eax
	je	.L102
	movq	%rdx, %rbp
	leal	-8(%rax), %edx
	cmpl	$1, %edx
	movq	%rdi, %rbx
	movl	%esi, %r13d
	movq	%rcx, %r14
	jbe	.L107
.L18:
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rsi
	movl	(%rsi), %edi
	testl	%edi, %edi
	jle	.L102
	movq	%r14, %rcx
	xorl	%r12d, %r12d
	movq	BTB@GOTPCREL(%rip), %rax
	movl	%r13d, %r14d
	xorl	%edx, %edx
	leaq	.L52(%rip), %rdi
	movl	$1, %r11d
	movq	%rcx, %r13
	jmp	.L68
	.p2align 4,,7
	.p2align 3
.L49:
	addl	$1, %edx
	addq	$96, %rax
	cmpl	%edx, (%rsi)
	jle	.L15
.L68:
	cmpq	%rbx, (%rax)
	jne	.L49
	movq	estat@GOTPCREL(%rip), %rcx
	movl	(%r15), %r12d
	movl	%r14d, 92(%rax)
	addq	$1, 32(%rcx)
	cmpl	$7, %r12d
	ja	.L50
	movl	%r12d, %ecx
	movslq	(%rdi,%rcx,4), %rcx
	addq	%rdi, %rcx
	notrack jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L52:
	.long	.L56-.L52
	.long	.L55-.L52
	.long	.L55-.L52
	.long	.L54-.L52
	.long	.L54-.L52
	.long	.L53-.L52
	.long	.L51-.L52
	.long	.L51-.L52
	.text
	.p2align 4,,7
	.p2align 3
.L54:
	movq	branch_mask_local@GOTPCREL(%rip), %rcx
	movl	%r11d, %r10d
	movslq	(%rcx), %r9
	movslq	%edx, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%r9,%rcx,8), %r9
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r10d
	movl	%r10d, %ecx
	movq	BTB@GOTPCREL(%rip), %r10
	cmpl	%ecx, 8(%r10,%r9,4)
	jge	.L106
.L61:
	movl	$0, 0(%rbp)
	xorl	%ecx, %ecx
.L62:
	addl	$1, %edx
	addq	$96, %rax
	cmpl	%edx, (%rsi)
	movl	$1, %r12d
	movl	%ecx, 12(%r8)
	movl	$0, 8(%r8)
	jg	.L68
.L15:
	leaq	40(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	movl	%r12d, %eax
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L55:
	.cfi_restore_state
	movq	branch_mask_global@GOTPCREL(%rip), %rcx
	movl	%r11d, %r10d
	movslq	(%rcx), %r9
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r10d
	movl	%r10d, %ecx
	movq	Global@GOTPCREL(%rip), %r10
	cmpl	%ecx, (%r10,%r9,4)
	jge	.L108
	movl	$0, 0(%rbp)
.L64:
	movl	$0, 12(%r8)
	movl	$1, %r12d
	movl	0(%rbp), %ecx
	movl	%ecx, 8(%r8)
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L51:
	movl	8(%rax), %ecx
	subl	$2, %ecx
	cmpl	$1, %ecx
	ja	.L61
.L106:
	movq	16(%rax), %rcx
	movl	$1, 0(%rbp)
	movq	%rcx, 0(%r13)
	movl	$1, %ecx
	jmp	.L62
	.p2align 4,,7
	.p2align 3
.L56:
	movq	branch_mask_local@GOTPCREL(%rip), %rcx
	movl	$0, 8(%rsp)
	movslq	(%rcx), %r9
	movslq	%edx, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%r9,%rcx,8), %r10
	movl	%r11d, %r9d
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r9d
	movl	%r9d, %ecx
	movq	BTB@GOTPCREL(%rip), %r9
	cmpl	%ecx, 8(%r9,%r10,4)
	jl	.L65
	movq	16(%rax), %rcx
	movl	$1, %r12d
	movl	$1, 8(%rsp)
	movq	%rcx, 24(%rsp)
.L65:
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rcx
	movl	%r11d, %r10d
	movl	%r11d, %r9d
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r10d
	movq	branch_mask_global@GOTPCREL(%rip), %rcx
	movl	%r10d, 20(%rsp)
	movslq	(%rcx), %r10
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	subl	$1, %ecx
	sall	%cl, %r9d
	movl	%r9d, %ecx
	movq	Global@GOTPCREL(%rip), %r9
	cmpl	%ecx, (%r9,%r10,4)
	jl	.L66
	movl	8(%rsp), %r9d
	movl	20(%rsp), %r10d
	movq	16(%rax), %rcx
	movl	%r9d, 12(%r8)
	movl	$1, 8(%r8)
	cmpl	%r10d, 88(%rax)
	jl	.L67
.L69:
	movl	%r12d, 0(%rbp)
	testl	%r12d, %r12d
	je	.L72
	movq	24(%rsp), %rcx
	movq	%rcx, 0(%r13)
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L53:
	cmpl	$3, 8(%rax)
	jne	.L61
	jmp	.L106
	.p2align 4,,7
	.p2align 3
.L66:
	movl	8(%rsp), %ecx
	movl	$0, 8(%r8)
	movl	%ecx, 12(%r8)
	movl	20(%rsp), %ecx
	cmpl	88(%rax), %ecx
	jle	.L69
	movl	$0, 0(%rbp)
	movl	$1, %r12d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L108:
	movq	16(%rax), %rcx
	movl	$1, 0(%rbp)
	movq	%rcx, 0(%r13)
	jmp	.L64
	.p2align 4,,7
	.p2align 3
.L107:
	movq	fase_ejecucion@GOTPCREL(%rip), %rdx
	movl	(%rdx), %r12d
	testl	%r12d, %r12d
	je	.L15
	cmpl	$1, %r12d
	jne	.L18
	movq	ciclo1@GOTPCREL(%rip), %rsi
	movq	Ciclo@GOTPCREL(%rip), %rdi
	movl	(%rsi), %esi
	cmpl	%esi, (%rdi)
	je	.L109
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	testl	%ecx, %ecx
	jne	.L110
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$1, (%r10)
	je	.L96
	movq	PC1@GOTPCREL(%rip), %rcx
.L31:
	cmpq	%rbx, (%rcx)
	jne	.L32
	movq	$-1, (%rcx)
	cmpl	$8, (%r15)
	je	.L33
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L35
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L39
	.p2align 4,,7
	.p2align 3
.L111:
	leaq	96(%rax), %rax
.L39:
	cmpq	%rbx, (%rdx)
	jne	.L38
	movl	%r13d, 92(%rdx)
	movl	$1, %esi
.L38:
	cmpq	%rax, %rcx
	movq	%rax, %rdx
	jne	.L111
	testl	%esi, %esi
	je	.L35
.L33:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 32(%rax)
	movq	cond1@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	testl	%edx, %edx
	jne	.L36
	movl	$0, 0(%rbp)
.L37:
	movl	$1, (%r10)
	jmp	.L15
.L32:
	movq	IF_ISS_1@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	leal	-100(%rax), %edx
	cmpl	$5, %edx
	ja	.L112
.L42:
	cmpl	$8, (%r15)
	je	.L101
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L43
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L48
	.p2align 4,,7
	.p2align 3
.L113:
	leaq	96(%rax), %rax
.L48:
	cmpq	%rbx, (%rdx)
	jne	.L47
	movl	%r13d, 92(%rdx)
	movl	$1, %esi
.L47:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L113
	testl	%esi, %esi
	je	.L43
.L101:
	movq	estat@GOTPCREL(%rip), %rax
	addq	$1, 32(%rax)
.L43:
	movl	$0, (%r10)
	.p2align 4
	.p2align 3
.L102:
	xorl	%r12d, %r12d
	jmp	.L15
	.p2align 4,,7
	.p2align 3
.L72:
	movl	$1, %r12d
	jmp	.L49
	.p2align 4,,7
	.p2align 3
.L67:
	movl	$1, 0(%rbp)
	movl	$1, %r12d
	movq	%rcx, 0(%r13)
	jmp	.L49
.L96:
	movq	f_trace@GOTPCREL(%rip), %rax
	leaq	.LC0(%rip), %rsi
	movq	PC1@GOTPCREL(%rip), %rcx
	movq	dest1@GOTPCREL(%rip), %r9
	movq	cond1@GOTPCREL(%rip), %r8
	movq	orden1@GOTPCREL(%rip), %rdx
	movq	(%rax), %rdi
	xorl	%eax, %eax
	movq	%rcx, 8(%rsp)
	call	__isoc99_fscanf@PLT
	movq	orden1@GOTPCREL(%rip), %rax
	movq	8(%rsp), %rcx
	movq	leer_sig_traza@GOTPCREL(%rip), %r10
	cmpl	$-2, (%rax)
	jne	.L31
	movq	stderr@GOTPCREL(%rip), %rbx
	movl	$270, %r9d
	leaq	__func__.4254(%rip), %r8
	leaq	.LC1(%rip), %rcx
	leaq	.LC2(%rip), %rdx
	movl	$1, %esi
	movq	(%rbx), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movq	(%rbx), %rcx
	leaq	.LC3(%rip), %rdi
	movl	$28, %edx
	movl	$1, %esi
	call	fwrite@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L110:
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L102
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L30
	.p2align 4,,7
	.p2align 3
.L114:
	leaq	96(%rax), %rax
.L30:
	cmpq	%rbx, (%rdx)
	jne	.L29
	movl	%r13d, 92(%rdx)
	movl	$1, %esi
.L29:
	cmpq	%rax, %rcx
	movq	%rax, %rdx
	jne	.L114
.L95:
	testl	%esi, %esi
	je	.L102
.L104:
	movq	estat@GOTPCREL(%rip), %rax
	xorl	%r12d, %r12d
	addq	$1, 32(%rax)
	jmp	.L15
.L109:
	movq	IF_ISS_1@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edx
	leal	-100(%rdx), %ecx
	cmpl	$5, %ecx
	ja	.L115
.L23:
	cmpl	$8, %eax
	je	.L104
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	testl	%eax, %eax
	jle	.L102
	leal	-1(%rax), %eax
	xorl	%esi, %esi
	movq	BTB@GOTPCREL(%rip), %rdx
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	leaq	96(%rdx), %rax
	addq	%rax, %rcx
	jmp	.L27
.L116:
	leaq	96(%rax), %rax
.L27:
	cmpq	%rbx, (%rdx)
	jne	.L26
	movl	%r13d, 92(%rdx)
	movl	$1, %esi
.L26:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L116
	jmp	.L95
.L35:
	movq	cond1@GOTPCREL(%rip), %rax
	movl	$0, 0(%rbp)
	movl	(%rax), %eax
	testl	%eax, %eax
	je	.L41
	movq	orden_clarividente@GOTPCREL(%rip), %rax
	movl	%r13d, (%rax)
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	$1, (%rax)
.L41:
	movl	$1, (%r10)
	xorl	%r12d, %r12d
	jmp	.L15
.L112:
	subl	$108, %eax
	cmpl	$3, %eax
	ja	.L43
	jmp	.L42
.L36:
	movq	dest1@GOTPCREL(%rip), %rax
	movl	$1, 0(%rbp)
	movq	ciclo1@GOTPCREL(%rip), %rdi
	movq	(%rax), %rax
	movq	%rax, (%r14)
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %eax
	movl	%eax, (%rdi)
	jmp	.L37
.L115:
	subl	$108, %edx
	cmpl	$3, %edx
	ja	.L102
	jmp	.L23
.L50:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$487, %r8d
	leaq	.LC1(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.cfi_endproc
.LFE57:
	.size	obtener_prediccion, .-obtener_prediccion
	.section	.rodata.str1.1
.LC5:
	.string	"%u %ld %d %ld\n"
	.text
	.p2align 4
	.globl	actualizar_prediccion
	.type	actualizar_prediccion, @function
actualizar_prediccion:
.LFB58:
	.cfi_startproc
	endbr64
	pushq	%r15
	.cfi_def_cfa_offset 16
	.cfi_offset 15, -16
	movl	%ecx, %r15d
	pushq	%r14
	.cfi_def_cfa_offset 24
	.cfi_offset 14, -24
	pushq	%r13
	.cfi_def_cfa_offset 32
	.cfi_offset 13, -32
	movq	%r8, %r13
	pushq	%r12
	.cfi_def_cfa_offset 40
	.cfi_offset 12, -40
	movq	%rsi, %r12
	pushq	%rbp
	.cfi_def_cfa_offset 48
	.cfi_offset 6, -48
	pushq	%rbx
	.cfi_def_cfa_offset 56
	.cfi_offset 3, -56
	leaq	-88(%rsp), %rsp
	.cfi_def_cfa_offset 144
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	%edx, 56(%rsp)
	movl	%edi, 52(%rsp)
	movl	(%rax), %edx
	leal	-8(%rdx), %eax
	cmpl	$1, %eax
	jbe	.L196
.L118:
	movl	%edx, %eax
	andl	$-3, %eax
	cmpl	$8, %eax
	je	.L117
	movq	TAM_BUFFER_PREDIC@GOTPCREL(%rip), %r14
	movl	(%r14), %eax
	testl	%eax, %eax
	jle	.L169
	cmpl	$1, %r15d
	movl	$-1, %ebx
	movslq	52(%rsp), %rdx
	sbbl	%eax, %eax
	movq	%r13, 24(%rsp)
	addl	$3, %eax
	movl	%eax, 60(%rsp)
	cmpl	$1, %r15d
	sbbl	%eax, %eax
	leaq	(%rdx,%rdx,2), %rdx
	andl	$-2, %eax
	addl	$3, %eax
	cmpl	$1, %r15d
	movl	%eax, 64(%rsp)
	sbbl	%eax, %eax
	notl	%eax
	xorl	%r11d, %r11d
	andl	$2, %eax
	xorl	%r8d, %r8d
	movl	%eax, 68(%rsp)
	movq	BTB@GOTPCREL(%rip), %rax
	salq	$5, %rdx
	movq	%rdx, 32(%rsp)
	movq	%rax, 40(%rsp)
	leaq	88(%rax), %r10
	movl	$-1, %eax
	movl	%eax, %r13d
	jmp	.L160
	.p2align 4,,7
	.p2align 3
.L122:
	movl	4(%r10), %edx
	cmpl	%ebx, %edx
	jnb	.L159
	movl	%ebp, %r13d
	movl	%edx, %ebx
.L159:
	addq	$1, %r11
	addq	$96, %r10
	cmpl	%r11d, (%r14)
	jle	.L197
.L160:
	cmpq	%r12, -88(%r10)
	movl	%r11d, %ebp
	jne	.L122
	movq	24(%rsp), %rax
	movq	%rax, -72(%r10)
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	cmpl	$9, %r8d
	ja	.L123
	leaq	.L125(%rip), %rsi
	movl	%r8d, %ecx
	movslq	(%rsi,%rcx,4), %rdx
	addq	%rsi, %rdx
	notrack jmp	*%rdx
	.section	.rodata
	.align 4
	.align 4
.L125:
	.long	.L132-.L125
	.long	.L131-.L125
	.long	.L130-.L125
	.long	.L129-.L125
	.long	.L128-.L125
	.long	.L124-.L125
	.long	.L127-.L125
	.long	.L126-.L125
	.long	.L123-.L125
	.long	.L124-.L125
	.text
.L162:
	cmpl	$9, %edx
	jne	.L117
.L163:
	movq	40(%rsp), %rdi
	leaq	(%rax,%rax,2), %rax
	salq	$5, %rax
	testl	%r15d, %r15d
	je	.L165
	movl	$3, 8(%rdi,%rax)
.L117:
	leaq	88(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L197:
	.cfi_restore_state
	testl	%r8d, %r8d
	movslq	%r13d, %rax
	movq	24(%rsp), %r13
	jne	.L117
	movq	tipo_predictor@GOTPCREL(%rip), %rdi
	movl	(%rdi), %edx
.L121:
	movl	56(%rsp), %edi
	leaq	(%rax,%rax,2), %rsi
	movq	%rsi, %rcx
	salq	$5, %rcx
	addq	40(%rsp), %rcx
	cmpl	$7, %edx
	movq	%r12, (%rcx)
	movl	%edi, 92(%rcx)
	movq	%r13, 16(%rcx)
	ja	.L162
	cmpl	$4, %edx
	ja	.L163
	movslq	52(%rsp), %rdx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$5, %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	52(%rdx), %rcx
	movslq	48(%rdx), %rdx
	jne	.L198
	movq	40(%rsp), %rdi
	leaq	4(%rcx,%rsi,8), %rcx
	movl	$0, 8(%rdi,%rcx,4)
	movq	Global@GOTPCREL(%rip), %rcx
	movl	$0, (%rcx,%rdx,4)
.L167:
	movq	40(%rsp), %rdi
	leaq	(%rax,%rax,2), %rdx
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rax
	salq	$5, %rdx
	movl	(%rax), %ecx
	movl	$1, %eax
	sall	%cl, %eax
	subl	$1, %eax
	movl	%eax, 88(%rdi,%rdx)
	leaq	88(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 56
	popq	%rbx
	.cfi_def_cfa_offset 48
	popq	%rbp
	.cfi_def_cfa_offset 40
	popq	%r12
	.cfi_def_cfa_offset 32
	popq	%r13
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L124:
	.cfi_restore_state
	testl	%r15d, %r15d
	je	.L138
.L136:
	movl	$3, -80(%r10)
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L128:
	movq	32(%rsp), %rdx
	leaq	(%r11,%r11,2), %rcx
	movq	40(%rsp), %rax
	movl	%r15d, %edi
	addq	RB@GOTPCREL(%rip), %rdx
	movslq	52(%rdx), %rdx
	leaq	6(%rdx,%rcx,8), %rdx
	leaq	(%rax,%rdx,4), %rsi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edx
	call	cambia_estado_histeresis
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L129:
	movq	32(%rsp), %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	52(%rdx), %rdx
	je	.L145
	movq	40(%rsp), %rax
	leaq	(%r11,%r11,2), %rcx
	leaq	6(%rdx,%rcx,8), %rdx
	leaq	(%rax,%rdx,4), %rdi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L130:
	movq	32(%rsp), %rdx
	addq	RB@GOTPCREL(%rip), %rdx
	movslq	48(%rdx), %rcx
	movq	Global@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rcx,4), %rsi
.L193:
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movl	%r15d, %edi
	movl	(%rdx), %edx
	call	cambia_estado_histeresis
.L195:
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L131:
	movq	32(%rsp), %rdx
	movq	Global@GOTPCREL(%rip), %rcx
	addq	RB@GOTPCREL(%rip), %rdx
	testl	%r15d, %r15d
	movslq	48(%rdx), %rdx
	je	.L147
	leaq	(%rcx,%rdx,4), %rdi
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L132:
	movq	RB@GOTPCREL(%rip), %r8
	movq	32(%rsp), %rax
	leaq	(%r8,%rax), %rdx
	movl	56(%rdx), %ecx
	movl	60(%rdx), %edx
	cmpl	%edx, %r15d
	jne	.L187
	cmpl	%ecx, %r15d
	jne	.L199
	cmpl	%edx, %r15d
	je	.L150
.L187:
	cmpl	%ecx, %r15d
	.p2align 4,,2
	jne	.L150
	movl	(%r10), %edx
	testl	%edx, %edx
	jle	.L150
	subl	$1, %edx
	movl	%edx, (%r10)
.L150:
	movq	32(%rsp), %rax
	leaq	(%r11,%r11,2), %rsi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rdx
	leaq	(%r8,%rax), %rcx
	movl	(%rdx), %r9d
	movslq	52(%rcx), %rdx
	movq	40(%rsp), %rax
	movq	%rcx, 72(%rsp)
	leaq	6(%rdx,%rsi,8), %rsi
	leaq	(%rax,%rsi,4), %rdi
	movq	subtipo_pred_local@GOTPCREL(%rip), %rsi
	cmpb	$104, (%rsi)
	je	.L200
	testl	%r15d, %r15d
	jne	.L201
	movq	40(%rsp), %rax
	movslq	%ebp, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%rdx,%rcx,8), %rcx
	movl	8(%rax,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L157
	subl	$1, %edx
	movl	%edx, 8(%rax,%rcx,4)
.L157:
	movq	subtipo_pred_global@GOTPCREL(%rip), %rcx
	movq	32(%rsp), %rax
	cmpb	$104, (%rcx)
	movslq	48(%r8,%rax), %rdx
	je	.L156
.L158:
	movq	Global@GOTPCREL(%rip), %rsi
	movl	(%rsi,%rdx,4), %ecx
	testl	%ecx, %ecx
	jle	.L195
	subl	$1, %ecx
	movl	%ecx, (%rsi,%rdx,4)
	jmp	.L195
	.p2align 4,,7
	.p2align 3
.L126:
	movl	-80(%r10), %edx
	cmpl	$2, %edx
	je	.L141
	ja	.L142
	testl	%edx, %edx
	.p2align 4,,2
	je	.L202
	movl	68(%rsp), %eax
	movl	%eax, -80(%r10)
	jmp	.L195
	.p2align 4,,7
	.p2align 3
.L127:
	movl	-80(%r10), %r8d
	testl	%r15d, %r15d
	je	.L134
	cmpl	$1, %r8d
	je	.L135
	cmpl	$2, %r8d
	je	.L136
	testl	%r8d, %r8d
	jne	.L195
	movl	$1, -80(%r10)
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L142:
	cmpl	$3, %edx
	movl	$1, %r8d
	jne	.L122
	movl	60(%rsp), %eax
	movl	%eax, -80(%r10)
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L196:
	movq	fase_ejecucion@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	testl	%ecx, %ecx
	jne	.L118
	movq	f_trace@GOTPCREL(%rip), %rax
	leaq	.LC5(%rip), %rdx
	movq	%r8, (%rsp)
	movl	%r15d, %r9d
	movl	56(%rsp), %ecx
	movq	%rsi, %r8
	movq	(%rax), %rdi
	movl	$1, %esi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movq	tipo_predictor@GOTPCREL(%rip), %rax
	movl	(%rax), %edx
	jmp	.L118
	.p2align 4,,7
	.p2align 3
.L134:
	cmpl	$2, %r8d
	jne	.L203
	.p2align 4
	.p2align 3
.L138:
	movl	$0, -80(%r10)
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L145:
	movq	40(%rsp), %rax
	movslq	%r11d, %rcx
	leaq	(%rcx,%rcx,2), %rcx
	leaq	4(%rdx,%rcx,8), %rcx
	movl	8(%rax,%rcx,4), %edx
	testl	%edx, %edx
	jle	.L195
	movq	40(%rsp), %rax
	subl	$1, %edx
	movl	$1, %r8d
	movl	%edx, 8(%rax,%rcx,4)
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L147:
	movl	(%rcx,%rdx,4), %esi
	testl	%esi, %esi
	jle	.L195
	subl	$1, %esi
	movl	%esi, (%rcx,%rdx,4)
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L203:
	cmpl	$3, %r8d
	je	.L139
	cmpl	$1, %r8d
	jne	.L195
	movl	$0, -80(%r10)
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L201:
	movl	%r9d, %esi
	call	cambia_estado_saturacion.part.0
	movq	subtipo_pred_global@GOTPCREL(%rip), %rdx
	movq	72(%rsp), %rcx
	cmpb	$104, (%rdx)
	je	.L152
	movq	n_bits_pred_global@GOTPCREL(%rip), %rdx
	movslq	48(%rcx), %rcx
	movl	(%rdx), %esi
	movq	Global@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rcx,4), %rdi
.L168:
	call	cambia_estado_saturacion.part.0
	movl	$1, %r8d
	jmp	.L122
	.p2align 4,,7
	.p2align 3
.L199:
	movq	n_bits_pred_hybrid@GOTPCREL(%rip), %rdx
	movq	%r10, %rdi
	movl	(%rdx), %esi
	call	cambia_estado_saturacion.part.0
	jmp	.L150
.L198:
	leaq	4(%rcx,%rsi,8), %rdi
	movl	$1, %esi
	movq	n_bits_pred_local@GOTPCREL(%rip), %rcx
	movl	%esi, %ebx
	movl	(%rcx), %ecx
	sall	%cl, %ebx
	movl	%ebx, %ecx
	movq	40(%rsp), %rbx
	subl	$1, %ecx
	movl	%ecx, 8(%rbx,%rdi,4)
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %ecx
	sall	%cl, %esi
	leal	-1(%rsi), %ecx
	movq	Global@GOTPCREL(%rip), %rsi
	movl	%ecx, (%rsi,%rdx,4)
	jmp	.L167
.L152:
	movslq	48(%rcx), %rdx
.L156:
	movq	Global@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rdx,4), %rsi
	jmp	.L193
.L200:
	movl	%r9d, %edx
	movq	%rdi, %rsi
	movl	%r15d, %edi
	call	cambia_estado_histeresis
	movq	subtipo_pred_global@GOTPCREL(%rip), %rdx
	movq	72(%rsp), %rcx
	cmpb	$104, (%rdx)
	je	.L152
	movslq	48(%rcx), %rdx
	testl	%r15d, %r15d
	je	.L158
	movq	n_bits_pred_global@GOTPCREL(%rip), %rcx
	movl	(%rcx), %esi
	movq	Global@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rdx,4), %rdi
	jmp	.L168
	.p2align 4,,7
	.p2align 3
.L202:
	xorl	%edx, %edx
	testl	%r15d, %r15d
	setne	%dl
	movl	$1, %r8d
	movl	%edx, -80(%r10)
	jmp	.L122
.L141:
	movl	64(%rsp), %eax
	movl	$1, %r8d
	movl	%eax, -80(%r10)
	jmp	.L122
.L165:
	movl	$0, 8(%rdi,%rax)
	jmp	.L117
	.p2align 4,,7
	.p2align 3
.L123:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$703, %r8d
	leaq	.LC1(%rip), %rcx
	leaq	.LC4(%rip), %rdx
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L139:
	movl	$2, -80(%r10)
	movl	$1, %r8d
	jmp	.L122
.L135:
	movl	$3, -80(%r10)
	jmp	.L122
.L169:
	movq	BTB@GOTPCREL(%rip), %rdi
	movq	$-1, %rax
	movq	%rdi, 40(%rsp)
	jmp	.L121
	.cfi_endproc
.LFE58:
	.size	actualizar_prediccion, .-actualizar_prediccion
	.section	.rodata
	.align 16
	.type	__func__.4254, @object
	.size	__func__.4254, 19
__func__.4254:
	.string	"obtener_prediccion"
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
