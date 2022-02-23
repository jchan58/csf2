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

	subq $8, %rsp		      /* %rsp to adjust stack pointer*/
	movq $31, %r11                /* %r11d is the store temporary value */
	movq $0, %r10                 /* %r10d is the temp val later to compare */
	movq %rsi, %rax               /* %rax will be returning value */

.ifLoop:
	cmpq %r10, %rdi              /* see if first parameter %rdi is >= %r10d */
        jbe .LLoopDone               /* if byteval <= 0, continue to next statement*/
	cmpq %r11, %rdi	      	     /* see if first parameter %rdi >= %r11d */
	jae .LLp
	
.LLp:
	movq $46, %rax		     /* move byteval in %rax */
	addq $8, %rsp  		     /* adjust stack pointer alignment */
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
	movq $0, %r10			/* %r10d is set equal to 0 */
	movq %rdi, %r11		/* %r11d is a copy of value in %rdi */
	movq $0, %rcx 			/* %rcx will be used to store temp */
.whileLoop:
	cmpq $0, %r11                 /* %r10d see if byte_copy has reached 0 */
	je .nextLines		      /* if it has jump to next command */
	shrq $4, %r11 		      /* %r11d shift right four times */
	incq %r10		      /* %r10d increment divides */
	
.nextLines:
	movq %r10, %rcx	      	     /* move the the value into divides */
	
	





	

/* Parameters:
 *   array (%rdi)
 *   n (%rsi) - length of the array
 *
 */

	.globl toLower

toLower:
	subq $8, %rsp
	movq $0, %r10                /* %r10d is the array index */
	movq $32, %r11		      /* %r11d is the value that needs to be added */	
.LLoop:
	cmpq %rsi, %r10              /* see if index has reached length */
	jae .LLoopDone		      /* if index >= length, done with loop */
	jmp .IfLoop		      /* jump to the next loop */ 
.IfLoop:
	cmpq  $65, (%rdi, %r10, 1)
	jbe .LLoop                    /* if sbuf[i] <= 65, reloop to for */
        cmpq  $90, (%rdi, %r10, 1)   /*compare the values */
        jae .LLoop 	              /* if sbuf[i] >= 90, reloop to for */
	addq %r11, (%rdi, %r10, 1)   /* add 32 to the array element */
	incq %r10                    /* increment index */
	jmp .LLoop                    /* continue loop */
	
.LLoopDone:
	addq $8, %rsp 
	ret                           /* doesn't return anything */



/* vim:ft=gas:
 */
