	.file	"main.cpp"
	.text
	.section .rdata,"dr"
.LC0:
	.ascii "%d\12\0"
	.section	.text.startup,"x"
	.p2align 4
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB2409:
	pushq	%rdi
	.seh_pushreg	%rdi
	pushq	%rsi
	.seh_pushreg	%rsi
	pushq	%rbx
	.seh_pushreg	%rbx
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	xorl	%edi, %edi
	xorl	%ebx, %ebx
	xorl	%esi, %esi
	call	__main
	jmp	.L4
	.p2align 4,,10
	.p2align 3
.L17:
	leaq	0(,%rdi,8), %rdx
	addq	%rdi, %rdi
.L3:
	movq	%rsi, %rcx
	call	realloc
	testq	%rax, %rax
	je	.L15
	movq	%rax, %rsi
.L2:
	movl	%ebx, (%rsi,%rbx,4)
	addq	$1, %rbx
	cmpq	$10, %rbx
	je	.L16
.L4:
	cmpq	%rdi, %rbx
	jb	.L2
	testq	%rdi, %rdi
	jne	.L17
	movl	$4, %edx
	movl	$1, %edi
	jmp	.L3
	.p2align 4,,10
	.p2align 3
.L16:
	xorl	%ebx, %ebx
	.p2align 4
	.p2align 3
.L5:
	movl	(%rsi,%rbx,4), %edx
	leaq	.LC0(%rip), %rcx
.LEHB0:
	call	__mingw_printf
	addq	$1, %rbx
	cmpq	$10, %rbx
	jne	.L5
	movq	%rsi, %rcx
	call	free
	xorl	%eax, %eax
	addq	$32, %rsp
	popq	%rbx
	popq	%rsi
	popq	%rdi
	ret
.L15:
	movl	$8, %ecx
	call	__cxa_allocate_exception
	leaq	_ZNSt9bad_allocD1Ev(%rip), %r8
	leaq	_ZTISt9bad_alloc(%rip), %rdx
	movq	%rax, %rcx
	movq	.refptr._ZTVSt9bad_alloc(%rip), %rax
	addq	$16, %rax
	movq	%rax, (%rcx)
	call	__cxa_throw
.LEHE0:
.L9:
	movq	%rax, %rbx
	movq	%rsi, %rcx
	call	free
	movq	%rbx, %rcx
.LEHB1:
	call	_Unwind_Resume
	nop
.LEHE1:
	.seh_handler	__gxx_personality_seh0, @unwind, @except
	.seh_handlerdata
.LLSDA2409:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE2409-.LLSDACSB2409
.LLSDACSB2409:
	.uleb128 .LEHB0-.LFB2409
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L9-.LFB2409
	.uleb128 0
	.uleb128 .LEHB1-.LFB2409
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE2409:
	.section	.text.startup,"x"
	.seh_endproc
	.globl	_ZTSSt9exception
	.section	.rdata$_ZTSSt9exception,"dr"
	.linkonce same_size
	.align 8
_ZTSSt9exception:
	.ascii "St9exception\0"
	.globl	_ZTISt9exception
	.section	.rdata$_ZTISt9exception,"dr"
	.linkonce same_size
	.align 8
_ZTISt9exception:
	.quad	_ZTVN10__cxxabiv117__class_type_infoE+16
	.quad	_ZTSSt9exception
	.globl	_ZTSSt9bad_alloc
	.section	.rdata$_ZTSSt9bad_alloc,"dr"
	.linkonce same_size
	.align 8
_ZTSSt9bad_alloc:
	.ascii "St9bad_alloc\0"
	.globl	_ZTISt9bad_alloc
	.section	.rdata$_ZTISt9bad_alloc,"dr"
	.linkonce same_size
	.align 8
_ZTISt9bad_alloc:
	.quad	_ZTVN10__cxxabiv120__si_class_type_infoE+16
	.quad	_ZTSSt9bad_alloc
	.quad	_ZTISt9exception
	.def	__gxx_personality_seh0;	.scl	2;	.type	32;	.endef
	.def	__main;	.scl	2;	.type	32;	.endef
	.ident	"GCC: (Rev5, Built by MSYS2 project) 16.1.0"
	.def	realloc;	.scl	2;	.type	32;	.endef
	.def	free;	.scl	2;	.type	32;	.endef
	.def	__cxa_allocate_exception;	.scl	2;	.type	32;	.endef
	.def	_ZNSt9bad_allocD1Ev;	.scl	2;	.type	32;	.endef
	.def	__cxa_throw;	.scl	2;	.type	32;	.endef
	.def	_Unwind_Resume;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr._ZNSt9bad_allocD1Ev, "dr"
	.p2align	3, 0
	.globl	.refptr._ZNSt9bad_allocD1Ev
	.linkonce	discard
.refptr._ZNSt9bad_allocD1Ev:
	.quad	_ZNSt9bad_allocD1Ev
	.section	.rdata$.refptr._ZTVSt9bad_alloc, "dr"
	.p2align	3, 0
	.globl	.refptr._ZTVSt9bad_alloc
	.linkonce	discard
.refptr._ZTVSt9bad_alloc:
	.quad	_ZTVSt9bad_alloc
