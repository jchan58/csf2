/* Assembly language implementation of hexdump functions */

	.section .rodata

	.section .data
arr:	.byte 40

	.section .text
/*
 * Prints the hex 
 *
 * Parameters:
 *   byteval (%rdi) - unsigned char
 *
 * Returns:
 *  the byteval
 */

	.globl hex_to_printable
	
hex_to_printable:
	movq $31, %r11                /* %r11 is the store temporary value */
	movq $0, %r10                 /* %r10 is the temp val later to compare */
	movq %rdi, %rax               /* %rax will be returning value */
	movl $0, %r10d

.ifLoop:
	cmpq %r10, %rdi              /* see if first parameter %rdi is >= %r10 */
        jbe .Lp  				     /* if byteval <= 0, continue to next statement*/
	cmpq %r11, %rdi	      	     /* see if first parameter %rdi <= %r11d */
	jae .Lp
	movq $46, %rax
	jmp .Lp
.Lp:
	ret


	.globl hex_format_offset
	
hex_format_offset:

	ret


/* Parameters:                                                                  
 *   byteval (%rdi)                                                               
 *   sbuf (%rsi) - the actual array passed in                                              
 *                                                                              
 */

	.globl hex_format_byte_as_hex
	
hex_format_byte_as_hex:
	pushq %r12
	pushq %r13
	pushq %r14
	pushq %r15
	subq $8, %rsp 			/* adjust the stack pointer and push registers*/
	movq $0, %r10			/* %r10 is var named divides */
	movq %rdi, %r11			/* %r11 is a copy of byteval */
	movq $0, %rcx 			/* %rcx will be used to compare with 0, then as the for loop i starting at 0 */
	movq $2, %r12			/* %r12 will be used to store d*/
	movq $0, %r8			/* %r8 will be used to store index for sbuf*/
	movq $32, %r15			/* %r15b is from_int and a space in begining(unusued?) */
.whileLoop:
	cmpq $0, %r11                 /* %r10d see if byte_copy has reached 0 */
	je .nextLines		      /* if it has jump to next command */
	shrq $4, %r11 		      /* %r11 shift right four times (/16)*/
	incq %r10		      /* %r10d increment divides */
	jmp .whileLoop 
	
.nextLines:
	movq $48, (%rsi, %r8, 1)	/* set index 0 of array equal to '0' */
	incq %r8			/* increment index set in %r11d */
	movq $48, (%rsi, %r8, 1)	/* set index 1 of array equal to '0' */
	jmp .fillSbufForLoop
	
.fillSbufForLoop:
	cmpq %r10, %rcx			/* comparing index with divides */
	jae .Done			/* if divides i is greater than or equal to divides go to done*/
	jmp .findFromInt		/* if not jump to if statement*/
	
.findFromInt:
	movq %rdi, %r9                  
	andq $15, %r9			  /* %r9 is reaminder now*/
	shrq $4, %rdi		
	cmpq $10, %r9			 /*divide rdi by 16 */
	jae .findElse			 /* if %r9 >= 10, do else */
	addq $48, %r9                   /* if not add 48 to remainder */
	movq %r9, %r15		         /* make from_int the above value*/
	jmp .setSbuf			 /*then set sbuf */

.findElse:
	addq $87, %r9			  /* if not add 87 to remainder */
	movq %r9, %r15			  /* make from_int the above value*/
	jmp .setSbuf                      /*then set sbuf */

.setSbuf:
	movq %r12,%r14                   /*copy of d*/
	decq %r14                       /*sub tract from copy of d*/
	decq %r12 			 /*subtract one from d*/
	movq %r15, (%rsi, %r14, 1)       /*fill sbuf[d -1] with from_int */
	incq %rcx			 /*increment the for loop index*/
	jmp .fillSbufForLoop			 /*go to the for loop header*/

.Done:
	movq $2, %r13               /*make index 2 the right byte size (8)*/
	movq $0, (%rsi, %r13, 1)    /* null terminate sbuf */
	addq $8, %rsp               /*pop and realine stack pointer*/
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	ret				  /* return*/
/* vim:ft=gas:
 */
