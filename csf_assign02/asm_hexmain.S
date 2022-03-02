/* Hexdump main function, assembly language version */

	.section .data
	//these strings come null terminated i think
line:
	.string "pppppppppppppppp"
hex_byte:	
	.string "pp"

hex_printable:
	.string "pppppppppppppppp"

offset_formatted:
	.string "pppppppp"

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
	subq $54, %rsp			// 17 + 3 + 17 + 9 + 8 + 8	
	pushb %rbx
	pushb %r12
	pushb %r13
	pushb %r14
	pushb %r15
	movb 0(%rsp), %r13b             /* 0(%rsp) mov it into %r13b */
	movb 17(%rsp), %r15b		/* 17(%rsp) move into %r15b */
	movb 20(%rsp), %bl 		/* 20(%rsp) move into %bl */
	movb 37(%rsp), %r12b		/* 37(%rsp) moves into %r12b */
	movb $0, %r10b 			/* %r10b holds offset */
	movb %r10b, %dil 		/*make offset arg 1 for hex_format_offset*/
	movb %r12b, %sil		/*put offset_formatted in sil, arg 2 for it*/
	call hex_format_offset
//	movq %rsi, %r12			/*store offset_formatted in r12*/
	jmp .LreadwhileLoop		/*start reading from the input*/

.LreadwhileLoop:
	movb %r13b, %dil		/*make line the argument for hex_read*/
	call hex_read
	movb %al, %r9b			/*%r9b stores line count given from hex_read*/
	movb $0, %r8b			/*%r8b stores 0 just for this comparison*/
	cmpb %r9b, %r8b
	je .LDone			/* if nothing is read (r9 is 0) you are done*/

	movq %r12b, %dil		/*make offset_formated the argument for hex_write_string*/
	call hex_write_string

	movb colon_space, %r14b		/*r14 holds print formatting strings or loop counters, here put in ": "*/
	movb %r14b, %dil			/* put r14b in arg 1 for hex_write */
	
	call hex_write_string		/* write a ": " */
	movb $0, %r8b                    /*make r8 the for loop index*/
	jmp .LWriteHexSentenceForLoop	/*jump to the loop */

.LWriteHexSentenceForLoop:
	cmpb %r9b, %r8b
	je .LSpaceFormatLoopPre		/*when index hits line count, continue */
	
	movb (%r13b, %r8b), %dil		/* put line[index] in %rdi(dil, 1 byte %rdi) for arg one of hex_format_byte */
	movb %r15b, %sil			/* put hex_byte in for %sil for arg two of hex_format_byte */
	movb %sil, %dil			/*put the "returned" hex_byte in dil for arg for hex_write*/
	call hex_write_string
	movq single_space, %r14b	/*store a space in r14b*/
	movq %r14b, %dil			/*make the single space the arg for hex_write*/
	call hex_write_string
	incb %r8b
	jmp .LWriteHexSentenceForLoop	/*loop again!*/


.LSpaceFormatLoopPre:
	movb $0, %r8b                    /*make r8 the for loop index*/
	movb $16, %cl
	subb %r9b, %cl			/* make rcx hold 16 - linecount*/
	movb three_spaces, %r14b	/* store three spaces in %r14b */
	movq %r14b, %dil		/*make 3 spaces be the arg for hex_write*/
	jmp .LSpaceFormatLoop

.LSpaceFormatLoop:
	cmpb %cl, %r8b			/*if loop index equals 16-linecount*/
	je .LFormatAndLoopPrep		/*jump toformat and prepare for the next for  loop*/
	call hex_write_string		/*write three spaces*/
	incb %r8b			/*increment the loop index */

.LFormatAndLoopPrep:
	movb $0, %r8b			/*make r8b the loop index*/
	jmp .LMakeLinePrintableForLoop	/*jump to make the line printable*/

.LMakeLinePrintableForLoop:
	cmpb %r9b, %r8b
	jmp .LwhileLoopEnd		/*if the index equals the line count jump to the end of the while loop*/
	movb (%r13, %r8), %dil		/*put line[index] in arg1 for hex_to_printable */
	call hex_to_printable
	movb %al, (%bl, %r8b)		/*put the returned printable character in hex_printable[index]*/
	incb %r8b			/*increment the index*/

.LwhileLoopEnd:
	movb %bl, %dil			/*make hex_printable arg 1 for hex_write*/
	call hex_write_string
	movb newline, %r14b		/*store a newline in %r14*/
	movb %r14b, %dil			/*put a newline in arg 1 for hex_write*/
	call hex_write_string
	addb %r9b, %r10b			/* add line count to offset*/

	movb %r10b, %dil                 /*make offset arg 1 for hex_format_offset*/
        movb offset_formatted, %sil     /*put offset_formatted in sil, arg 2 for it*/
        call hex_format_offset
        movb %sil, %r12b                 /*store offset_formatted in r12*/
	jmp .LreadwhileLoop		/*start the while loop again */ 
	
.LDone:
	popb %rbx
	popb %r15
	popb %r14
	popb %r13
	popb %r12
	
	addq $54, %rsp
	ret

/* vim:ft=gas:
 */
