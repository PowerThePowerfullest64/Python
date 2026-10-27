	.file	"main.cpp"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2569:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	call	__main
	movl	$5, -4(%rbp)
	leaq	-4(%rbp), %rax
	movq	%rax, %rcx
	call	_Z4add1IiEvRT_
	movss	.LC0(%rip), %xmm0
	movss	%xmm0, -8(%rbp)
	leaq	-8(%rbp), %rax
	movq	%rax, %rcx
	call	_Z4add1IfEvRT_
	movl	$0, %eax
	addq	$48, %rsp
	popq	%rbp
	ret
	.seh_endproc
	.section	.text$_Z4add1IiEvRT_,"x"
	.linkonce discard
	.globl	_Z4add1IiEvRT_
	.def	_Z4add1IiEvRT_;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4add1IiEvRT_
_Z4add1IiEvRT_:
.LFB2846:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rax
	movl	(%rax), %eax
	leal	1(%rax), %edx
	movq	16(%rbp), %rax
	movl	%edx, (%rax)
	nop
	popq	%rbp
	ret
	.seh_endproc
	.section	.text$_Z4add1IfEvRT_,"x"
	.linkonce discard
	.globl	_Z4add1IfEvRT_
	.def	_Z4add1IfEvRT_;	.scl	2;	.type	32;	.endef
	.seh_proc	_Z4add1IfEvRT_
_Z4add1IfEvRT_:
.LFB2847:
	pushq	%rbp
	.seh_pushreg	%rbp
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.seh_endprologue
	movq	%rcx, 16(%rbp)
	movq	16(%rbp), %rax
	movss	(%rax), %xmm1
	movss	.LC1(%rip), %xmm0
	addss	%xmm1, %xmm0
	movq	16(%rbp), %rax
	movss	%xmm0, (%rax)
	nop
	popq	%rbp
	ret
	.seh_endproc
	.section .rdata,"dr"
	.align 4
.LC0:
	.long	1084227584
	.align 4
.LC1:
	.long	1065353216
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev8, Built by MSYS2 project) 15.2.0"
