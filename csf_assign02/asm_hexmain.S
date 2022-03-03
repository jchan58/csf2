/* Hexdump main function, assembly language version */

	.section .data
	//these strings come null terminated i think

colon_space:
	.string ": "

single_space:
	.string " "

three_spaces:
	.string "   "

newline:
	.string "\n"
	
	.section .text

	.globl main
main:
	subq $8, %rsp
	pushq %rbp
	pushq %r15
	subq $56, %rsp			// 17 + 3 + 17 + 9 + 8 + 8	
	movq %rsp, %rbp			//store where rsp points to hold the strings
	movq $0, %r8
	pushq %r14

			/* 8(%rbp is start of line */
			/* 25(%rbp) is start of hexbyte */
	 		/* 28(%rbp) start of hex_printable */
			/* 45(%rbp) is start of offset_formatted */
	movb $0, 24(%rbp, %r8, 1)              /*null terminate line */
	movb $0, 44(%rbp, %r8, 1)  		/*null terminate hex_printable*/
	movb $0, 28(%rbp, %r8, 1)		/*null terminate hex_byte*/
	movb $0, 53(%rbp, %r8, 1)		/*null terminate format_offset*/     	
	movq $0, %r10 			/* %r10d holds offset */
	movq %r10, %rdi 		/*make offset arg 1 for hex_format_offset*/
	leaq 45(%rbp, %r8, 1), %rsi 	/*put offset_formatted in sil, arg 2 for it*/
	call hex_format_offset
	jmp .LreadwhileLoop		/*start reading from the input*/

.LreadwhileLoop:
	leaq 8(%rbp, %r8, 1), %rdi		/*make line the argument for hex_read*/
	call hex_read
	movq %rax, %r9			/* %r9d stores line count given from hex_read*/
	movq $0, %r8			/*%r8 stores 0 just for this comparison*/
	cmpq %r9, %r8
	je .LDone			/* if nothing is read (r9 is 0) you are done*/	
	leaq 45(%rbp, %r8, 1), %rdi 		/*make offset_formated the argument for hex_write_string*/
	call hex_write_string

	movq $colon_space, %r14		/*r14 holds print formatting strings or loop counters, here put in ": "*/
	movq %r14, %rdi			/* put r14 in arg 1 for hex_write */
	
	call hex_write_string		/* write a ": " */
	movq $0, %r8                    /*make r8d the for loop index*/
	jmp .LWriteHexSentenceForLoop	/*jump to the loop */

.LWriteHexSentenceForLoop:
	cmpq %r9, %r8
	je .LSpaceFormatLoopPre		/*when index hits line count, continue */
	
	movb 8(%rbp, %r8, 1), %dil       /* put line[index] in %rdi(dil, 1 byte %rdi) for arg one of hex_format_byte */
	movb 25(%rbp, %r8, 1), %sil			/* put hex_byte in for %sil for arg two of hex_format_byte */
	movb %sil, %dil			/*put the "returned" hex_byte in dil for arg for hex_write*/
	call hex_write_string
	movq $single_space, %r14		/*store a space in r14*/
	movq %r14, %rdi			/*make the single space the arg for hex_write*/
	call hex_write_string
	incq %r8			/*increment the index in line*/
	
	jmp .LWriteHexSentenceForLoop	/*loop again!*/


.LSpaceFormatLoopPre:
	movq $0, %r8                    /*make r8 the for loop index*/
	movq $16, %rcx
	subq %r9, %rcx			/* make ecx hold 16 - linecount*/
	movq $three_spaces, %r14	/* store three spaces in %r14 */
	movq %r14, %rdi		/*make 3 spaces be the arg for hex_write*/
	jmp .LSpaceFormatLoop

.LSpaceFormatLoop:
	cmpq %rcx, %r8			/*if loop index equals 16-linecount*/
	je .LFormatAndLoopPrep		/*jump toformat and prepare for the next for  loop*/
	call hex_write_string		/*write three spaces*/
	incq %r8			/*increment the loop index */

.LFormatAndLoopPrep:
	movq $0, %r8			/*make r8d the loop index*/
	movq $single_space, %r14 	/* store a space in %r14 */
	movq %r14, %rdi 		/*move the space into arg 1 for hex write*/
	call hex_write_string
	jmp .LMakeLinePrintableForLoop	/*jump to make the line printable*/

.LMakeLinePrintableForLoop:
	cmpq %r9, %r8
	jmp .LwhileLoopEnd		/*if the index equals the line count jump to the end of the while loop*/
	movb 8(%rbp, %r8, 1), %dil			/*put line[index] in arg1 for hex_to_printable */
	call hex_to_printable
	movb 28(%rbp, %r8, 1), %dil 	/*put the "returned" printable character in hex_printable*/
	incq %r8			/*increment the index*/

.LwhileLoopEnd:
	movb $0, 24(%rbp, %r8, 1)		/*null terminate line */
	movb $0, 44(%rbp, %r8, 1)		/* null terminate hex printable*/
	leaq 28(%rbp, %r8, 1), %rdi		/*make hex_printable arg 1 for hex_write get only certain part?*/
	call hex_write_string
	movq $newline, %r14		  /*store a newline in %r14*/
	movq %r14, %rdi			/*put a newline in arg 1 for hex_write*/
	call hex_write_string
	addq %r9, %r10			/* add line count to offset*/

	movq %r10, %rdi                 /*make offset arg 1 for hex_format_offset*/
        leaq 45(%rbp, %r8, 1), %rsi     /*put offset_formatted in sil, arg 2 for it*/
        call hex_format_offset
	jmp .LreadwhileLoop		/*start the while loop again */ 
	
.LDone:
	movq $0, %rax
	popq %r14
	
	addq $64, %rsp
	ret

/* vim:ft=gas:
 */
