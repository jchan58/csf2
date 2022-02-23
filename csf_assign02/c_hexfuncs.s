	.file	"c_hexfuncs.c"
	.text
	.globl	hex_read
	.type	hex_read, @function
hex_read:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	movq	-24(%rbp), %rax
	movl	$16, %edx
	movq	%rax, %rsi
	movl	$0, %edi
	call	read
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	hex_read, .-hex_read
	.globl	hex_write_string
	.type	hex_write_string, @function
hex_write_string:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L4
.L5:
	addl	$1, -4(%rbp)
	addl	$1, -8(%rbp)
.L4:
	movl	-8(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	testb	%al, %al
	jne	.L5
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	write
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	hex_write_string, .-hex_write_string
	.globl	toLower
	.type	toLower, @function
toLower:
.LFB2:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L7
.L9:
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$90, %al
	jg	.L8
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	cmpb	$64, %al
	jle	.L8
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %eax
	leal	32(%rax), %ecx
	movl	-4(%rbp), %eax
	movslq	%eax, %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	movl	%ecx, %edx
	movb	%dl, (%rax)
.L8:
	addl	$1, -4(%rbp)
.L7:
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jle	.L9
	nop
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE2:
	.size	toLower, .-toLower
	.globl	hex_format_byte_as_hex
	.type	hex_format_byte_as_hex, @function
hex_format_byte_as_hex:
.LFB3:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$72, %rsp
	.cfi_offset 3, -24
	movl	%edi, %eax
	movq	%rsi, -80(%rbp)
	movb	%al, -68(%rbp)
	movq	%rsp, %rax
	movq	%rax, %rbx
	movl	$0, -20(%rbp)
	movzbl	-68(%rbp), %eax
	movb	%al, -21(%rbp)
	jmp	.L11
.L12:
	movzbl	-21(%rbp), %eax
	shrb	$4, %al
	movb	%al, -21(%rbp)
	addl	$1, -20(%rbp)
.L11:
	cmpb	$0, -21(%rbp)
	jne	.L12
	movl	-20(%rbp), %eax
	movl	%eax, -28(%rbp)
	movl	-20(%rbp), %eax
	movslq	%eax, %rdx
	subq	$1, %rdx
	movq	%rdx, -48(%rbp)
	movslq	%eax, %rdx
	movq	%rdx, %r10
	movl	$0, %r11d
	movslq	%eax, %rdx
	movq	%rdx, %r8
	movl	$0, %r9d
	cltq
	movl	$16, %edx
	subq	$1, %rdx
	addq	%rdx, %rax
	movl	$16, %esi
	movl	$0, %edx
	divq	%rsi
	imulq	$16, %rax, %rax
	subq	%rax, %rsp
	movq	%rsp, %rax
	addq	$0, %rax
	movq	%rax, -56(%rbp)
	movb	$32, -29(%rbp)
	movl	$0, -36(%rbp)
	jmp	.L13
.L16:
	movzbl	-68(%rbp), %eax
	andl	$15, %eax
	cmpb	$9, %al
	ja	.L14
	movzbl	-68(%rbp), %eax
	andl	$15, %eax
	addl	$48, %eax
	movb	%al, -29(%rbp)
	jmp	.L15
.L14:
	movzbl	-68(%rbp), %eax
	andl	$15, %eax
	addl	$55, %eax
	movb	%al, -29(%rbp)
.L15:
	movl	-28(%rbp), %eax
	subl	$1, %eax
	movq	-56(%rbp), %rdx
	cltq
	movzbl	-29(%rbp), %ecx
	movb	%cl, (%rdx,%rax)
	movzbl	-68(%rbp), %eax
	shrb	$4, %al
	movb	%al, -68(%rbp)
	subl	$1, -28(%rbp)
	addl	$1, -36(%rbp)
.L13:
	movl	-36(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L16
	movl	$0, -40(%rbp)
	jmp	.L17
.L18:
	movl	-40(%rbp), %eax
	movslq	%eax, %rdx
	movq	-80(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-56(%rbp), %rdx
	movl	-40(%rbp), %eax
	cltq
	movzbl	(%rdx,%rax), %eax
	movb	%al, (%rcx)
	addl	$1, -40(%rbp)
.L17:
	movl	-40(%rbp), %eax
	cmpl	-20(%rbp), %eax
	jl	.L18
	movq	-80(%rbp), %rax
	movl	$2, %esi
	movq	%rax, %rdi
	call	toLower
	movq	%rbx, %rsp
	nop
	movq	-8(%rbp), %rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE3:
	.size	hex_format_byte_as_hex, .-hex_format_byte_as_hex
	.globl	hex_to_printable
	.type	hex_to_printable, @function
hex_to_printable:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	%edi, %eax
	movb	%al, -4(%rbp)
	cmpb	$31, -4(%rbp)
	ja	.L20
	movl	$46, %eax
	jmp	.L21
.L20:
	movzbl	-4(%rbp), %eax
.L21:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	hex_to_printable, .-hex_to_printable
	.globl	hex_format_offset
	.type	hex_format_offset, @function
hex_format_offset:
.LFB5:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	movl	%edi, -36(%rbp)
	movq	%rsi, -48(%rbp)
	movl	$0, -4(%rbp)
	movl	-36(%rbp), %eax
	movl	%eax, -8(%rbp)
	jmp	.L23
.L24:
	movl	-8(%rbp), %eax
	shrl	$4, %eax
	movl	%eax, -8(%rbp)
	addl	$1, -4(%rbp)
.L23:
	cmpl	$0, -8(%rbp)
	jne	.L24
	movl	$1, -12(%rbp)
	movb	$32, -13(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L25
.L28:
	movl	-36(%rbp), %eax
	andl	$15, %eax
	cmpl	$9, %eax
	ja	.L26
	movl	-36(%rbp), %eax
	andl	$15, %eax
	addl	$48, %eax
	movb	%al, -13(%rbp)
	jmp	.L27
.L26:
	movl	-36(%rbp), %eax
	andl	$15, %eax
	addl	$55, %eax
	movb	%al, -13(%rbp)
.L27:
	movl	$8, %eax
	subl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rax, %rdx
	movzbl	-13(%rbp), %eax
	movb	%al, (%rdx)
	movl	-36(%rbp), %eax
	shrl	$4, %eax
	movl	%eax, -36(%rbp)
	addl	$1, -12(%rbp)
	addl	$1, -20(%rbp)
.L25:
	movl	-20(%rbp), %eax
	cmpl	-4(%rbp), %eax
	jl	.L28
	movl	$0, -24(%rbp)
	jmp	.L29
.L30:
	movl	-24(%rbp), %eax
	movslq	%eax, %rdx
	movq	-48(%rbp), %rax
	addq	%rdx, %rax
	movb	$48, (%rax)
	addl	$1, -24(%rbp)
.L29:
	movl	$8, %eax
	subl	-4(%rbp), %eax
	cmpl	%eax, -24(%rbp)
	jl	.L30
	movq	-48(%rbp), %rax
	addq	$8, %rax
	movb	$0, (%rax)
	movq	-48(%rbp), %rax
	movl	$8, %esi
	movq	%rax, %rdi
	call	toLower
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE5:
	.size	hex_format_offset, .-hex_format_offset
	.ident	"GCC: (GNU) 11.2.1 20211203 (Red Hat 11.2.1-7)"
	.section	.note.GNU-stack,"",@progbits
