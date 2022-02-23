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
	subq $32, %rsp 			/* adjust the stack pointer */
	movq $0, %r10			/* %r10 is var named divides */
	movq %rdi, %r11			/* %r11 is a copy of byteval */
	movq $0, %rcx 			/* %rcx will be used to compare */
	movq $2, %r12			/* %r12 will be used to store d */
	movq $0, %r8			/* %r8 will be used to store index for sbuf*/
	movb $32, %r10b			/* %r10b is from_int and a space in begining(unusued?) */
	movq $0, %rcx			/* %rcx will be used as an index in for loop */
.whileLoop:
	cmpq $0, %r11                 /* %r10d see if byte_copy has reached 0 */
	je .nextLines		      /* if it has jump to next command */
	shrq $4, %r11 		      /* %r11d shift right four times */
	incq %r10		      /* %r10d increment divides */
	jmp .whileLoop 
	
.nextLines:
	movq $48, (%rsi, %r8, 1)	/* set index 0 of array equal to '0' */
	incq %r8			/* increment index set in %r11d */
	movq $48, (%rsi, %r8, 1)	/* set index 1 of array equal to '0' */
	jmp .fillSbuf
	
.fillSbuf:
	cmpq %r10, %rcx			/* comparing index with divides */
	jae .Done			/* if divides is 0 or less go to done*/
	jmp .findFromInt		/* if not jump to if statement*/
	
.findFromInt:
	movq %rdi, %rax                 /*div does rax + rdx / given */
	movq $0, %rdx
	movq $16, %r9                   /*make 16 be 8 bytes (match sizes for operatioin*/  
	divq %r9 			 /* divide byteval by 16 */
	movq %rax, %rdi 		 /* byteval is quotient is in %rax */
	cmpq $10, %rdx			 /* %rdx is reaminder */
	jae .findElse			 /* if %rdx >= 10, do else */
	addq $48, %rdx                   /* if not add 48 to remainder */
	movq $0, %r9                      /*empty r9 (from int)*/
	movq %rdx, %r9		         /* make from_int the above value*/
	jmp .setSbuf			 /*then set sbuf */

.findElse:
	addq $87, %rdx			  /* if not add 87 to remainder */
	movq %rdx, %r9			  /* make from_int the above value*/
	jmp .setSbuf                      /*then set sbuf */

.setSbuf:
	movq %r12,%r14                   /*copy of d*/
	decq %r14                       /*sub tract from copy of d*/
	decq %r12 			 /*subtract one from d*/
	movq %r9, (%rsi, %r14, 1)       /*fill sbuf[d] (is d - 1) */
	incq %r10			 /*increment the for loop index*/
	jmp .fillSbuf			 /*go to the for loop header*/

.Done:
	movq $2, %r13               /*make index 2 the right byte size (8)*/
	movq $0, (%rsi, %r13, 1)    /* null terminate sbuf */
	addq $32, %rsp               /*pop and realine stack pointer*/
	popq %r13
	popq %r12
	pushq %r14
	ret				  /* return*/
/* vim:ft=gas:
 */
