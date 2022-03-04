	.file	"c_hexmain.c"
	.text
	.section	.rodata
.LC0:
	.string	": "
.LC1:
	.string	" "
.LC2:
	.string	"   "
.LC3:
	.string	"\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	addq	$-128, %rsp
	movl	%edi, -116(%rbp)
	movq	%rsi, -128(%rbp)
	movl	$0, -32(%rbp)
	movq	$0, -96(%rbp)
	movq	$0, -88(%rbp)
	movb	$0, -80(%rbp)
	movl	$0, -4(%rbp)
	movq	$48, -105(%rbp)
	movb	$0, -97(%rbp)
	movb	$0, -97(%rbp)
	leaq	-105(%rbp), %rdx
	movl	-4(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	hex_format_offset
	jmp	.L2
.L15:
	movl	-32(%rbp), %eax
	cltq
	movb	$0, -64(%rbp,%rax)
	leaq	-105(%rbp), %rax
	movq	%rax, %rdi
	call	hex_write_string
	movl	$.LC0, %edi
	call	hex_write_string
	movl	$0, -8(%rbp)
	jmp	.L3
.L4:
	movl	-8(%rbp), %eax
	cltq
	movzbl	-64(%rbp,%rax), %eax
	movzbl	%al, %eax
	leaq	-67(%rbp), %rdx
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	hex_format_byte_as_hex
	leaq	-67(%rbp), %rax
	movq	%rax, %rdi
	call	hex_write_string
	movb	$0, -65(%rbp)
	movl	$.LC1, %edi
	call	hex_write_string
	addl	$1, -8(%rbp)
.L3:
	movl	-8(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L4
	movl	$0, -12(%rbp)
	jmp	.L5
.L6:
	movl	$.LC2, %edi
	call	hex_write_string
	addl	$1, -12(%rbp)
.L5:
	movl	$16, %eax
	subl	-32(%rbp), %eax
	cmpl	%eax, -12(%rbp)
	jl	.L6
	movl	$.LC1, %edi
	call	hex_write_string
	movl	$0, -16(%rbp)
	jmp	.L7
.L8:
	movl	-16(%rbp), %eax
	cltq
	movzbl	-64(%rbp,%rax), %eax
	movzbl	%al, %eax
	movl	%eax, %edi
	call	hex_to_printable
	movl	-16(%rbp), %edx
	movslq	%edx, %rdx
	movb	%al, -96(%rbp,%rdx)
	addl	$1, -16(%rbp)
.L7:
	movl	-16(%rbp), %eax
	cmpl	-32(%rbp), %eax
	jl	.L8
	movl	-32(%rbp), %eax
	cltq
	movb	$0, -96(%rbp,%rax)
	leaq	-96(%rbp), %rax
	movq	%rax, %rdi
	call	hex_write_string
	movl	$.LC3, %edi
	call	hex_write_string
	movl	-32(%rbp), %eax
	addl	%eax, -4(%rbp)
	leaq	-105(%rbp), %rdx
	movl	-4(%rbp), %eax
	movq	%rdx, %rsi
	movl	%eax, %edi
	call	hex_format_offset
	movl	$0, -20(%rbp)
	jmp	.L9
.L10:
	movl	-20(%rbp), %eax
	cltq
	movb	$0, -96(%rbp,%rax)
	addl	$1, -20(%rbp)
.L9:
	cmpl	$16, -20(%rbp)
	jle	.L10
	movl	$0, -24(%rbp)
	jmp	.L11
.L12:
	movl	-24(%rbp), %eax
	cltq
	movb	$0, -64(%rbp,%rax)
	addl	$1, -24(%rbp)
.L11:
	cmpl	$16, -24(%rbp)
	jle	.L12
	movl	$0, -28(%rbp)
	jmp	.L13
.L14:
	movl	-28(%rbp), %eax
	cltq
	movb	$48, -67(%rbp,%rax)
	movb	$0, -64(%rbp)
	addl	$1, -28(%rbp)
.L13:
	cmpl	$2, -28(%rbp)
	jle	.L14
.L2:
	leaq	-64(%rbp), %rax
	movq	%rax, %rdi
	call	hex_read
	movl	%eax, -32(%rbp)
	cmpl	$0, -32(%rbp)
	jne	.L15
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 11.2.1 20211203 (Red Hat 11.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
