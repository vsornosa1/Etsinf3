	.file	"f_confirmacion.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"x"
.LC1:
	.string	"X"
	.text
	.p2align 4
	.globl	cancelar_instrucciones
	.type	cancelar_instrucciones, @function
cancelar_instrucciones:
.LFB43:
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
	leaq	-24(%rsp), %rsp
	.cfi_def_cfa_offset 80
	movq	TAM_REORDER@GOTPCREL(%rip), %rbp
	movq	RB@GOTPCREL(%rip), %rax
	movl	0(%rbp), %r9d
	movq	%rax, 8(%rsp)
	testl	%r9d, %r9d
	jle	.L41
	movq	%rax, %r14
	xorl	%r12d, %r12d
	movq	RB_inicio@GOTPCREL(%rip), %r13
	leaq	.LC0(%rip), %r15
	.p2align 4
	.p2align 3
.L4:
	movl	(%r14), %r8d
	testl	%r8d, %r8d
	je	.L3
	cmpl	%r12d, 0(%r13)
	je	.L3
	movl	92(%r14), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	%r15, %rdi
	call	*64(%rax)
.L3:
	addl	$1, %r12d
	addq	$96, %r14
	cmpl	%r12d, 0(%rbp)
	jg	.L4
.L2:
	movq	TAM_ESTACIONES@GOTPCREL(%rip), %r12
	movl	(%r12), %edi
	testl	%edi, %edi
	jle	.L5
	movq	RS@GOTPCREL(%rip), %r14
	xorl	%r15d, %r15d
	leaq	.LC1(%rip), %rbx
	jmp	.L8
	.p2align 4,,7
	.p2align 3
.L6:
	addl	$1, %r15d
	addq	$168, %r14
	cmpl	%r15d, (%r12)
	jle	.L5
.L8:
	movl	(%r14), %esi
	testl	%esi, %esi
	je	.L6
	cmpl	$3, 124(%r14)
	jne	.L6
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rdx
	movl	(%rax), %eax
	addl	(%rdx), %eax
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rdx
	addl	(%rdx), %eax
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rdx
	addl	(%rdx), %eax
	cmpl	%r15d, %eax
	jg	.L7
	movq	TAM_BUFFER_ALMACEN@GOTPCREL(%rip), %rdx
	addl	(%rdx), %eax
	cmpl	%r15d, %eax
	jg	.L6
.L7:
	movl	160(%r14), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	%rbx, %rdi
	call	*64(%rax)
	jmp	.L6
	.p2align 4,,7
	.p2align 3
.L5:
	movq	NUM_OPERADORES@GOTPCREL(%rip), %r14
	movl	(%r14), %esi
	testl	%esi, %esi
	jle	.L9
	movq	Op@GOTPCREL(%rip), %r15
	xorl	%ebx, %ebx
	.p2align 4
	.p2align 3
.L12:
	movl	(%r15), %ecx
	testl	%ecx, %ecx
	je	.L10
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rsi
	movslq	8(%r15), %rdx
	movl	(%rax), %eax
	addl	(%rsi), %eax
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rsi
	addl	(%rsi), %eax
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rsi
	addl	(%rsi), %eax
	cmpl	%eax, %edx
	jl	.L11
	movq	TAM_BUFFER_ALMACEN@GOTPCREL(%rip), %rsi
	addl	(%rsi), %eax
	cmpl	%eax, %edx
	jl	.L10
.L11:
	leaq	(%rdx,%rdx,4), %rax
	leaq	.LC1(%rip), %rdi
	leaq	(%rdx,%rax,4), %rax
	movq	RS@GOTPCREL(%rip), %rdx
	leaq	(%rdx,%rax,8), %rax
	xorl	%edx, %edx
	movl	160(%rax), %esi
	movq	presentacion@GOTPCREL(%rip), %rax
	call	*64(%rax)
.L10:
	movl	(%r14), %esi
	addl	$1, %ebx
	addq	$28, %r15
	cmpl	%ebx, %esi
	jg	.L12
.L9:
	movq	TAM_REGISTROS@GOTPCREL(%rip), %rax
	movslq	(%rax), %r8
	testl	%r8d, %r8d
	jle	.L42
	leal	-1(%r8), %edx
	movl	$8, %eax
	movq	Rfp@GOTPCREL(%rip), %rdi
	salq	$4, %rdx
	movq	Rint@GOTPCREL(%rip), %rcx
	addq	$24, %rdx
	.p2align 4
	.p2align 3
.L14:
	movl	$4096, (%rax,%rdi)
	movl	$4096, (%rcx,%rax)
	addq	$16, %rax
	cmpq	%rdx, %rax
	jne	.L14
.L13:
	movl	(%r12), %eax
	salq	$4, %r8
	movl	$4096, 8(%rcx,%r8)
	testl	%eax, %eax
	jle	.L15
	movq	RS@GOTPCREL(%rip), %rdx
	leal	-1(%rax), %eax
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rcx
	leaq	168(%rdx), %rax
	leaq	(%rax,%rcx,8), %rcx
	jmp	.L18
	.p2align 4,,7
	.p2align 3
.L43:
	leaq	168(%rax), %rax
.L18:
	movl	(%rdx), %r11d
	testl	%r11d, %r11d
	je	.L16
	movl	120(%rdx), %r10d
	testl	%r10d, %r10d
	jne	.L17
.L16:
	movl	$0, (%rdx)
.L17:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L43
.L15:
	testl	%esi, %esi
	jle	.L19
	movq	Op@GOTPCREL(%rip), %rdx
	leal	-1(%rsi), %eax
	leaq	0(,%rax,8), %rcx
	subq	%rax, %rcx
	leaq	28(%rdx), %rax
	leaq	(%rax,%rcx,4), %rcx
	jmp	.L22
	.p2align 4,,7
	.p2align 3
.L21:
	movl	$0, (%rdx)
.L20:
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	je	.L19
.L44:
	leaq	28(%rax), %rax
.L22:
	movl	(%rdx), %r9d
	testl	%r9d, %r9d
	je	.L20
	cmpl	$3, 4(%rdx)
	jne	.L21
	movslq	8(%rdx), %rsi
	leaq	(%rsi,%rsi,4), %rdi
	leaq	(%rsi,%rdi,4), %rsi
	movq	RS@GOTPCREL(%rip), %rdi
	leaq	(%rdi,%rsi,8), %rsi
	movl	120(%rsi), %r8d
	testl	%r8d, %r8d
	je	.L21
	cmpq	%rcx, %rax
	movq	%rax, %rdx
	jne	.L44
	.p2align 4
	.p2align 3
.L19:
	movslq	0(%r13), %rax
	leaq	(%rax,%rax,2), %rax
	salq	$5, %rax
	addq	8(%rsp), %rax
	movl	40(%rax), %edx
	testl	%edx, %edx
	jne	.L23
	movq	80(%rax), %rdx
	addq	$4, %rdx
.L24:
	movq	Control_1@GOTPCREL(%rip), %rax
	movq	%rdx, (%rax)
	movq	$1, 12(%rax)
	movl	0(%rbp), %eax
	testl	%eax, %eax
	jle	.L25
	movq	8(%rsp), %rbx
	leal	-1(%rax), %eax
	leaq	(%rax,%rax,2), %rdx
	salq	$5, %rdx
	leaq	96(%rbx), %rax
	addq	%rax, %rdx
	jmp	.L26
	.p2align 4,,7
	.p2align 3
.L45:
	leaq	96(%rax), %rax
.L26:
	movl	$0, (%rbx)
	cmpq	%rax, %rdx
	movl	$0, 72(%rbx)
	movq	%rax, %rbx
	jne	.L45
.L25:
	movq	RB_long@GOTPCREL(%rip), %rax
	movl	$0, 0(%r13)
	movl	$0, (%rax)
	movq	RB_fin@GOTPCREL(%rip), %rax
	movl	$0, (%rax)
	leaq	24(%rsp), %rsp
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
.L23:
	.cfi_restore_state
	movq	32(%rax), %rdx
	jmp	.L24
	.p2align 4,,7
	.p2align 3
.L41:
	movq	RB_inicio@GOTPCREL(%rip), %r13
	jmp	.L2
	.p2align 4,,7
	.p2align 3
.L42:
	movq	Rint@GOTPCREL(%rip), %rcx
	jmp	.L13
	.cfi_endproc
.LFE43:
	.size	cancelar_instrucciones, .-cancelar_instrucciones
	.p2align 4
	.globl	prediccion_correcta
	.type	prediccion_correcta, @function
prediccion_correcta:
.LFB44:
	.cfi_startproc
	endbr64
	movq	RB_inicio@GOTPCREL(%rip), %rax
	movslq	(%rax), %rax
	leaq	(%rax,%rax,2), %rax
	salq	$5, %rax
	addq	RB@GOTPCREL(%rip), %rax
	movl	40(%rax), %edx
	cmpl	%edx, 44(%rax)
	sete	%al
	movzbl	%al, %eax
	ret
	.cfi_endproc
.LFE44:
	.size	prediccion_correcta, .-prediccion_correcta
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC2:
	.string	"Ciclo %u: Acceso a memoria de instrucciones ilegal. PC=%ld (%s)\n"
	.align 8
.LC3:
	.string	"Ciclo %u: Acceso a memoria de datos ilegal. PC=%ld (%s)\n"
	.align 8
.LC4:
	.string	"Ciclo %u: Acceso deslineado a memoria de datos. PC=%ld (%s)\n"
	.align 8
.LC5:
	.string	"Ciclo %u: El programa est\303\241 terminando normalmente. PC=%ld (%s)\n"
	.align 8
.LC6:
	.string	"Ciclo %u: Excepci\303\263n no manejada. PC=%ld (%s)\n"
	.section	.rodata.str1.1
.LC7:
	.string	"C"
.LC8:
	.string	"f_confirmacion.c"
	.section	.rodata.str1.8
	.align 8
.LC9:
	.string	"ERROR (%s:%d): Operacion %d no implementada\n"
	.text
	.p2align 4
	.globl	fase_COM
	.type	fase_COM, @function
fase_COM:
.LFB46:
	.cfi_startproc
	endbr64
	pushq	%r14
	.cfi_def_cfa_offset 16
	.cfi_offset 14, -16
	pushq	%r13
	.cfi_def_cfa_offset 24
	.cfi_offset 13, -24
	pushq	%r12
	.cfi_def_cfa_offset 32
	.cfi_offset 12, -32
	pushq	%rbp
	.cfi_def_cfa_offset 40
	.cfi_offset 6, -40
	pushq	%rbx
	.cfi_def_cfa_offset 48
	.cfi_offset 3, -48
	leaq	-144(%rsp), %rsp
	.cfi_def_cfa_offset 192
	movq	terminando@GOTPCREL(%rip), %rbp
	movq	RB_inicio@GOTPCREL(%rip), %rbx
	movq	RB_cab@GOTPCREL(%rip), %rdx
	movl	0(%rbp), %r12d
	movq	%fs:40, %rax
	movq	%rax, 136(%rsp)
	xorl	%eax, %eax
	movl	(%rbx), %eax
	testl	%r12d, %r12d
	movl	%eax, (%rdx)
	jne	.L47
	movq	RB@GOTPCREL(%rip), %r13
	movslq	%eax, %rdx
	leaq	(%rdx,%rdx,2), %rax
	salq	$5, %rax
	addq	%r13, %rax
	movl	(%rax), %r11d
	testl	%r11d, %r11d
	jne	.L151
.L47:
	movq	136(%rsp), %rax
	xorq	%fs:40, %rax
	jne	.L152
	leaq	144(%rsp), %rsp
	.cfi_remember_state
	.cfi_def_cfa_offset 48
	popq	%rbx
	.cfi_def_cfa_offset 40
	popq	%rbp
	.cfi_def_cfa_offset 32
	popq	%r12
	.cfi_def_cfa_offset 24
	popq	%r13
	.cfi_def_cfa_offset 16
	popq	%r14
	.cfi_def_cfa_offset 8
	ret
	.p2align 4,,7
	.p2align 3
.L151:
	.cfi_restore_state
	movl	4(%rax), %esi
	movl	8(%rax), %r10d
	leal	-16(%rsi), %ecx
	cmpl	$2, %ecx
	setbe	%cl
	cmpl	$65, %esi
	sete	%dil
	orl	%edi, %ecx
	testl	%r10d, %r10d
	je	.L153
	movq	estat@GOTPCREL(%rip), %r12
	addq	$1, 8(%r12)
	testb	%cl, %cl
	je	.L52
	movq	16(%rax), %rax
	movq	RS@GOTPCREL(%rip), %rsi
.L53:
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rdx,%rdx,2), %rcx
	leaq	(%rax,%rdi,4), %rax
	movl	152(%rsi,%rax,8), %eax
	salq	$5, %rcx
	movl	%eax, 72(%r13,%rcx)
.L54:
	leaq	(%rdx,%rdx,2), %rax
	movq	%rsp, %r14
	xorl	%edx, %edx
	movq	%r14, %rdi
	salq	$5, %rax
	movq	80(%r13,%rax), %rsi
	call	imprime_instruccion_color@PLT
	movslq	(%rbx), %rcx
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	movl	72(%r13,%rax), %eax
	cmpl	$32, %eax
	jg	.L55
	testl	%eax, %eax
	js	.L56
	cmpl	$32, %eax
	ja	.L56
	leaq	.L58(%rip), %rdx
	movslq	(%rdx,%rax,4), %rax
	addq	%rdx, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L58:
	.long	.L62-.L58
	.long	.L61-.L58
	.long	.L60-.L58
	.long	.L56-.L58
	.long	.L59-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L56-.L58
	.long	.L57-.L58
	.text
	.p2align 4,,7
	.p2align 3
.L153:
	testb	%cl, %cl
	je	.L154
	movq	16(%rax), %rax
	movq	RS@GOTPCREL(%rip), %rsi
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rcx
	cmpl	$1, 124(%rsi,%rcx,8)
	jne	.L47
	movq	estat@GOTPCREL(%rip), %r12
	addq	$1, 8(%r12)
	jmp	.L53
.L62:
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	64(%rax), %r8
.L74:
	leaq	(%rcx,%rcx,2), %rax
	xorl	%edx, %edx
	leaq	.LC7(%rip), %rdi
	salq	$5, %rax
	movl	92(%r13,%rax), %esi
	call	*%r8
.L75:
	movslq	(%rbx), %rdx
	leaq	(%rdx,%rdx,2), %rcx
	movq	%rdx, %rax
	salq	$5, %rcx
	movl	4(%r13,%rcx), %r9d
	cmpl	$183, %r9d
	ja	.L76
	leaq	.L78(%rip), %rdi
	movl	%r9d, %esi
	movslq	(%rdi,%rsi,4), %rcx
	addq	%rdi, %rcx
	notrack jmp	*%rcx
	.section	.rodata
	.align 4
	.align 4
.L78:
	.long	.L87-.L78
	.long	.L86-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L79-.L78
	.long	.L79-.L78
	.long	.L79-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L79-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L85-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L84-.L78
	.long	.L84-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L83-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L77-.L78
	.long	.L79-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L76-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L77-.L78
	.long	.L79-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L82-.L78
	.long	.L76-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L81-.L78
	.long	.L80-.L78
	.long	.L80-.L78
	.long	.L80-.L78
	.long	.L80-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L76-.L78
	.long	.L77-.L78
	.long	.L79-.L78
	.long	.L77-.L78
	.long	.L77-.L78
	.text
.L77:
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	addq	%r13, %rcx
	movq	16(%rcx), %rdi
	movq	24(%rcx), %rcx
	movq	%rdi, %rsi
	salq	$4, %rsi
	addq	Rfp@GOTPCREL(%rip), %rsi
	movq	%rcx, (%rsi)
	cmpl	8(%rsi), %edx
	jne	.L99
	movl	$4096, 8(%rsi)
.L99:
	leal	-141(%r9), %ecx
	cmpl	$42, %ecx
	jbe	.L100
	leal	-128(%r9), %ecx
.L101:
	movabsq	$18014398509490111, %rsi
	btq	%rcx, %rsi
	jnc	.L103
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	movl	$6, (%rcx,%rdi,4)
.L103:
	leal	-154(%r9), %ecx
	cmpl	$3, %ecx
	jbe	.L118
	subl	$180, %r9d
	cmpl	$1, %r9d
	ja	.L87
.L118:
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	movl	$7, (%rcx,%rdi,4)
	.p2align 4
	.p2align 3
.L87:
	leaq	(%rdx,%rdx,2), %rdx
	addl	$1, %eax
	movq	TAM_REORDER@GOTPCREL(%rip), %rdi
	salq	$5, %rdx
	addq	%rdx, %r13
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	(%rdi)
	movq	RB_long@GOTPCREL(%rip), %rax
	movl	$0, 0(%r13)
	movl	$0, 72(%r13)
	subl	$1, (%rax)
	movl	%edx, (%rbx)
	jmp	.L47
	.p2align 4,,7
	.p2align 3
.L55:
	cmpl	$255, %eax
	jne	.L56
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rax
	movl	$1, (%rax)
	movq	presentacion@GOTPCREL(%rip), %rax
	movq	64(%rax), %r8
.L73:
	leaq	(%rcx,%rcx,2), %rax
	movl	$1, %edx
	leaq	.LC7(%rip), %rdi
	salq	$5, %rax
	addq	%r13, %rax
	movl	92(%rax), %esi
	movl	$1, 4(%rax)
	call	*%r8
	jmp	.L75
.L57:
	leaq	(%rcx,%rcx,2), %rax
	movq	%r14, %r9
	leaq	.LC5(%rip), %rdx
	salq	$5, %rax
	movq	80(%r13,%rax), %r8
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
.L145:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movslq	(%rbx), %rcx
	movl	$1, 0(%rbp)
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	movl	72(%r13,%rax), %edx
.L64:
	movq	presentacion@GOTPCREL(%rip), %rax
	testl	%edx, %edx
	movq	64(%rax), %r8
	je	.L74
	jmp	.L73
.L59:
	leaq	(%rcx,%rcx,2), %rax
	movq	%r14, %r9
	leaq	.LC4(%rip), %rdx
	salq	$5, %rax
	movq	80(%r13,%rax), %r8
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
.L150:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movslq	(%rbx), %rcx
	movl	$1, 0(%rbp)
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	addq	%r13, %rax
	movl	4(%rax), %esi
	leal	-65(%rsi), %edx
	andl	$-65, %edx
	je	.L117
	leal	-16(%rsi), %edx
	cmpl	$2, %edx
	ja	.L69
.L117:
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	movl	72(%r13,%rax), %edx
.L71:
	movq	RS@GOTPCREL(%rip), %r9
	leaq	(%rcx,%rcx,2), %rax
	salq	$5, %rax
	movq	16(%r13,%rax), %rax
	leaq	(%rax,%rax,4), %rsi
	leaq	(%rax,%rsi,4), %rax
	leaq	(%r9,%rax,8), %rax
	movl	$0, (%rax)
	jmp	.L64
.L60:
	leaq	(%rcx,%rcx,2), %rax
	movq	%r14, %r9
	leaq	.LC3(%rip), %rdx
	salq	$5, %rax
	movq	80(%r13,%rax), %r8
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	jmp	.L150
.L61:
	leaq	(%rcx,%rcx,2), %rax
	movq	%r14, %r9
	leaq	.LC2(%rip), %rdx
	salq	$5, %rax
	movq	80(%r13,%rax), %r8
	movq	Ciclo@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	jmp	.L145
.L56:
	imulq	$96, %rcx, %rcx
	movq	Ciclo@GOTPCREL(%rip), %rax
	movq	%r14, %r9
	leaq	.LC6(%rip), %rdx
	movl	$1, %esi
	movq	80(%r13,%rcx), %r8
	movl	(%rax), %ecx
	movq	stderr@GOTPCREL(%rip), %rax
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
.L76:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	$476, %r8d
	leaq	.LC8(%rip), %rcx
	leaq	.LC9(%rip), %rdx
	movl	$1, %esi
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L85:
	movq	Rint@GOTPCREL(%rip), %rcx
	leaq	(%rdx,%rdx,2), %rsi
	salq	$5, %rsi
	addq	%r13, %rsi
	movq	16(%rsi), %rdi
	testq	%rdi, %rdi
	je	.L95
	movq	24(%rsi), %rsi
	movq	%rdi, %r8
	salq	$4, %r8
	movq	%rsi, (%rcx,%r8)
.L95:
	movq	%rdi, %rsi
	salq	$4, %rsi
	addq	%rsi, %rcx
	cmpl	8(%rcx), %eax
	jne	.L96
	movl	$4096, 8(%rcx)
.L96:
	movq	Rint_tipo@GOTPCREL(%rip), %rcx
	movl	$5, (%rcx,%rdi,4)
	jmp	.L87
	.p2align 4,,7
	.p2align 3
.L81:
	leaq	(%rdx,%rdx,2), %rsi
	salq	$5, %rsi
	addq	%r13, %rsi
	movq	16(%rsi), %rcx
	movq	24(%rsi), %rsi
	salq	$4, %rcx
	addq	Rint@GOTPCREL(%rip), %rcx
	movq	%rsi, (%rcx)
	cmpl	8(%rcx), %edx
	jne	.L107
	movl	$4096, 8(%rcx)
	.p2align 4
	.p2align 3
.L107:
	addq	$1, 16(%r12)
	jmp	.L87
.L83:
	subl	$110, %r9d
	cmpl	$1, %r9d
	jbe	.L155
.L90:
	leaq	(%rdx,%rdx,2), %rsi
	salq	$5, %rsi
	addq	%r13, %rsi
	movl	72(%rsi), %r8d
	testl	%r8d, %r8d
	je	.L156
.L92:
	addq	$1, 40(%r12)
	call	prediccion_correcta@PLT
	testl	%eax, %eax
	je	.L93
	movslq	(%rbx), %rdx
	addq	$1, 48(%r12)
	movq	%rdx, %rax
	jmp	.L87
.L82:
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	addq	%r13, %rcx
	movq	16(%rcx), %rdi
	movq	24(%rcx), %rcx
	movq	%rdi, %rsi
	salq	$4, %rsi
	addq	Rfp@GOTPCREL(%rip), %rsi
	movq	%rcx, (%rsi)
	cmpl	8(%rsi), %edx
	jne	.L105
	movl	$4096, 8(%rsi)
.L105:
	leal	-141(%r9), %ecx
	cmpl	$5, %ecx
	jbe	.L157
	leal	-130(%r9), %ecx
	cmpl	$3, %ecx
	ja	.L111
.L110:
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	movl	$6, (%rcx,%rdi,4)
	jmp	.L107
.L79:
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	movq	16(%r13,%rcx), %rcx
	leaq	(%rcx,%rcx,4), %rsi
	leaq	(%rcx,%rsi,4), %rcx
	movq	RS@GOTPCREL(%rip), %rsi
	leaq	(%rsi,%rcx,8), %rcx
	movl	$1, 120(%rcx)
	jmp	.L87
.L80:
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	addq	%r13, %rcx
	movq	16(%rcx), %rdi
	movq	24(%rcx), %rcx
	movq	%rdi, %rsi
	salq	$4, %rsi
	addq	Rfp@GOTPCREL(%rip), %rsi
	movq	%rcx, (%rsi)
	cmpl	8(%rsi), %edx
	jne	.L108
	movl	$4096, 8(%rsi)
.L108:
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	addq	$2, 16(%r12)
	movl	$7, (%rcx,%rdi,4)
	jmp	.L87
.L84:
	movq	Rint@GOTPCREL(%rip), %rcx
	leaq	(%rdx,%rdx,2), %rsi
	salq	$5, %rsi
	addq	%r13, %rsi
	movq	16(%rsi), %rdi
	cmpl	$1, 40(%rsi)
	je	.L142
.L97:
	salq	$4, %rdi
	addq	%rdi, %rcx
	cmpl	8(%rcx), %eax
	jne	.L87
	movl	$4096, 8(%rcx)
	jmp	.L87
.L86:
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rcx
	movl	(%rcx), %esi
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rcx
	addl	(%rcx), %esi
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rcx
	addl	(%rcx), %esi
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rcx
	addl	(%rcx), %esi
	movq	TAM_BUFFER_ALMACEN@GOTPCREL(%rip), %rcx
	movl	(%rcx), %edi
	leal	(%rdi,%rsi), %ecx
	cmpl	%esi, %ecx
	jle	.L88
	movslq	%esi, %rsi
	leal	-1(%rdi), %edi
	movq	RS@GOTPCREL(%rip), %r8
	leaq	(%rsi,%rsi,4), %rcx
	leaq	(%rsi,%rcx,4), %rcx
	addq	%rdi, %rsi
	leaq	(%rsi,%rsi,4), %rdi
	leaq	(%rsi,%rdi,4), %rsi
	leaq	(%r8,%rcx,8), %rcx
	leaq	168(%r8,%rsi,8), %rdi
	jmp	.L89
	.p2align 4,,7
	.p2align 3
.L158:
	addq	$168, %rcx
	cmpq	%rcx, %rdi
	je	.L88
.L89:
	movl	(%rcx), %esi
	testl	%esi, 120(%rcx)
	je	.L158
	jmp	.L87
	.p2align 4,,7
	.p2align 3
.L52:
	subl	$129, %esi
	cmpl	$52, %esi
	ja	.L54
.L112:
	movabsq	$4503599627378689, %rax
	btq	%rsi, %rax
	jnc	.L54
	movq	RS@GOTPCREL(%rip), %rsi
	leaq	(%rdx,%rdx,2), %rax
	salq	$5, %rax
	movq	16(%r13,%rax), %rax
	jmp	.L53
	.p2align 4,,7
	.p2align 3
.L154:
	subl	$129, %esi
	cmpl	$52, %esi
	ja	.L47
	movabsq	$4503599627378689, %rcx
	btq	%rsi, %rcx
	jnc	.L47
	movq	16(%rax), %rax
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rax
	movq	RS@GOTPCREL(%rip), %rcx
	leaq	(%rcx,%rax,8), %rax
	cmpl	$1, 124(%rax)
	jne	.L47
	movq	estat@GOTPCREL(%rip), %r12
	addq	$1, 8(%r12)
	jmp	.L112
.L157:
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	movl	$8, (%rcx,%rdi,4)
.L111:
	subl	$135, %r9d
	cmpl	$5, %r9d
	ja	.L107
	jmp	.L110
	.p2align 4,,7
	.p2align 3
.L69:
	movl	72(%rax), %edx
	cmpl	$142, %esi
	je	.L71
	cmpl	$181, %esi
	je	.L71
	jmp	.L64
.L100:
	movabsq	$4398046519231, %rsi
	btq	%rcx, %rsi
	jnc	.L102
	movq	Rfp_tipo@GOTPCREL(%rip), %rcx
	movl	$8, (%rcx,%rdi,4)
.L102:
	cmpl	$183, %r9d
	leal	-128(%r9), %ecx
	je	.L87
	jmp	.L101
	.p2align 4,,7
	.p2align 3
.L155:
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	addq	%r13, %rcx
	movq	16(%rcx), %rdi
	movq	24(%rcx), %rcx
	movq	%rdi, %rsi
	salq	$4, %rsi
	addq	Rint@GOTPCREL(%rip), %rsi
	movq	%rcx, (%rsi)
	cmpl	8(%rsi), %edx
	jne	.L91
	movl	$4096, 8(%rsi)
.L91:
	movq	Rint_tipo@GOTPCREL(%rip), %rcx
	movl	$5, (%rcx,%rdi,4)
	jmp	.L90
.L142:
	testq	%rdi, %rdi
	je	.L97
	movq	24(%rsi), %rsi
	movq	%rdi, %r8
	salq	$4, %r8
	movq	%rsi, (%rcx,%r8)
	jmp	.L97
.L93:
	movslq	(%rbx), %rax
	movl	$1, %esi
	leaq	(%rax,%rax,2), %rax
	salq	$5, %rax
	addq	%rax, %r13
	movl	52(%r13), %eax
	movl	40(%r13), %edi
	leal	(%rax,%rax), %edx
	movq	n_bits_branch_mask_local@GOTPCREL(%rip), %rax
	orl	%edi, %edx
	movl	(%rax), %ecx
	movl	%esi, %eax
	sall	%cl, %eax
	movl	%eax, %ecx
	movq	branch_mask_local@GOTPCREL(%rip), %rax
	subl	$1, %ecx
	andl	%ecx, %edx
	movl	%edx, (%rax)
	movq	n_bits_branch_mask_global@GOTPCREL(%rip), %rdx
	movl	48(%r13), %eax
	movl	(%rdx), %ecx
	addl	%eax, %eax
	sall	%cl, %esi
	orl	%edi, %eax
	leal	-1(%rsi), %edx
	andl	%edx, %eax
	movq	branch_mask_global@GOTPCREL(%rip), %rdx
	movl	%eax, (%rdx)
	movq	parar_clarividente@GOTPCREL(%rip), %rax
	movl	(%rax), %edi
	testl	%edi, %edi
	je	.L94
	movq	orden_clarividente@GOTPCREL(%rip), %rdx
	movl	(%rdx), %edi
	cmpl	%edi, 92(%r13)
	jne	.L94
	movl	$0, (%rax)
.L94:
	call	cancelar_instrucciones@PLT
	jmp	.L47
.L156:
	movq	32(%rsi), %r8
	movl	%eax, %edi
	movl	40(%rsi), %ecx
	movl	92(%rsi), %edx
	movq	80(%rsi), %rsi
	call	actualizar_prediccion@PLT
	jmp	.L92
.L88:
	leaq	(%rdx,%rdx,2), %rdx
	movl	%eax, %edi
	movl	$1, %ecx
	salq	$5, %rdx
	addq	%r13, %rdx
	movq	80(%rdx), %rsi
	movl	92(%rdx), %edx
	leaq	4(%rsi), %r8
	call	actualizar_prediccion@PLT
	call	cancelar_instrucciones@PLT
	movq	SIG_SYSCALL@GOTPCREL(%rip), %rax
	cmpl	$1, (%rax)
	je	.L109
	movslq	(%rbx), %rdx
	movq	%rdx, %rax
	jmp	.L87
.L152:
	call	__stack_chk_fail@PLT
.L109:
	movl	$0, (%rax)
	call	process_syscall@PLT
	movl	%eax, 0(%rbp)
	jmp	.L47
	.cfi_endproc
.LFE46:
	.size	fase_COM, .-fase_COM
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
