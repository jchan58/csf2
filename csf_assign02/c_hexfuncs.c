// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]){
  int count = 0;
  //return the count of the character read from the standard output
  count = read(0, data_buf, 16); 
  return count; 
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]){
  int length = 0;
  
  //to find the length of s 
  for(int k = 0; s[k] != '\0'; ++k){
    length++; 
  }
  
  write(1, s, length); 
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]){
  int divides = 0;
  char byteval_copy = byteval;

  //check how many times you will divide when converting
  while(byteval_copy != 0){
    byteval_copy/=16;
    divides++;
  }

  //make sbuf big enough
  //char sbuf[divides - 1];

  //we don't want to change divides
  int d = divides;
  
  //convert decimal to hex and store it in a string
  for(int i = 0; i < divides; i++){
    sbuf[d - 1] = byteval % 16;
    byteval/=16;
    d--;
  }
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval){
  //non-printable characters are 0-31; for them return a .
  if(byteval >= 0 && byteval <= 31){
    return 46;
  }

  //otherwise return the byteval unmodified
  return byteval;
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[]){
  int length = 0;

  //to find the length of s
  for(int k = 0; s[k] != '\0'; ++k){
    length++;
  }

  write(1, s, length);
}































