
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
	pushq %rbx
	pushq %rbp
	pushq %r15
	subq $48, %rsp			// 17 + 3 + 17 + 9 + 8 + 8	
	movq %rsp, %rbp			//store where rsp points to hold the strings
	pushq %r14
	pushq %r13
	pushq %r12
	movq $0, %r12			/*r12 starts as index 0*/
	

					/* 8(%rbp is start of line */
					/* 25(%rbp) is start of hexbyte */
					/* 28(%rbp) start of hex_printable */
					/* 45(%rbp) is start of offset_formatted */
	movb $0, 24(%rbp, %r12, 1)      /*null terminate line */
	movb $0, 44(%rbp, %r12, 1)  	/*null terminate hex_printable*/
	movb $0, 27(%rbp, %r12, 1)	/*null terminate hex_byte*/
	movb $0, 53(%rbp, %r12, 1)	/*null terminate format_offset*/     	


	 movb $0, 8(%rbp, %r12, 1)      /*make sure the buffers are null terminated*/
        movb $0, 9(%rbp, %r12, 1)
        movb $0, 10(%rbp, %r12, 1)
        movb $0, 11(%rbp, %r12, 1)
        movb $0, 12(%rbp, %r12, 1)
        movb $0, 13(%rbp, %r12, 1)
        movb $0, 14(%rbp, %r12, 1)
        movb $0, 15(%rbp, %r12, 1)
        movb $0, 16(%rbp, %r12, 1)
        movb $0, 17(%rbp, %r12, 1)
        movb $0, 18(%rbp, %r12, 1)
        movb $0, 19(%rbp, %r12, 1)
        movb $0, 20(%rbp, %r12, 1)
        movb $0, 21(%rbp, %r12, 1)
        movb $0, 22(%rbp, %r12, 1)
        movb $0, 23(%rbp, %r12, 1)
        movb $0, 24(%rbp, %r12, 1)
        movb $0, 25(%rbp, %r12, 1)
        movb $0, 26(%rbp, %r12, 1)
        movb $0, 27(%rbp, %r12, 1)
        movb $0, 28(%rbp, %r12, 1)
        movb $0, 29(%rbp, %r12, 1)
        movb $0, 30(%rbp, %r12, 1)
        movb $0, 31(%rbp, %r12, 1)
        movb $0, 32(%rbp, %r12, 1)
        movb $0, 33(%rbp, %r12, 1)
        movb $0, 34(%rbp, %r12, 1)
        movb $0, 35(%rbp, %r12, 1)
        movb $0, 36(%rbp, %r12, 1)
        movb $0, 37(%rbp, %r12, 1)
        movb $0, 38(%rbp, %r12, 1)
        movb $0, 39(%rbp, %r12, 1)
        movb $0, 40(%rbp, %r12, 1)
        movb $0, 41(%rbp, %r12, 1)
        movb $0, 42(%rbp, %r12, 1)
        movb $0, 42(%rbp, %r12, 1)
        movb $0, 44(%rbp, %r12, 1)

	

	movq $0, %r14 			/* %r10 holds offset */
	movq %r14, %rdi 		/*make offset arg 1 for hex_format_offset*/
	leaq 45(%rbp, %r12, 1), %rsi 	/*put offset_formatted in rsi, arg 2 for it*/
	call hex_format_offset
	jmp .LreadwhileLoop		/*start reading from the input*/

.LreadwhileLoop:
	leaq 8(%rbp, %r12, 1), %rdi	/*make line the argument for hex_read*/
	call hex_read
	movq %rax, %rbx			/* %rbx stores line count given from hex_read*/
	movq $0, %r12			/*%r12 stores 0 for this comparison*/
	cmpq %rbx, %r12
	je .LDone			/* if nothing is read (rbx is 0) you are done*/	
	leaq 45(%rbp, %r12, 1), %rdi 	/*make offset_formated the argument for hex_write_string*/
	call hex_write_string

	movq $colon_space, %r11		/*r11 holds print formatting strings, here put in ": "*/
	movq %r11, %rdi			/* put r11 in arg 1 for hex_write */
	
	call hex_write_string		/* write a ": " */
	movq $0, %r12                   /*make r12 the for loop index*/
	jmp .LWriteHexSentenceForLoop	/*jump to the loop */

.LWriteHexSentenceForLoop:
	cmpq %rbx, %r12
	je .LSpaceFormatLoopPre		/*when index hits line count, continue */
	
	movb 8(%rbp, %r12, 1), %dil     /* put line[index] in %rdi(dil, 1 byte %rdi) for arg one of hex_format_byte */
	leaq 25(%rbp, %r12, 1), %rsi	/* put hex_byte in for %rsi for arg two of hex_format_byte */
	call hex_format_byte_as_hex
	movq %rsi, %r15			/*put the "returned" hex_byte in r15 */
	movq $2, %r13			//make r13 hold 2
	movb $0, (%r15, %r13)		/*null terminate hex_byte*/ 
	movq %r15, %rdi			/*make the null terminated hex_byte the argument for hex_write*/
	call hex_write_string
	movq $single_space, %r11	/*store a space in r11*/
	movq %r11, %rdi			/*make the single space the arg for hex_write*/
	call hex_write_string
	incq %r12			/*increment the index in line*/
	
	jmp .LWriteHexSentenceForLoop	/*loop again!*/


.LSpaceFormatLoopPre:
	movq $0, %r12                    /*make r12 the for loop index*/
	movq $16, %r13
	subq %rbx, %r13			/* make r13 hold 16 - linecount*/
	jmp .LSpaceFormatLoop

.LSpaceFormatLoop:
	cmpq %r13, %r12			/*if loop index equals 16-linecount*/
	je .LFormatAndLoopPrep		/*jump toformat and prepare for the next for  loop*/
	movq $three_spaces, %r11	/*store three spaces in %r11*/
	movq %r11, %rdi         	/*make 3 spaces be the arg for hex_write*/
	call hex_write_string		/*write three spaces*/
	incq %r12			/*increment the loop index */
	jmp .LSpaceFormatLoop
	
.LFormatAndLoopPrep:
	movq $0, %r12			/*make r12 the loop index*/
	movq $single_space, %r11 	/* store a space in %r11 */
	movq %r11, %rdi 		/*move the space into arg 1 for hex write*/
	call hex_write_string
	jmp .LMakeLinePrintableForLoop	/*jump to make the line printable*/

.LMakeLinePrintableForLoop:
	cmpq %rbx, %r12
	je .LwhileLoopEnd		/*if the index equals the line count jump to the end of the while loop*/
	movb 8(%rbp, %r12, 1), %dil	/*put line[index] in arg1 for hex_to_printable */
	call hex_to_printable
	movb %dil, 28(%rbp, %r12, 1) 	/*put the "returned" printable character in hex_printable*/
	incq %r12			/*increment the index*/
	jmp .LMakeLinePrintableForLoop
	
.LwhileLoopEnd:
	movq $0, %r12
	leaq 28(%rbp, %r12, 1), %rdi	/*make hex_printable arg 1 for hex_write*/
	call hex_write_string
	movq $newline, %r11		/*store a newline in %r11*/
	movq %r11, %rdi			/*put a newline in arg 1 for hex_write*/
	call hex_write_string
	addq %rbx, %r14			/* add line count to offset*/

	movq %r14, %rdi                 /*make offset arg 1 for hex_format_offset*/
        leaq 45(%rbp, %r12, 1), %rsi     /*put offset_formatted in rsi, arg 2 for it*/
        call hex_format_offset

	movb $0, 8(%rbp, %r12, 1)      /*make sure the buffers are null terminated*/
        movb $0, 9(%rbp, %r12, 1)     
        movb $0, 10(%rbp, %r12, 1)
	movb $0, 11(%rbp, %r12, 1)
        movb $0, 12(%rbp, %r12, 1)
	movb $0, 13(%rbp, %r12, 1)
	movb $0, 14(%rbp, %r12, 1)
	movb $0, 15(%rbp, %r12, 1)	
	movb $0, 16(%rbp, %r12, 1)
	movb $0, 17(%rbp, %r12, 1)
	movb $0, 18(%rbp, %r12, 1)
	movb $0, 19(%rbp, %r12, 1)
	movb $0, 20(%rbp, %r12, 1)
	movb $0, 21(%rbp, %r12, 1)
	movb $0, 22(%rbp, %r12, 1)
	movb $0, 23(%rbp, %r12, 1)
	movb $0, 24(%rbp, %r12, 1)
	movb $0, 25(%rbp, %r12, 1)
	movb $0, 26(%rbp, %r12, 1)
	movb $0, 27(%rbp, %r12, 1)
	movb $0, 28(%rbp, %r12, 1)
	movb $0, 29(%rbp, %r12, 1)
	movb $0, 30(%rbp, %r12, 1)
	movb $0, 31(%rbp, %r12, 1)
	movb $0, 32(%rbp, %r12, 1)
	movb $0, 33(%rbp, %r12, 1)
	movb $0, 34(%rbp, %r12, 1)
	movb $0, 35(%rbp, %r12, 1)
	movb $0, 36(%rbp, %r12, 1)
	movb $0, 37(%rbp, %r12, 1)
	movb $0, 38(%rbp, %r12, 1)
	movb $0, 39(%rbp, %r12, 1)
	movb $0, 40(%rbp, %r12, 1)
	movb $0, 41(%rbp, %r12, 1)
	movb $0, 42(%rbp, %r12, 1)
	movb $0, 42(%rbp, %r12, 1)
	movb $0, 44(%rbp, %r12, 1)

	jmp .LreadwhileLoop		/*start the while loop again */ 
	
.LDone:
	movq $0, %rax
	popq %r12
	popq %r13
	popq %r14
	popq %r15
	popq %rbp
	popq %rbx
	addq $56, %rsp
	ret

/* vim:ft=gas:
 */
