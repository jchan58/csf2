/* Assembly language implementation of hexdump functions */

	.section .rodata

	/* TODO: add read-only data such as strings here */

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
	movl $31, %r11d               /* %r11d is the store temporary value */
	movl $0, %r10d                /* %r10d is the temp val later to compare */
	movl %rsi, %eax                /* %eax will be returning value */
.ifLoop:
	cmpl %r10d, %rdi              /* see if first parameter %rdi is >= %r10d */
        jbe .LLoopDone                /* if byteval <= 0, continue to next statement*/
	cmpl %r11d, %rdi	      /* see if first parameter %rdi >= %r11d */
	jae .LLoopDone
	
.LLoopDone:
	addq $8, rsp		     /* adjust stack pointer alignment */
	movl $46, %eax		     /* move byteval in %eax */
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
	subq $8, rsp 			/* adjust the stack pointer */
	movl $0, %r10d			/* %r10d is set equal to 0 */
	movl %rdi, %r11d		/* %r11d is a copy of value in %rdi */
	movl $0, %rcx 			/* %rcx will be used to store temp */
.whileLoop:
	cmpl $0, %r11d                 /* %r10d see if byte_copy has reached 0 */
	je .nextLines		       /* if it has jump to next command */
	shrl $4, %r11d 		       /* %r11d shift right four times */
	incl %r10d		       /* %r10d increment divides */
	
.nextLines:
	movl %r10d, %rcx	      /* move the the value into divides */
	
	





	

/* Parameters:
 *   array (%rdi)
 *   n (%esi) - length of the array
 *
 */

	.globl toLower

toLower:
	subq $8, %rsp
	movl $0, %r10d                /* %r10d is the array index */
	movl $32, %r11d		      /* %r11d is the value that needs to be added */	
.LLoop:
	cmpl %esi, %r10d              /* see if index has reached length */
	jae .LLoopDone		      /* if index >= length, done with loop */
	jmp .IfLoop		      /* jump to the if statement*/
.IfLoop:
	cmpl  $65, (%rdi, %r10, 1)
	jbe .LLoop                    /* if sbuf[i] <= 65, reloop to for */
        cmpl  $90, (%rdi, %r10, 1)   /*compare the values */
        jae .LLoop 	              /* if sbuf[i] >= 90, reloop to for */
	addl %r11d, (%rdi, %r10, 1)   /* add 32 to the array element */
	incl %r10d                    /* increment index */
	jmp .LLoop                    /* continue loop */
	
.LLoopDone:
	addq $8, %rsp 
	ret                           /* doesn't return anything */



/* vim:ft=gas:
 */
