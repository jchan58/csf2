// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char *argv[]){
  //read the input file
  int line_count = 0;
  char line[17];
  char hex_byte[3];
  char hex_printable[17] = {'\0'};
  unsigned offset = 0;
  char offset_formatted[9] = {48};
  offset_formatted[8] = '\0';
  
  //format the offset (start position of the character leading the line)
  hex_format_offset(offset, offset_formatted);

  
  while((line_count = hex_read(line)) != 0){
   
    line[line_count] = '\0';
    hex_write_string(offset_formatted);
    
    
    hex_write_string(": ");

    
    for(int i = 0; i < line_count; i++){
      //convert the sentence into a hex sentence
      hex_format_byte_as_hex(line[i], hex_byte);
      hex_write_string(hex_byte);
      hex_byte[2] = '\0';
      hex_write_string(" "); 
    }
    
    //add the appropriate blank space (2 chars + 1 space is 3)
    for(int i = 0; i < 16-line_count; i++){
      hex_write_string("   ");
    }


    
      hex_write_string(" ");
      

    //make sure all the sentence values are printable
    for(int i = 0; i < line_count; i++){
      //convert the sentence into a hex sentence
      hex_printable[i] = hex_to_printable(line[i]);
    }

    hex_printable[line_count] = '\0';
    
    //print a line 
    hex_write_string(hex_printable);

    //write a newline
    hex_write_string("\n");
  
    //write the next offset
    offset += line_count;
    hex_format_offset(offset, offset_formatted);

     //make sure line and hex_printable and hex_byte are properly is null terminated
    for(int i = 0; i < 17; i++){
      hex_printable[i] = '\0';
    }
 
      for(int i = 0; i < 17; i++){
         line[i] = '\0';
      }
     
      for(int i = 0; i < 3; i++){
	hex_byte[i] = '0';
        hex_byte[3] = '\0';
      }


  }

  return 0;
}

