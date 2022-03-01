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
	subq $8, %rsp
	pushq %rbx
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	movq $0, %r10 			/*this holds offset */
	movq %r10, %rdi 		/*make offset arg 1 for hex_format_offset*/
	movq offset_formatted, %rsi	/*put offset_formatted in rsi, arg 2 for it*/
	call hex_format_offset
	movq %rsi, %r12			/*store offset_formatted in r12*/
	movq line, %r13			/*store line in %r13*/
	movq hex_byte, %r15		/* put hex_byte in %r15*/
	movq hex_printable, %rbx	/* put hex_printable in rbx */
	jmp .LreadwhileLoop		/*start reading from the input*/

.LreadwhileLoop:
	movq %r13, %rdi			/*make line the argument for hex_read*/
	call hex_read
	movq %rax, %r9			/*%r9 stores line count given from hex_read*/
	movq $0, %r8			/*%r8 stores 0 just for this comparison*/
	cmpq %r9, %r8
	je .LDone			/* if nothing is read (r9 is 0) you are done*/

	movq %r12, %rdi			/*make offset_formated the argument for hex_write_string*/
	call hex_write_string

	movq colon_space, %r14		/*r14 holds print formatting strings or loop counters, here put in ": "*/
	movq %r14, %rdi			/* put r14 in arg 1 for hex_write */
	
	call hex_write_string		/* write a ": " */
	movq $0, %r8                    /*make r8 the for loop index*/
	jmp .LWriteHexSentenceForLoop	/*jump to the loop */

.LWriteHexSentenceForLoop:
	cmpq %r9, %r8
	je .LSpaceFormatLoopPre		/*when index hits line count, continue */
	movb (%r13, %r14), %dil		/* put line[index] in %rdi(dil, 1 byte %rdi) for arg one of hex_format_byte */
	movq %r15, %rsi			/* put hex_byte in for %rsi for arg two of hex_format_byte */
	movq %rsi, %rdi			/*put the "returned" hex_byte in rdi for arg for hex_write*/
	call hex_write_string
	movq single_space, %r14		/*store a space in r14*/
	movq %r14, %rdi			/*make the single space the arg for hex_write*/
	call hex_write_string
	incq %r8
	jmp .LWriteHexSentenceForLoop	/*loop again!*/


.LSpaceFormatLoopPre:
	movq $0, %r8                    /*make r8 the for loop index*/
	movq $16, %rcx
	subq %r9, %rcx			/*make rcx hold 16 - linecount*/
	movq three_spaces, %r14		/* store three spaces in %r14 */
	movq %r14, %rdi			/*make 3 spaces be the arg for hex_write*/
	jmp .LSpaceFormatLoop

.LSpaceFormatLoop:
	cmpq %rcx, %r8			/*if loop index equals 16-linecount*/
	je .LFormatAndLoopPrep		/*jump toformat and prepare for the next for  loop*/
	call hex_write_string		/*write three spaces*/
	incq %r8			/*increment the loop index */

.LFormatAndLoopPrep:
	movq $0, %r8			/*make r8 the loop index*/
	jmp .LMakeLinePrintableForLoop	/*jump to make the line printable*/

.LMakeLinePrintableForLoop:
	cmpq %r9, %r8
	jmp .LwhileLoopEnd		/*if the index equals the line count jump to the end of the while loop*/
	movb (%r13, %r8), %dil		/*put line[index] in arg1 for hex_to_printable */
	call hex_to_printable
	movb %al, (%rbx, %r8)		/*put the returned printable character in hex_printable[index]*/
	incq %r8			/*increment the index*/

.LwhileLoopEnd:
	movq %rbx, %rdi			/*make hex_printable arg 1 for hex_write*/
	call hex_write_string
	movq newline, %r14		/*store a newline in %r14*/
	movq %r14, %rdi			/*put a newline in arg 1 for hex_write*/
	call hex_write_string
	addq %r9, %r10			/* add line count to offset*/

	movq %r10, %rdi                 /*make offset arg 1 for hex_format_offset*/
        movq offset_formatted, %rsi     /*put offset_formatted in rsi, arg 2 for it*/
        call hex_format_offset
        movq %rsi, %r12                 /*store offset_formatted in r12*/
	jmp .LreadwhileLoop		/*start the while loop again */ 
	
.LDone:
	popq %rbx
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	
	addq $8, %rsp
	ret

/* vim:ft=gas:
 */
