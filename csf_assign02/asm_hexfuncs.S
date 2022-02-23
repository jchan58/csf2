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
	subq $8, %rsp 			/* adjust the stack pointer */
	movq $0, %r10			/* %r10 is var named divides */
	movq %rdi, %r11			/* %r11 is a copy of byteval */
	movq $0, %rcx 			/* %rcx will be used to compare */
	movl $2, %r10d			/* %r10d will be used to store d */
	movl $0, %r11d			/* %r11d will be used to store index */
	movb $32, %r10b			/* %r10b is from_int and a space in begining */
	movq $0, %rcx			/* %rcx will be used as an index in for loop */
.whileLoop:
	cmpq $0, %r11                 /* %r10d see if byte_copy has reached 0 */
	je .nextLines		      /* if it has jump to next command */
	shrq $4, %r11 		      /* %r11d shift right four times */
	incq %r10		      /* %r10d increment divides */
	jmp .whileLoop 
	
.nextLines:
	movb $48, (%rsi, %r11d, 1)	/* set index 0 of array equal to '0' */
	incl %r11d			/* increment index set in %r11d */
	movb $48, (%rsi, %r11d, 1)	/* set index 1 of array equal to '0' */
	jmp .fillSbuf
	
.fillSbuf:
	cmpq %r10, %rcx			/* comparing index with divides */
	jae .Done			/* if divides is 0 or less go to done*/
	jmp .findFromInt		/* if not jump to if statement*/
	
.findFromInt:
	divb 16(%rdi) 			 /* divide byteval by 16 */
	movb %al, %rdi 			 /* byteval is quotient is %al */
	cmpb $10, %ah			 /* %ah is reaminder */
	jae .findElse			 /* if %ah >= 10, do else */
	addb $48, %ah			 /* if not add 48 to remainder */
	movb %ah, %r10b			 /* make from_int the above value*/
	jmp .setSbuf			 /*then set sbuf */

.findElse:
	addb $87, %ah			  /* if not add 87 to remainder */
	movb %ah, %r10b			  /* make from_int the above value*/
	jmp .setSbuf                      /*then set sbuf */

.setSbuf:
	subl $1, %r10d 			 /*subtract one from d*/
	movb %r10b, (%rsi, %r10d, 1)	 /*fill sbuf[d] (is d - 1) */
	incq %r10			 /*increment the for loop index*/
	jmp .fillSbuf			 /*go to the for loop header*/

.Done:
	movb $0, (%rsi, $2, 1)   	  /* null terminate sbuf */
	ret				  /* return*/
/* vim:ft=gas:
 */
