/* Assembly language implementation of hexdump functions */

	.section .rodata
hex_digits:
	.string "0123456789abcdef"
	
	.section .data


	.section .text


/*                                                                             
 * Reading from the file                                                       
 *                                                                             
 * Parameters:                                                                 
 *   data_buff (%rdi) - char array                                             
 *                                                                             
 * Returns:                                                                    
 *  the count of characters in the array                                       
 */
	
	.globl hex_read
	
hex_read:
	subq $8, %rsp
       	movq %rdi, %rsi			  /* %rsp is the stack ptr so align it */
	movq $0, %rdi 		  /* %rdi calls readOnly value */ 	
	movq $16, %rdx 	  	  /* %rdx gives the max bytes */
	call read		  /* call the read syscall */
	addq $8, %rsp		  /* %rsp is the stack ptr so realign it */
	ret			  /* return the count */


	.globl hex_write_string

hex_write_string:
	subq $8, %rsp             /* %rsp is the stack ptr so align it */
	movq $0, %r10		  /* %r10 is the length of the string */
	movq $0, %r11		  /* %r11 is null terminator in ascii */
	jmp .LLoop
.LLoop:
	cmpq %r11, (%rdi, %r10)  /* compare the character in the array */
	je .LNext		  /* if the value in array is null, jump */
	incq %r10		  /* increment length */
	jmp .LLoop		  /* jump back in while loop */

.LNext:
	movq %rdi, %rsi           /* move what we will read into second args */
	movq $1, %rdi		  /* %rdi stores written argument */
	movq %r10, %rdx		  /* %r10 moves length val into %rdx */
	call write 		  /* make a syscall to write */
	addq $8, %rsp		  /* adjust stack pointer */
	ret 			  /* don't return anything */




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
	movq $31, %r11                /* %r11 is the stores 31 */
	movq $126, %r10                 /* %r10 stores 126*/
	movq %rdi, %rax               /* %rax will be returning value */

	cmpq %r10, %rdi              /* see if first parameter %rdi is >= %r10 */
        ja .LDot  				     /* if byteval <= 0, continue to next statement*/

	cmpq %r11, %rdi	      	     /* see if first parameter %rdi <= %r11d */
	jle .LDot
	jmp .LDonep

.LDot:
	movq $46, %rax
	jmp .LDonep
.LDonep:
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
	movq $hex_digits, %r15			/* %r15 is a string of hex characters */
.LwhileLoopb:
	cmpq $0, %r11                 /* %r10d see if byte_copy has reached 0 */
	je .LnextLinesb		      /* if it has jump to next command */
	shrq $4, %r11 		      /* %r11 shift right four times (/16)*/
	incq %r10		      /* %r10d increment divides */
	jmp .LwhileLoopb 
	
.LnextLinesb:
	movb 0(%r15), %r8b
	movb %r8b, 0(%rsi)   		/* set index 0 and 1 of array equal to '0' (put char in r8 first) */
	movb 1(%r15), %r8b
	movb %r8b, 1(%rsi)     
	jmp .LfillSbufForLoopb
	
.LfillSbufForLoopb:
	cmpq %r10, %rcx			/* comparing index with divides */
	jae .LDoneb			/* if divides i is greater than or equal to divides go to done*/
	jmp .LsetSbufb			/* if not jump to if statement*/
	
.LsetSbufb:
	movq %rdi, %r9                  
	andq $15, %r9			  /* %r9 is reaminder now*/
	shrq $4, %rdi		
					/*divide rdi by 16 */


	movq %r12,%r14                   /*copy of d*/
	decq %r14                       /*sub tract from copy of d*/
	decq %r12 			 /*subtract one from d*/
	movb (%r15, %r9), %r8b            /*get hex_digit at index r9 (remainder) and store it*/
        movb %r8b, (%rsi, %r14)         /*put the hex_digit in rsi(sbuf) at the appropriate index 9r10 */
	incq %rcx			 /*increment the for loop index*/
	jmp .LfillSbufForLoopb			 /*go to the for loop header*/

.LDoneb:
	movq $2, %r13               /*make index 2 the right byte size (8)*/
	movq $0, (%rsi, %r13, 1)    /* null terminate sbuf */
	addq $8, %rsp               /*pop and realine stack pointer*/
	popq %r15
	popq %r14
	popq %r13
	popq %r12
	ret				  /* return*/

  .globl hex_format_offset

hex_format_offset:
        pushq %r12
        pushq %r13
        pushq %r14
        pushq %r15
	pushq %rbx
	subq $8, %rsp                   /* adjust the stack pointer and push registers*/
        movq $0, %r10                   /* %r10 is var named divides */
        movq %rdi, %r11                 /* %r11 is a copy of offset */
        movq $0, %rcx                   /* %rcx will be used to compare with 0, then as the for loop i starting at 0 */
        movq $1, %r12                   /* %r12 is used to store d*/
        movq $hex_digits, %r15                  /* %r15 is a string of hex characters */
	jmp .LwhileLoopo

.LwhileLoopo:
        cmpq $0, %r11                 /* %r11 see if offset_copy has reached 0 */
        je .LfillSbufForLoopo                 /* if it has jump to next command */
        shrq $4, %r11                 /* %r11 shift right four times (/16)*/
        incq %r10                     /* %r10d increment divides */
        jmp .LwhileLoopo

.LfillSbufForLoopo:
        cmpq %r10, %rcx                 /* comparing index with divides */
        jae .LpadSbufForLoopPreo                       /* if divides i is greater than or equal to divides go to pad*/
        jmp .LsetSbufo                    /* if not jump to if statement*/

.LsetSbufo:
        movq %rdi, %r9
        andq $15, %r9                     /* %r9 is reaminder now*/
        shrq $4, %rdi
                                        /*divide rdi by 16 */


        movq $8,%r14                   /*holds 8*/
        subq %r12,%r14                       /*sub tract d from 8*/
        incq %r12                        /*add one to d*/
        movb (%r15, %r9), %bl            /*get hex_digit at index r9 (remainder) and store it*/
        movb %bl, (%rsi, %r14)         /*put the hex_digit in rsi(sbuf) at the appropriate index %r14 */
	incq %rcx                        /*increment the for loop index*/
        jmp .LfillSbufForLoopo                     /*go to the for loop header*/

.LpadSbufForLoopPreo:
	movq $0, %rcx			/*make rcx an index for the pad loop starting at 0*/
	movb 0(%r15), %bl 		/*store the character 0 in bl*/
	movq $8, %r14
	subq %r10, %r14			/*make %r14 8-divides*/
	jmp .LpadSbufForLoopo

.LpadSbufForLoopo:
	cmpq %r14, %rcx
	jae .LDoneo		/*done when rcx equals 8-divides*/
	movb %bl, (%rsi, %rcx)
	incq %rcx
	jmp .LpadSbufForLoopo /*increment and loop again*/

	
	
.LDoneo:
        movq $8, %r13               /*make r13 sbuf's last index (8)*/
        movb $0, (%rsi, %r13, 1)    /* null terminate sbuf */
        addq $8, %rsp               /*pop and realine stack pointer*/
	popq %rbx
	popq %r15
        popq %r14
        popq %r13
        popq %r12
        ret        
/* vim:ft=gas:
 */
