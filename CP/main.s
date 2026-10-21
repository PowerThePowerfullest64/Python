	.file	"main.c"
	.text
	.p2align 4
	.globl	_Z6v_initP6vectory
	.def	_Z6v_initP6vectory;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6v_initP6vectory
_Z6v_initP6vectory:
.LFB191:
	.seh_endprologue
	movq	%rdx, %xmm0
	movq	$0, (%rcx)
	movq	$0, 24(%rcx)
	movups	%xmm0, 8(%rcx)
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC0:
	.ascii "Grew array\0"
	.text
	.p2align 4
	.globl	_Z6v_growP6vector
	.def	_Z6v_growP6vector;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6v_growP6vector
_Z6v_growP6vector:
.LFB192:
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	movq	24(%rcx), %rax
	movq	%rcx, %rbx
	cmpq	%rax, 16(%rcx)
	jb	.L4
	testq	%rax, %rax
	leaq	(%rax,%rax), %rsi
	movl	$1, %eax
	movq	8(%rcx), %rdx
	cmove	%rax, %rsi
	movq	(%rcx), %rcx
	imulq	%rsi, %rdx
	call	realloc
	testq	%rax, %rax
	je	.L8
	movq	%rax, (%rbx)
	leaq	.LC0(%rip), %rcx
	movq	%rsi, 24(%rbx)
	call	puts
.L4:
	movl	$1, %eax
.L3:
	addq	$40, %rsp
	popq	%rbx
	popq	%rsi
	ret
.L8:
	xorl	%eax, %eax
	jmp	.L3
	.seh_endproc
	.p2align 4
	.globl	_Z6v_pushP6vectorPv
	.def	_Z6v_pushP6vectorPv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z6v_pushP6vectorPv
_Z6v_pushP6vectorPv:
.LFB193:
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	24(%rcx), %rax
	movq	%rcx, %rbx
	movq	16(%rcx), %rcx
	movq	%rdx, %rdi
	cmpq	%rax, %rcx
	jb	.L10
	testq	%rax, %rax
	leaq	(%rax,%rax), %rsi
	movl	$1, %eax
	movq	8(%rbx), %rdx
	cmove	%rax, %rsi
	movq	(%rbx), %rcx
	imulq	%rsi, %rdx
	call	realloc
	testq	%rax, %rax
	je	.L14
	movq	%rax, (%rbx)
	leaq	.LC0(%rip), %rcx
	movq	%rsi, 24(%rbx)
	call	puts
	movq	16(%rbx), %rcx
.L10:
	movq	8(%rbx), %r8
	movq	%rdi, %rdx
	imulq	%r8, %rcx
	addq	(%rbx), %rcx
	call	memcpy
	addq	$1, 16(%rbx)
	movl	$1, %eax
.L9:
	addq	$32, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	ret
.L14:
	xorl	%eax, %eax
	jmp	.L9
	.seh_endproc
	.p2align 4
	.globl	_Z5v_popP6vector
	.def	_Z5v_popP6vector;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z5v_popP6vector
_Z5v_popP6vector:
.LFB194:
	.seh_endprologue
	movq	16(%rcx), %rax
	testq	%rax, %rax
	je	.L15
	subq	$1, %rax
	movq	%rax, 16(%rcx)
.L15:
	ret
	.seh_endproc
	.p2align 4
	.globl	_Z8v_deleteP6vector
	.def	_Z8v_deleteP6vector;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z8v_deleteP6vector
_Z8v_deleteP6vector:
.LFB195:
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movq	%rcx, %rbx
	movq	(%rcx), %rcx
	call	free
	pxor	%xmm0, %xmm0
	movq	$0, (%rbx)
	movups	%xmm0, 16(%rbx)
	addq	$32, %rsp
	popq	%rbx
	ret
	.seh_endproc
	.p2align 4
	.globl	_Z5v_getP6vectory
	.def	_Z5v_getP6vectory;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z5v_getP6vectory
_Z5v_getP6vectory:
.LFB196:
	.seh_endprologue
	movq	8(%rcx), %rax
	imulq	%rdx, %rax
	addq	(%rcx), %rax
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC1:
	.ascii "WARNING: Index out of range.\0"
	.text
	.p2align 4
	.globl	_Z4v_atP6vectory
	.def	_Z4v_atP6vectory;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4v_atP6vectory
_Z4v_atP6vectory:
.LFB197:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	cmpq	16(%rcx), %rdx
	jnb	.L25
	imulq	8(%rcx), %rdx
	movq	(%rcx), %rax
	addq	%rdx, %rax
.L22:
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L25:
	leaq	.LC1(%rip), %rcx
	call	puts
	xorl	%eax, %eax
	jmp	.L22
	.seh_endproc
	.p2align 4
	.globl	_Z8v_insertP6vectoryPv
	.def	_Z8v_insertP6vectoryPv;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z8v_insertP6vectoryPv
_Z8v_insertP6vectoryPv:
.LFB198:
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movq	%rcx, %rbx
	movq	16(%rcx), %rcx
	movq	%rdx, %rsi
	movq	%r8, %rdi
	cmpq	%rdx, %rcx
	jnb	.L27
.L31:
	xorl	%eax, %eax
	addq	$48, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	ret
	.p2align 4,,10
	.p2align 3
.L27:
	movq	24(%rbx), %rax
	cmpq	%rax, %rcx
	jb	.L29
	testq	%rax, %rax
	leaq	(%rax,%rax), %r8
	movl	$1, %eax
	movq	(%rbx), %rcx
	cmove	%rax, %r8
	movq	%r8, %rdx
	imulq	8(%rbx), %rdx
	movq	%r8, 40(%rsp)
	call	realloc
	testq	%rax, %rax
	je	.L31
	movq	40(%rsp), %r8
	movq	%rax, (%rbx)
	leaq	.LC0(%rip), %rcx
	movq	%r8, 24(%rbx)
	call	puts
	movq	16(%rbx), %rcx
.L29:
	movq	8(%rbx), %r8
	movq	%rdi, %rdx
	imulq	%r8, %rcx
	addq	(%rbx), %rcx
	call	memcpy
	movq	8(%rbx), %r8
	movq	16(%rbx), %rax
	movq	(%rbx), %rcx
	movq	%r8, %rdx
	addq	$1, %rax
	imulq	%rsi, %rdx
	movq	%rax, 16(%rbx)
	subq	%rsi, %rax
	subq	$1, %rax
	leaq	(%rcx,%rdx), %r9
	addq	%r8, %rdx
	imulq	%rax, %r8
	addq	%rdx, %rcx
	movq	%r9, %rdx
	call	memmove
	movq	8(%rbx), %r8
	movq	(%rbx), %rcx
	movq	%rdi, %rdx
	imulq	%r8, %rsi
	addq	%rsi, %rcx
	call	memcpy
	movl	$1, %eax
	addq	$48, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	ret
	.seh_endproc
	.p2align 4
	.globl	_Z8v_removeP6vectory
	.def	_Z8v_removeP6vectory;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z8v_removeP6vectory
_Z8v_removeP6vectory:
.LFB199:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	movq	16(%rcx), %rax
	movq	%rcx, %r9
	xorl	%ecx, %ecx
	cmpq	%rax, %rdx
	jnb	.L36
	leaq	-1(%rax), %rcx
	cmpq	%rdx, %rcx
	je	.L44
	movq	8(%r9), %r8
	leaq	1(%rdx), %r10
	movq	(%r9), %rcx
	subq	%rdx, %rax
	subq	$1, %rax
	movq	%r9, 48(%rsp)
	imulq	%r8, %r10
	leaq	(%rcx,%r10), %rdx
	subq	%r8, %r10
	imulq	%rax, %r8
	addq	%r10, %rcx
	call	memmove
	movq	48(%rsp), %r9
	movq	16(%r9), %rax
	testq	%rax, %rax
	je	.L39
	subq	$1, %rax
	movq	%rax, 16(%r9)
.L39:
	movl	$1, %ecx
.L36:
	movl	%ecx, %eax
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L44:
	movq	%rcx, 16(%r9)
	jmp	.L39
	.seh_endproc
	.p2align 4
	.globl	_Z9v_reserveP6vectory
	.def	_Z9v_reserveP6vectory;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z9v_reserveP6vectory
_Z9v_reserveP6vectory:
.LFB200:
	subq	$40, %rsp
	.seh_stackalloc	40
	.seh_endprologue
	movq	%rcx, %r8
	cmpq	%rdx, 24(%rcx)
	jb	.L46
.L48:
	xorl	%eax, %eax
	addq	$40, %rsp
	ret
	.p2align 4,,10
	.p2align 3
.L46:
	movq	%rdx, 56(%rsp)
	imulq	8(%rcx), %rdx
	movq	(%rcx), %rcx
	movq	%r8, 48(%rsp)
	call	realloc
	testq	%rax, %rax
	je	.L48
	movq	48(%rsp), %r8
	movq	56(%rsp), %r9
	movq	%rax, (%r8)
	movl	$1, %eax
	movq	%r9, 24(%r8)
	addq	$40, %rsp
	ret
	.seh_endproc
	.section .rdata,"dr"
.LC2:
	.ascii "Hello, World!\0"
.LC4:
	.ascii "%d\12\0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB201:
	pushq	%r15
	.seh_pushreg	%r15
	pushq	%r14
	.seh_pushreg	%r14
	pushq	%r13
	.seh_pushreg	%r13
	pushq	%r12
	.seh_pushreg	%r12
	pushq	%rbp
	.seh_pushreg	%rbp
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$104, %rsp
	.seh_stackalloc	104
	.seh_endprologue
	xorl	%esi, %esi
	xorl	%ebx, %ebx
	call	__main
	leaq	.LC2(%rip), %rcx
	call	puts
	leaq	64(%rsp), %rcx
	movl	$1000, %edx
	movdqu	.LC3(%rip), %xmm0
	movq	$0, 64(%rsp)
	movq	$0, 88(%rsp)
	movups	%xmm0, 72(%rsp)
	call	_Z9v_reserveP6vectory
	leaq	60(%rsp), %rax
	movq	64(%rsp), %rdi
	movl	$0, 60(%rsp)
	movq	72(%rsp), %r12
	movq	80(%rsp), %r13
	movq	%rax, 40(%rsp)
	movq	88(%rsp), %rbp
	jmp	.L56
	.p2align 4,,10
	.p2align 3
.L63:
	leaq	(%rbp,%rbp), %r14
	movq	%r14, %rdx
	imulq	%r12, %rdx
.L54:
	movq	%rdi, %rcx
	call	realloc
	testq	%rax, %rax
	je	.L55
	leaq	.LC0(%rip), %rcx
	movq	%rax, %rdi
	movq	%r14, %rbp
	call	puts
.L53:
	movq	%r13, %rcx
	movq	40(%rsp), %rdx
	movq	%r12, %r8
	addq	$1, %r13
	imulq	%r12, %rcx
	addq	%rdi, %rcx
	call	memcpy
.L55:
	movl	(%rdi,%rsi), %edx
	leaq	.LC4(%rip), %rcx
	addl	$1, %ebx
	addq	%r12, %rsi
	call	printf
	movl	%ebx, 60(%rsp)
	cmpl	$1000, %ebx
	je	.L62
.L56:
	cmpq	%rbp, %r13
	jb	.L53
	testq	%rbp, %rbp
	jne	.L63
	movq	%r12, %rdx
	movl	$1, %r14d
	jmp	.L54
	.p2align 4,,10
	.p2align 3
.L62:
	xorl	%eax, %eax
	addq	$104, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	popq	%rbp
	popq	%r12
	popq	%r13
	popq	%r14
	popq	%r15
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 16
.LC3:
	.quad	4
	.quad	0
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev8, Built by MSYS2 project) 15.2.0"
	.def	realloc;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	memcpy;	.scl	2;	.type	32;	.endef
	.def	free;	.scl	2;	.type	32;	.endef
	.def	memmove;	.scl	2;	.type	32;	.endef
	.def	printf;	.scl	2;	.type	32;	.endef
