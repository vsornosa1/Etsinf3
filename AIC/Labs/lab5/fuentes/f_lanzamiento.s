	.file	"f_lanzamiento.c"
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"X"
.LC1:
	.string	"i"
.LC2:
	.string	"I"
.LC3:
	.string	"%s"
.LC4:
	.string	"f_lanzamiento.c"
	.section	.rodata.str1.8,"aMS",@progbits,1
	.align 8
.LC5:
	.string	"ERROR (%s:%d): Operacion %d no implementada\n"
	.text
	.p2align 4
	.globl	fase_ISS
	.type	fase_ISS, @function
fase_ISS:
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
	leaq	-40(%rsp), %rsp
	.cfi_def_cfa_offset 96
	movq	IF_ISS_2@GOTPCREL(%rip), %rbx
	movl	(%rbx), %eax
	leal	-141(%rax), %edx
	cmpl	$5, %edx
	jbe	.L160
	movq	PC_ISS@GOTPCREL(%rip), %rdx
	movq	160(%rbx), %rcx
	movq	%rcx, (%rdx)
	movq	Control_2@GOTPCREL(%rip), %rdx
	movl	12(%rdx), %edx
	orl	208(%rbx), %edx
	jne	.L161
	movq	Control_1@GOTPCREL(%rip), %rbp
	movl	12(%rbp), %edx
	orl	204(%rbx), %edx
	jne	.L162
	movl	8(%rbp), %edx
	orl	16(%rbp), %edx
	jne	.L163
	testl	%eax, %eax
	je	.L7
	movq	presentacion@GOTPCREL(%rip), %rdx
	cmpl	$1, %eax
	movl	168(%rbx), %esi
	movq	64(%rdx), %rcx
	je	.L164
	xorl	%edx, %edx
	leaq	.LC2(%rip), %rdi
	call	*%rcx
.L7:
	movq	RB_long@GOTPCREL(%rip), %r15
	movq	TAM_REORDER@GOTPCREL(%rip), %rax
	movl	$1, 8(%rbp)
	movl	(%r15), %ecx
	movl	(%rax), %esi
	cmpl	%esi, %ecx
	jge	.L1
	movq	RB_fin@GOTPCREL(%rip), %rax
	movq	RB@GOTPCREL(%rip), %r14
	movl	172(%rbx), %edx
	movslq	(%rax), %r13
	movl	200(%rbx), %edi
	movdqu	176(%rbx), %xmm0
	leaq	0(%r13,%r13,2), %rax
	movq	%r13, %r12
	salq	$5, %rax
	addq	%r14, %rax
	movl	%edx, 44(%rax)
	movq	192(%rbx), %rdx
	movl	%edi, 72(%rax)
	movdqu	%xmm0, 48(%rax)
	movq	%rdx, 64(%rax)
	movl	(%rbx), %edx
	cmpl	$183, %edx
	ja	.L10
	leaq	.L12(%rip), %r9
	movl	%edx, %r8d
	movslq	(%r9,%r8,4), %rax
	addq	%r9, %rax
	notrack jmp	*%rax
	.section	.rodata
	.align 4
	.align 4
.L12:
	.long	.L25-.L12
	.long	.L24-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L14-.L12
	.long	.L14-.L12
	.long	.L14-.L12
	.long	.L14-.L12
	.long	.L14-.L12
	.long	.L14-.L12
	.long	.L23-.L12
	.long	.L23-.L12
	.long	.L23-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L14-.L12
	.long	.L23-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L10-.L12
	.long	.L19-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L22-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L19-.L12
	.long	.L22-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L20-.L12
	.long	.L20-.L12
	.long	.L21-.L12
	.long	.L21-.L12
	.long	.L20-.L12
	.long	.L20-.L12
	.long	.L19-.L12
	.long	.L19-.L12
	.long	.L18-.L12
	.long	.L17-.L12
	.long	.L18-.L12
	.long	.L17-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L14-.L12
	.long	.L13-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L15-.L12
	.long	.L15-.L12
	.long	.L10-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L14-.L12
	.long	.L13-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L15-.L12
	.long	.L15-.L12
	.long	.L10-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L16-.L12
	.long	.L15-.L12
	.long	.L15-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L10-.L12
	.long	.L14-.L12
	.long	.L13-.L12
	.long	.L11-.L12
	.long	.L11-.L12
	.text
	.p2align 4,,7
	.p2align 3
.L161:
	movq	Control_1@GOTPCREL(%rip), %rax
	movl	$0, 8(%rax)
.L1:
	leaq	40(%rsp), %rsp
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
.L160:
	.cfi_restore_state
	leaq	40(%rsp), %rsp
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
	jmp	fase_ISS_alum@PLT
	.p2align 4,,7
	.p2align 3
.L162:
	.cfi_restore_state
	movq	presentacion@GOTPCREL(%rip), %rax
	xorl	%edx, %edx
	movl	168(%rbx), %esi
	leaq	.LC0(%rip), %rdi
	movq	64(%rax), %rax
	leaq	40(%rsp), %rsp
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
	jmp	*%rax
	.p2align 4,,7
	.p2align 3
.L163:
	.cfi_restore_state
	movl	168(%rbx), %esi
	xorl	%edx, %edx
	movq	presentacion@GOTPCREL(%rip), %rax
	leaq	.LC1(%rip), %rdi
	call	*64(%rax)
	movl	$1, 8(%rbp)
	jmp	.L1
.L10:
	movq	stderr@GOTPCREL(%rip), %rax
	movl	%edx, %r9d
	movl	$960, %r8d
	leaq	.LC4(%rip), %rcx
	movl	$1, %esi
	leaq	.LC5(%rip), %rdx
	movq	(%rax), %rdi
	xorl	%eax, %eax
	call	__fprintf_chk@PLT
	movl	$1, %edi
	call	exit@PLT
	.p2align 4,,7
	.p2align 3
.L16:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rdi
	movl	(%rax), %eax
	movl	(%rdi), %r8d
	addl	%eax, %r8d
	cmpl	%r8d, %eax
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%eax, %rdi
	leaq	(%rdi,%rdi,4), %r9
	leaq	(%rdi,%r9,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	jmp	.L87
	.p2align 4,,7
	.p2align 3
.L165:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L87:
	movl	(%rdi), %r11d
	testl	%r11d, %r11d
	jne	.L165
	movslq	8(%rbx), %r8
	cltq
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	salq	$4, %r8
	leaq	(%r10,%rdi,8), %rdi
	movl	$1, (%rdi)
	movl	%edx, 4(%rdi)
	movl	%r12d, 148(%rdi)
	movq	Rfp@GOTPCREL(%rip), %rdi
	leaq	(%rdi,%r8), %r9
	movl	8(%r9), %r8d
	cmpl	$4096, %r8d
	je	.L166
	movslq	%r8d, %r11
	leaq	(%r11,%r11,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L89
	imulq	$96, %r11, %r11
	imulq	$168, %rax, %r8
	movq	24(%r14,%r11), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
.L88:
	movslq	12(%rbx), %r9
	salq	$4, %r9
	addq	%rdi, %r9
	movl	8(%r9), %r8d
	cmpl	$4096, %r8d
	je	.L167
	movslq	%r8d, %r11
	leaq	(%r11,%r11,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L92
	imulq	$96, %r11, %r11
	imulq	$168, %rax, %r8
	movq	24(%r14,%r11), %r9
	addq	%r10, %r8
	movq	%r9, 32(%r8)
	movl	$4096, 24(%r8)
.L91:
	movslq	16(%rbx), %r9
	leaq	0(%r13,%r13,2), %r8
	salq	$5, %r8
	addq	%r14, %r8
	movl	$1, (%r8)
	movl	%edx, 4(%r8)
	movq	%r9, 16(%r8)
	movl	$0, 8(%r8)
	leal	-135(%rdx), %r8d
	cmpl	$5, %r8d
	jbe	.L126
	subl	$148, %edx
	cmpl	$5, %edx
	jbe	.L126
	salq	$4, %r9
	movl	%r12d, 8(%rdi,%r9)
.L95:
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rax
	movl	168(%rbx), %edx
	leaq	(%r10,%rax,8), %rax
	movl	$0, 124(%rax)
	movl	%edx, 160(%rax)
	leaq	0(%r13,%r13,2), %rax
	salq	$5, %rax
	addq	%rax, %r14
	movq	160(%rbx), %rax
	movl	%edx, 92(%r14)
	movq	%rax, 80(%r14)
	jmp	.L33
.L19:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%eax, %eax
	movq	%r10, %rdi
	jmp	.L72
	.p2align 4,,7
	.p2align 3
.L168:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L72:
	movl	(%rdi), %r9d
	testl	%r9d, %r9d
	jne	.L168
	cltq
	leaq	0(%r13,%r13,2), %r8
	movl	168(%rbx), %r9d
	leaq	(%rax,%rax,4), %rdi
	salq	$5, %r8
	leaq	(%rax,%rdi,4), %rdi
	addq	%r14, %r8
	leaq	(%r10,%rdi,8), %rdi
	movl	%edx, 4(%rdi)
	movl	%edx, 4(%r8)
	movq	160(%rbx), %rdx
	movl	%r9d, 92(%r8)
	movl	$1, (%r8)
	movq	%rdx, 80(%r8)
	movslq	16(%rbx), %rdx
	movl	$0, 8(%r8)
	movl	%r9d, 160(%rdi)
	movq	%rdx, 16(%r8)
	movslq	8(%rbx), %r8
	movq	Rint@GOTPCREL(%rip), %r9
	movl	$1, (%rdi)
	salq	$4, %r8
	movl	$0, 124(%rdi)
	leaq	(%r9,%r8), %r11
	movl	8(%r11), %r8d
	movl	%r12d, 148(%rdi)
	movq	%rdx, %rdi
	cmpl	$4096, %r8d
	je	.L169
	movslq	%r8d, %r13
	leaq	0(%r13,%r13,2), %r11
	salq	$5, %r11
	movl	8(%r14,%r11), %r11d
	testl	%r11d, %r11d
	je	.L74
	imulq	$96, %r13, %r13
	imulq	$168, %rax, %r8
	movq	24(%r14,%r13), %r11
	addq	%r10, %r8
	movq	%r11, 16(%r8)
	movl	$4096, 8(%r8)
.L73:
	movslq	12(%rbx), %r8
	salq	$4, %r8
	addq	%r9, %r8
	movl	8(%r8), %r11d
	cmpl	$4096, %r11d
	je	.L170
	movslq	%r11d, %r8
	imulq	$168, %rax, %rax
	leaq	(%r8,%r8,2), %rbx
	salq	$5, %rbx
	movl	8(%r14,%rbx), %ebx
	testl	%ebx, %ebx
	je	.L77
	imulq	$96, %r8, %r8
	addq	%rax, %r10
	movl	$4096, 24(%r10)
	movq	24(%r14,%r8), %rax
	movq	%rax, 32(%r10)
.L76:
	testl	%edi, %edi
	je	.L33
	salq	$4, %rdx
	movl	%r12d, 8(%r9,%rdx)
	jmp	.L33
.L22:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%eax, %eax
	movq	%r10, %rdi
	jmp	.L79
	.p2align 4,,7
	.p2align 3
.L171:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L79:
	movl	(%rdi), %r9d
	testl	%r9d, %r9d
	jne	.L171
	movl	168(%rbx), %r8d
	cltq
	movq	Rint@GOTPCREL(%rip), %r9
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	movl	%edx, 4(%rdi)
	movl	$1, (%rdi)
	movl	%r8d, 160(%rdi)
	movl	$0, 124(%rdi)
	movl	%r12d, 148(%rdi)
	leaq	0(%r13,%r13,2), %rdi
	salq	$5, %rdi
	addq	%r14, %rdi
	movl	%edx, 4(%rdi)
	movq	160(%rbx), %rdx
	movl	%r8d, 92(%rdi)
	movslq	16(%rbx), %r8
	movq	%rdx, 80(%rdi)
	movslq	8(%rbx), %rdx
	movl	$1, (%rdi)
	movl	$0, 8(%rdi)
	salq	$4, %rdx
	movq	%r8, 16(%rdi)
	addq	%r9, %rdx
	movl	8(%rdx), %edi
	cmpl	$4096, %edi
	je	.L172
	movslq	%edi, %rdx
	leaq	(%rdx,%rdx,2), %r11
	salq	$5, %r11
	movl	8(%r14,%r11), %r13d
	testl	%r13d, %r13d
	je	.L81
	imulq	$96, %rdx, %rdx
	imulq	$168, %rax, %rdi
	movq	24(%r14,%rdx), %rdx
	addq	%r10, %rdi
	movq	%rdx, 16(%rdi)
	movl	$4096, 8(%rdi)
.L80:
	salq	$4, %r8
	leaq	(%rax,%rax,4), %rdx
	movl	%r12d, 8(%r9,%r8)
	leaq	(%rax,%rdx,4), %rax
	movswq	20(%rbx), %rdx
	leaq	(%r10,%rax,8), %rax
	movl	$4096, 24(%rax)
	movq	%rdx, 32(%rax)
	jmp	.L33
.L14:
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rcx
	movl	(%rax), %eax
	addl	(%rcx), %eax
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rcx
	addl	(%rcx), %eax
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rcx
	movl	(%rcx), %esi
	addl	%eax, %esi
	cmpl	%esi, %eax
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%eax, %rcx
	leaq	(%rcx,%rcx,4), %rdi
	leaq	(%rcx,%rdi,4), %rcx
	leaq	(%r10,%rcx,8), %rcx
	jmp	.L52
	.p2align 4,,7
	.p2align 3
.L173:
	addl	$1, %eax
	addq	$168, %rcx
	cmpl	%esi, %eax
	je	.L1
.L52:
	movl	(%rcx), %r8d
	testl	%r8d, %r8d
	jne	.L173
	movq	Rint@GOTPCREL(%rip), %r9
	cltq
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rcx
	leaq	(%r10,%rcx,8), %rcx
	movl	$1, (%rcx)
	movl	%edx, 4(%rcx)
	movl	%r12d, 148(%rcx)
	movslq	8(%rbx), %rcx
	salq	$4, %rcx
	addq	%r9, %rcx
	movl	8(%rcx), %esi
	cmpl	$4096, %esi
	je	.L174
	movslq	%esi, %rcx
	leaq	(%rcx,%rcx,2), %rdi
	salq	$5, %rdi
	movl	8(%r14,%rdi), %edi
	testl	%edi, %edi
	je	.L54
	imulq	$96, %rcx, %rcx
	imulq	$168, %rax, %rsi
	movq	24(%r14,%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 16(%rsi)
	movl	$4096, 8(%rsi)
.L53:
	movswq	20(%rbx), %rsi
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rcx
	leaq	(%r10,%rcx,8), %rcx
	movq	%rsi, 48(%rcx)
	movl	$4096, 24(%rcx)
	leaq	0(%r13,%r13,2), %rcx
	movslq	16(%rbx), %rsi
	salq	$5, %rcx
	addq	%r14, %rcx
	movq	%rsi, 16(%rcx)
	salq	$4, %rsi
	cmpl	$127, %edx
	movl	$1, (%rcx)
	movl	%edx, 4(%rcx)
	movl	$0, 8(%rcx)
	jg	.L55
	movl	%r12d, 8(%r9,%rsi)
.L56:
	leaq	(%rax,%rax,4), %rdx
	leaq	32(%rbx), %r8
	leaq	(%rax,%rdx,4), %rax
	movl	$1, %esi
	movl	168(%rbx), %ecx
	salq	$3, %rax
	leaq	(%r10,%rax), %rdx
	leaq	56(%r10,%rax), %rdi
	movl	%ecx, 160(%rdx)
	xorl	%eax, %eax
	movl	$0, 124(%rdx)
	leaq	.LC3(%rip), %rcx
	movl	$64, %edx
	call	__sprintf_chk@PLT
	leaq	0(%r13,%r13,2), %rax
	salq	$5, %rax
	addq	%rax, %r14
	movl	168(%rbx), %eax
	movl	%eax, 92(%r14)
	movq	160(%rbx), %rax
	movq	%rax, 80(%r14)
.L158:
	movq	RB_fin@GOTPCREL(%rip), %rax
	movl	(%r15), %ecx
	movl	(%rax), %r12d
	movq	TAM_REORDER@GOTPCREL(%rip), %rax
	movl	(%rax), %esi
.L33:
	leal	1(%r12), %eax
	addl	$1, %ecx
	movl	$0, 8(%rbp)
	movl	%eax, %edx
	sarl	$31, %edx
	idivl	%esi
	movq	RB_fin@GOTPCREL(%rip), %rax
	movl	%ecx, (%r15)
	movl	%edx, (%rax)
	jmp	.L1
.L15:
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rdi
	movl	(%rax), %eax
	addl	(%rdi), %eax
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rdi
	movl	(%rdi), %r8d
	addl	%eax, %r8d
	cmpl	%r8d, %eax
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%eax, %rdi
	leaq	(%rdi,%rdi,4), %r9
	leaq	(%rdi,%r9,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	jmp	.L97
	.p2align 4,,7
	.p2align 3
.L175:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L97:
	movl	(%rdi), %r11d
	testl	%r11d, %r11d
	jne	.L175
	movslq	8(%rbx), %r8
	cltq
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	salq	$4, %r8
	leaq	(%r10,%rdi,8), %rdi
	movl	$1, (%rdi)
	movl	%edx, 4(%rdi)
	movl	%r12d, 148(%rdi)
	movq	Rfp@GOTPCREL(%rip), %rdi
	leaq	(%rdi,%r8), %r9
	movl	8(%r9), %r8d
	cmpl	$4096, %r8d
	je	.L176
	movslq	%r8d, %r11
	leaq	(%r11,%r11,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L99
	imulq	$96, %r11, %r11
	imulq	$168, %rax, %r8
	movq	24(%r14,%r11), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
.L98:
	movslq	12(%rbx), %r9
	salq	$4, %r9
	addq	%rdi, %r9
	movl	8(%r9), %r8d
	cmpl	$4096, %r8d
	je	.L177
	movslq	%r8d, %r11
	leaq	(%r11,%r11,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L102
	imulq	$96, %r11, %r11
	imulq	$168, %rax, %r8
	movq	24(%r14,%r11), %r9
	addq	%r10, %r8
	movq	%r9, 32(%r8)
	movl	$4096, 24(%r8)
.L101:
	leaq	0(%r13,%r13,2), %r8
	salq	$5, %r8
	addq	%r8, %r14
	movl	%edx, 4(%r14)
	movslq	16(%rbx), %rdx
	movl	$1, (%r14)
	movl	$0, 8(%r14)
	movq	%rdx, 16(%r14)
	salq	$4, %rdx
	movl	%r12d, 8(%rdi,%rdx)
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rax
	leaq	(%r10,%rax,8), %rdx
	movl	168(%rbx), %eax
	movl	$0, 124(%rdx)
	movl	%eax, 160(%rdx)
	movl	%eax, 92(%r14)
	movq	160(%rbx), %rax
	movq	%rax, 80(%r14)
	jmp	.L33
.L23:
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movl	(%rax), %r11d
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	addl	(%rax), %r11d
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rax
	addl	(%rax), %r11d
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rax
	addl	(%rax), %r11d
	movq	TAM_BUFFER_ALMACEN@GOTPCREL(%rip), %rax
	movl	(%rax), %ecx
	addl	%r11d, %ecx
	cmpl	%ecx, %r11d
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%r11d, %rax
	leaq	(%rax,%rax,4), %rsi
	leaq	(%rax,%rsi,4), %rax
	leaq	(%r10,%rax,8), %rax
	jmp	.L58
	.p2align 4,,7
	.p2align 3
.L178:
	addl	$1, %r11d
	addq	$168, %rax
	cmpl	%ecx, %r11d
	je	.L1
.L58:
	movl	(%rax), %esi
	testl	%esi, %esi
	jne	.L178
	movl	168(%rbx), %ecx
	movslq	%r11d, %r11
	movq	Rint@GOTPCREL(%rip), %r9
	leaq	(%r11,%r11,4), %rax
	leaq	(%r11,%rax,4), %rax
	leaq	(%r10,%rax,8), %rax
	movl	%edx, 4(%rax)
	movl	$1, (%rax)
	movl	$0, 124(%rax)
	movl	%ecx, 160(%rax)
	movl	%r12d, 148(%rax)
	leaq	0(%r13,%r13,2), %rax
	salq	$5, %rax
	addq	%r14, %rax
	movl	%edx, 4(%rax)
	movq	160(%rbx), %rdx
	movl	$0, 8(%rax)
	movl	$1, (%rax)
	movl	%ecx, 92(%rax)
	movq	%r11, 16(%rax)
	movq	%rdx, 80(%rax)
	movslq	8(%rbx), %rax
	salq	$4, %rax
	addq	%r9, %rax
	movl	8(%rax), %edx
	cmpl	$4096, %edx
	je	.L179
	movslq	%edx, %rax
	leaq	(%rax,%rax,2), %rcx
	salq	$5, %rcx
	movl	8(%r14,%rcx), %r12d
	testl	%r12d, %r12d
	je	.L60
	imulq	$96, %rax, %rax
	imulq	$168, %r11, %rdx
	movq	24(%r14,%rax), %rax
	addq	%r10, %rdx
	movq	%rax, 16(%rdx)
	movl	$4096, 8(%rdx)
.L59:
	leaq	(%r11,%r11,4), %rax
	leaq	32(%rbx), %r8
	movswq	20(%rbx), %rdx
	leaq	(%r11,%rax,4), %rax
	leaq	.LC3(%rip), %rcx
	movl	$1, %esi
	salq	$3, %rax
	movq	%rdx, 48(%r10,%rax)
	leaq	56(%r10,%rax), %rdi
	movl	$64, %edx
	xorl	%eax, %eax
	movq	%r9, 24(%rsp)
	movq	%r11, 16(%rsp)
	movq	%r10, 8(%rsp)
	call	__sprintf_chk@PLT
	movslq	12(%rbx), %rax
	movq	24(%rsp), %r9
	movq	8(%rsp), %r10
	salq	$4, %rax
	movq	16(%rsp), %r11
	addq	%rax, %r9
	movl	8(%r9), %eax
	cmpl	$4096, %eax
	je	.L180
	movslq	%eax, %rdx
	leaq	(%rdx,%rdx,2), %rcx
	salq	$5, %rcx
	movl	8(%r14,%rcx), %r9d
	testl	%r9d, %r9d
	je	.L63
	imulq	$96, %rdx, %rdx
	imulq	$168, %r11, %rax
	movq	24(%r14,%rdx), %rdx
	addq	%r10, %rax
	movq	%rdx, 32(%rax)
	movl	$4096, 24(%rax)
.L62:
	leaq	(%r11,%r11,4), %rax
	leaq	(%r11,%rax,4), %rax
	movl	$0, 120(%r10,%rax,8)
	jmp	.L158
.L20:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%eax, %eax
	movq	%r10, %rdi
	jmp	.L41
	.p2align 4,,7
	.p2align 3
.L181:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L41:
	movl	(%rdi), %r9d
	testl	%r9d, %r9d
	jne	.L181
	movl	168(%rbx), %r8d
	cltq
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	movl	%edx, 4(%rdi)
	movl	$1, (%rdi)
	movl	%r8d, 160(%rdi)
	movl	$0, 124(%rdi)
	movl	%r12d, 148(%rdi)
	leaq	0(%r13,%r13,2), %rdi
	salq	$5, %rdi
	addq	%r14, %rdi
	movl	%edx, 4(%rdi)
	movl	%r8d, 92(%rdi)
	movq	160(%rbx), %rdx
	movswq	20(%rbx), %r8
	movl	$1, (%rdi)
	movq	%rdx, 80(%rdi)
	movl	$0, 8(%rdi)
	leaq	4(%rdx,%r8), %rdx
	movq	%rdx, 32(%rdi)
	movslq	8(%rbx), %rdx
	salq	$4, %rdx
	addq	Rint@GOTPCREL(%rip), %rdx
	movl	8(%rdx), %edi
	cmpl	$4096, %edi
	je	.L182
	movslq	%edi, %rdx
	leaq	(%rdx,%rdx,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r11d
	testl	%r11d, %r11d
	je	.L43
	imulq	$96, %rdx, %rdx
	imulq	$168, %rax, %rdi
	movq	24(%r14,%rdx), %rdx
	addq	%r10, %rdi
	movq	%rdx, 16(%rdi)
	movl	$4096, 8(%rdi)
.L42:
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rax
	leaq	(%r10,%rax,8), %rax
	movq	$0, 32(%rax)
	movl	$4096, 24(%rax)
	movq	%r8, 48(%rax)
	jmp	.L33
.L13:
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rax
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rcx
	movl	(%rax), %eax
	addl	(%rcx), %eax
	movq	TAM_RS_MULTDIV@GOTPCREL(%rip), %rcx
	addl	(%rcx), %eax
	movq	TAM_BUFFER_CARGA@GOTPCREL(%rip), %rcx
	addl	(%rcx), %eax
	movq	TAM_BUFFER_ALMACEN@GOTPCREL(%rip), %rcx
	movl	(%rcx), %esi
	addl	%eax, %esi
	cmpl	%esi, %eax
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%eax, %rcx
	leaq	(%rcx,%rcx,4), %rdi
	leaq	(%rcx,%rdi,4), %rcx
	leaq	(%r10,%rcx,8), %rcx
	jmp	.L65
	.p2align 4,,7
	.p2align 3
.L183:
	addl	$1, %eax
	addq	$168, %rcx
	cmpl	%esi, %eax
	je	.L1
.L65:
	movl	(%rcx), %r8d
	testl	%r8d, %r8d
	jne	.L183
	cltq
	leaq	(%rax,%rax,4), %rcx
	leaq	(%rax,%rcx,4), %rcx
	leaq	(%r10,%rcx,8), %rcx
	movl	$1, (%rcx)
	movl	%edx, 4(%rcx)
	movslq	8(%rbx), %rcx
	salq	$4, %rcx
	addq	Rint@GOTPCREL(%rip), %rcx
	movl	8(%rcx), %esi
	cmpl	$4096, %esi
	je	.L184
	movslq	%esi, %rcx
	leaq	(%rcx,%rcx,2), %rdi
	salq	$5, %rdi
	movl	8(%r14,%rdi), %edi
	testl	%edi, %edi
	je	.L67
	imulq	$96, %rcx, %rcx
	imulq	$168, %rax, %rsi
	movq	24(%r14,%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 16(%rsi)
	movl	$4096, 8(%rsi)
.L66:
	movslq	12(%rbx), %rcx
	salq	$4, %rcx
	addq	Rfp@GOTPCREL(%rip), %rcx
	movl	8(%rcx), %esi
	cmpl	$4096, %esi
	je	.L185
	movslq	%esi, %rcx
	leaq	(%rcx,%rcx,2), %rdi
	salq	$5, %rdi
	movl	8(%r14,%rdi), %r11d
	testl	%r11d, %r11d
	je	.L70
	imulq	$96, %rcx, %rcx
	imulq	$168, %rax, %rsi
	movq	24(%r14,%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 32(%rsi)
	movl	$4096, 24(%rsi)
.L69:
	leaq	(%rax,%rax,4), %rcx
	leaq	32(%rbx), %r8
	movswq	20(%rbx), %rsi
	leaq	(%rax,%rcx,4), %rcx
	salq	$3, %rcx
	leaq	(%r10,%rcx), %r11
	leaq	56(%r10,%rcx), %rdi
	movq	%rsi, 48(%r11)
	leaq	0(%r13,%r13,2), %rsi
	movl	$0, 124(%r11)
	leaq	.LC3(%rip), %rcx
	salq	$5, %rsi
	movq	%r11, 8(%rsp)
	addq	%rsi, %r14
	movq	%rax, 16(%r14)
	movl	$1, %esi
	movl	168(%rbx), %eax
	movl	%edx, 4(%r14)
	movl	$64, %edx
	movl	$1, (%r14)
	movl	$0, 8(%r14)
	movl	%eax, 160(%r11)
	xorl	%eax, %eax
	call	__sprintf_chk@PLT
	movl	168(%rbx), %eax
	movq	8(%rsp), %r11
	movl	%eax, 92(%r14)
	movq	160(%rbx), %rax
	movl	$0, 120(%r11)
	movl	%r12d, 148(%r11)
	movq	%rax, 80(%r14)
	jmp	.L158
.L18:
	movl	168(%rbx), %edi
	leaq	0(%r13,%r13,2), %rax
	movq	160(%rbx), %r8
	salq	$5, %rax
	addq	%r14, %rax
	movl	%edi, 92(%rax)
	movswq	20(%rbx), %rdi
	movq	%r8, 80(%rax)
	addq	$4, %r8
	movl	$1, (%rax)
	movl	%edx, 4(%rax)
	addq	%r8, %rdi
	movq	%rdi, 32(%rax)
	leal	-110(%rdx), %edi
	movl	$1, 40(%rax)
	cmpl	$1, %edi
	jbe	.L186
	movl	$1, 8(%rax)
.L17:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%eax, %eax
	movq	%r10, %rdi
	jmp	.L36
	.p2align 4,,7
	.p2align 3
.L187:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L36:
	movl	(%rdi), %r9d
	testl	%r9d, %r9d
	jne	.L187
	movl	168(%rbx), %r8d
	cltq
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	movl	%edx, 4(%rdi)
	movl	$1, (%rdi)
	movl	%r8d, 160(%rdi)
	movl	$0, 124(%rdi)
	movl	%r12d, 148(%rdi)
	leaq	0(%r13,%r13,2), %rdi
	salq	$5, %rdi
	addq	%r14, %rdi
	movl	%r8d, 92(%rdi)
	movq	160(%rbx), %r8
	movl	%edx, 4(%rdi)
	subl	$110, %edx
	movl	$1, (%rdi)
	cmpl	$1, %edx
	movl	$0, 44(%rdi)
	movq	%r8, 80(%rdi)
	jbe	.L115
	movq	Rint@GOTPCREL(%rip), %r9
.L116:
	leaq	0(%r13,%r13,2), %rdx
	salq	$5, %rdx
	movl	$0, 8(%r14,%rdx)
	movslq	8(%rbx), %rdx
	salq	$4, %rdx
	addq	%rdx, %r9
	movl	8(%r9), %edi
	cmpl	$4096, %edi
	je	.L188
	movslq	%edi, %rdx
	leaq	(%rdx,%rdx,2), %rdx
	salq	$5, %rdx
	addq	%rdx, %r14
	leaq	(%rax,%rax,4), %rdx
	movl	8(%r14), %r11d
	leaq	(%rax,%rdx,4), %rdx
	testl	%r11d, %r11d
	je	.L39
	movq	24(%r14), %rdi
	leaq	(%r10,%rdx,8), %rdx
	movl	$4096, 8(%rdx)
	movq	%rdi, 16(%rdx)
.L38:
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rax
	movq	192(%rbx), %rdx
	leaq	(%r10,%rax,8), %rax
	movl	$4096, 24(%rax)
	movq	%rdx, 32(%rax)
	movswq	20(%rbx), %rdx
	movq	%rdx, 48(%rax)
	jmp	.L33
.L11:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movq	TAM_RS_SUMREST@GOTPCREL(%rip), %rdi
	movl	(%rax), %eax
	movl	(%rdi), %r8d
	addl	%eax, %r8d
	cmpl	%r8d, %eax
	jge	.L1
	movq	RS@GOTPCREL(%rip), %r10
	movslq	%eax, %rdi
	leaq	(%rdi,%rdi,4), %r9
	leaq	(%rdi,%r9,4), %rdi
	leaq	(%r10,%rdi,8), %rdi
	jmp	.L83
	.p2align 4,,7
	.p2align 3
.L189:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L83:
	movl	(%rdi), %r11d
	testl	%r11d, %r11d
	jne	.L189
	movslq	8(%rbx), %r8
	cltq
	leaq	(%rax,%rax,4), %rdi
	leaq	(%rax,%rdi,4), %rdi
	salq	$4, %r8
	leaq	(%r10,%rdi,8), %rdi
	movl	$1, (%rdi)
	movl	%edx, 4(%rdi)
	movl	%r12d, 148(%rdi)
	movq	Rfp@GOTPCREL(%rip), %rdi
	leaq	(%rdi,%r8), %r9
	movl	8(%r9), %r8d
	cmpl	$4096, %r8d
	je	.L190
	movslq	%r8d, %r11
	leaq	(%r11,%r11,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L85
	imulq	$96, %r11, %r11
	imulq	$168, %rax, %r8
	movq	24(%r14,%r11), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
.L84:
	leaq	(%rax,%rax,4), %r8
	leaq	(%rax,%r8,4), %rax
	leaq	(%r10,%rax,8), %r8
	movslq	12(%rbx), %rax
	movl	$4096, 24(%r8)
	movl	$0, 124(%r8)
	salq	$4, %rax
	movq	(%rdi,%rax), %rax
	movq	%rax, 32(%r8)
	leaq	0(%r13,%r13,2), %rax
	salq	$5, %rax
	addq	%rax, %r14
	movslq	16(%rbx), %rax
	movl	$1, (%r14)
	movl	%edx, 4(%r14)
	movq	%rax, 16(%r14)
	salq	$4, %rax
	movl	%r12d, 8(%rdi,%rax)
	movl	168(%rbx), %eax
	movl	$0, 8(%r14)
	movl	%eax, 160(%r8)
	movl	%eax, 92(%r14)
	movq	160(%rbx), %rax
	movq	%rax, 80(%r14)
	jmp	.L33
.L21:
	movq	TAM_RS_ENTEROS@GOTPCREL(%rip), %rax
	movl	(%rax), %r8d
	testl	%r8d, %r8d
	jle	.L1
	movq	RS@GOTPCREL(%rip), %r10
	xorl	%eax, %eax
	movq	%r10, %rdi
	jmp	.L45
	.p2align 4,,7
	.p2align 3
.L191:
	addl	$1, %eax
	addq	$168, %rdi
	cmpl	%r8d, %eax
	je	.L1
.L45:
	movl	(%rdi), %r9d
	testl	%r9d, %r9d
	jne	.L191
	cltq
	leaq	0(%r13,%r13,2), %r8
	movl	168(%rbx), %r9d
	leaq	(%rax,%rax,4), %rdi
	salq	$5, %r8
	leaq	(%rax,%rdi,4), %rdi
	addq	%r14, %r8
	leaq	(%r10,%rdi,8), %rdi
	movl	%edx, 4(%rdi)
	movl	%r9d, 160(%rdi)
	movl	%edx, 4(%r8)
	movl	$1, (%rdi)
	movl	$0, 124(%rdi)
	movl	%r12d, 148(%rdi)
	movq	160(%rbx), %rdx
	movswq	20(%rbx), %rdi
	movl	%r9d, 92(%r8)
	movq	%rdx, 80(%r8)
	movq	Rint@GOTPCREL(%rip), %r9
	leaq	4(%rdx,%rdi), %rdx
	movq	%rdx, 32(%r8)
	movslq	8(%rbx), %rdx
	movl	$1, (%r8)
	movl	$0, 8(%r8)
	salq	$4, %rdx
	addq	%r9, %rdx
	movl	8(%rdx), %r8d
	cmpl	$4096, %r8d
	je	.L192
	movslq	%r8d, %rdx
	leaq	(%rdx,%rdx,2), %r11
	salq	$5, %r11
	movl	8(%r14,%r11), %r11d
	testl	%r11d, %r11d
	je	.L47
	imulq	$96, %rdx, %rdx
	imulq	$168, %rax, %r8
	movq	24(%r14,%rdx), %rdx
	addq	%r10, %r8
	movq	%rdx, 16(%r8)
	movl	$4096, 8(%r8)
.L46:
	movslq	12(%rbx), %rdx
	salq	$4, %rdx
	addq	%rdx, %r9
	movl	8(%r9), %edx
	cmpl	$4096, %edx
	je	.L193
	movslq	%edx, %r8
	leaq	(%r8,%r8,2), %r9
	salq	$5, %r9
	movl	8(%r14,%r9), %r9d
	testl	%r9d, %r9d
	je	.L50
	imulq	$96, %r8, %r8
	imulq	$168, %rax, %rdx
	movq	24(%r14,%r8), %r8
	addq	%r10, %rdx
	movq	%r8, 32(%rdx)
	movl	$4096, 24(%rdx)
.L49:
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rax
	movq	%rdi, 48(%r10,%rax,8)
	jmp	.L33
.L25:
	movl	$0, 8(%rbp)
	jmp	.L1
.L24:
	leaq	0(%r13,%r13,2), %rdx
	movabsq	$4294967297, %r11
	movl	168(%rbx), %r8d
	salq	$5, %rdx
	testl	%edi, %edi
	leaq	(%r14,%rdx), %rax
	movl	%r8d, 92(%rax)
	movq	160(%rbx), %r8
	movl	$0, 40(%rax)
	movq	%r8, 80(%rax)
	movq	%r11, (%r14,%rdx)
	movl	$1, 8(%rax)
	je	.L125
	cmpl	$32, %edi
	jg	.L125
.L31:
	movl	$1, 16(%rbp)
	jmp	.L33
	.p2align 4,,7
	.p2align 3
.L164:
	movl	$1, %edx
	leaq	.LC2(%rip), %rdi
	call	*%rcx
	jmp	.L7
.L125:
	movswl	20(%rbx), %edx
	leaq	0(%r13,%r13,2), %rax
	salq	$5, %rax
	addl	$32, %edx
	movl	%edx, 72(%r14,%rax)
	jmp	.L31
.L115:
	movq	TAM_REGISTROS@GOTPCREL(%rip), %rdx
	addq	$4, %r8
	movq	Rint@GOTPCREL(%rip), %r9
	movq	%r8, 24(%rdi)
	movl	(%rdx), %edx
	subl	$1, %edx
	movslq	%edx, %rdx
	movq	%rdx, 16(%rdi)
	salq	$4, %rdx
	movl	%r12d, 8(%r9,%rdx)
	jmp	.L116
.L186:
	movq	TAM_REGISTROS@GOTPCREL(%rip), %rdx
	movq	%r8, 24(%rax)
	movl	$1, 8(%rax)
	movl	(%rdx), %edx
	subl	$1, %edx
	movslq	%edx, %rdx
	movq	%rdx, 16(%rax)
	salq	$4, %rdx
	addq	Rint@GOTPCREL(%rip), %rdx
	movl	%r13d, 8(%rdx)
	jmp	.L33
.L188:
	movq	(%r9), %rdi
	leaq	(%rax,%rax,4), %rdx
	leaq	(%rax,%rdx,4), %rdx
	leaq	(%r10,%rdx,8), %rdx
	movq	%rdi, 16(%rdx)
	movl	$4096, 8(%rdx)
	jmp	.L38
.L39:
	movl	%edi, 8(%r10,%rdx,8)
	jmp	.L38
.L55:
	movq	Rfp@GOTPCREL(%rip), %rdx
	addq	%rsi, %rdx
	movl	%r12d, 8(%rdx)
	jmp	.L56
.L126:
	movq	%r9, %rdx
	salq	$4, %rdx
	addq	Rint@GOTPCREL(%rip), %rdx
	movl	%r12d, 8(%rdx)
	jmp	.L95
.L70:
	imulq	$168, %rax, %rcx
	movl	%esi, 24(%r10,%rcx)
	jmp	.L69
.L185:
	imulq	$168, %rax, %rsi
	movq	(%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 32(%rsi)
	movl	$4096, 24(%rsi)
	jmp	.L69
.L81:
	imulq	$168, %rax, %rdx
	movl	%edi, 8(%r10,%rdx)
	jmp	.L80
.L172:
	imulq	$168, %rax, %rdi
	movq	(%rdx), %rdx
	addq	%r10, %rdi
	movq	%rdx, 16(%rdi)
	movl	$4096, 8(%rdi)
	jmp	.L80
.L67:
	imulq	$168, %rax, %rcx
	movl	%esi, 8(%r10,%rcx)
	jmp	.L66
.L184:
	imulq	$168, %rax, %rsi
	movq	(%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 16(%rsi)
	movl	$4096, 8(%rsi)
	jmp	.L66
.L54:
	imulq	$168, %rax, %rcx
	movl	%esi, 8(%r10,%rcx)
	jmp	.L53
.L174:
	imulq	$168, %rax, %rsi
	movq	(%rcx), %rcx
	addq	%r10, %rsi
	movq	%rcx, 16(%rsi)
	movl	$4096, 8(%rsi)
	jmp	.L53
.L43:
	imulq	$168, %rax, %rdx
	movl	%edi, 8(%r10,%rdx)
	jmp	.L42
.L182:
	imulq	$168, %rax, %rdi
	movq	(%rdx), %rdx
	addq	%r10, %rdi
	movq	%rdx, 16(%rdi)
	movl	$4096, 8(%rdi)
	jmp	.L42
.L85:
	imulq	$168, %rax, %r9
	movl	%r8d, 8(%r10,%r9)
	jmp	.L84
.L190:
	imulq	$168, %rax, %r8
	movq	(%r9), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
	jmp	.L84
.L50:
	imulq	$168, %rax, %r8
	movl	%edx, 24(%r10,%r8)
	jmp	.L49
.L193:
	imulq	$168, %rax, %rdx
	movq	(%r9), %r8
	addq	%r10, %rdx
	movq	%r8, 32(%rdx)
	movl	$4096, 24(%rdx)
	jmp	.L49
.L47:
	imulq	$168, %rax, %rdx
	movl	%r8d, 8(%r10,%rdx)
	jmp	.L46
.L192:
	imulq	$168, %rax, %r8
	movq	(%rdx), %rdx
	addq	%r10, %r8
	movq	%rdx, 16(%r8)
	movl	$4096, 8(%r8)
	jmp	.L46
.L77:
	movl	%r11d, 24(%r10,%rax)
	jmp	.L76
.L170:
	imulq	$168, %rax, %rax
	addq	%rax, %r10
	movq	(%r8), %rax
	movl	$4096, 24(%r10)
	movq	%rax, 32(%r10)
	jmp	.L76
.L74:
	imulq	$168, %rax, %r11
	movl	%r8d, 8(%r10,%r11)
	jmp	.L73
.L169:
	imulq	$168, %rax, %r8
	movq	(%r11), %r11
	addq	%r10, %r8
	movq	%r11, 16(%r8)
	movl	$4096, 8(%r8)
	jmp	.L73
.L102:
	imulq	$168, %rax, %r9
	movl	%r8d, 24(%r10,%r9)
	jmp	.L101
.L177:
	imulq	$168, %rax, %r8
	movq	(%r9), %r9
	addq	%r10, %r8
	movq	%r9, 32(%r8)
	movl	$4096, 24(%r8)
	jmp	.L101
.L99:
	imulq	$168, %rax, %r9
	movl	%r8d, 8(%r10,%r9)
	jmp	.L98
.L176:
	imulq	$168, %rax, %r8
	movq	(%r9), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
	jmp	.L98
.L92:
	imulq	$168, %rax, %r9
	movl	%r8d, 24(%r10,%r9)
	jmp	.L91
.L167:
	imulq	$168, %rax, %r8
	movq	(%r9), %r9
	addq	%r10, %r8
	movq	%r9, 32(%r8)
	movl	$4096, 24(%r8)
	jmp	.L91
.L89:
	imulq	$168, %rax, %r9
	movl	%r8d, 8(%r10,%r9)
	jmp	.L88
.L166:
	imulq	$168, %rax, %r8
	movq	(%r9), %r9
	addq	%r10, %r8
	movq	%r9, 16(%r8)
	movl	$4096, 8(%r8)
	jmp	.L88
.L63:
	imulq	$168, %r11, %rdx
	movl	%eax, 24(%r10,%rdx)
	jmp	.L62
.L180:
	imulq	$168, %r11, %rax
	movq	(%r9), %rdx
	addq	%r10, %rax
	movq	%rdx, 32(%rax)
	movl	$4096, 24(%rax)
	jmp	.L62
.L60:
	imulq	$168, %r11, %rax
	movl	%edx, 8(%r10,%rax)
	jmp	.L59
.L179:
	imulq	$168, %r11, %rdx
	movq	(%rax), %rax
	addq	%r10, %rdx
	movq	%rax, 16(%rdx)
	movl	$4096, 8(%rdx)
	jmp	.L59
	.cfi_endproc
.LFE43:
	.size	fase_ISS, .-fase_ISS
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
