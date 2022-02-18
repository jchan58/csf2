// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(int argc, char *argv[]){
  //read the input file
  int line_count = 0;
  char line[16];
  char hex_sentence[16];
  char hex_printable[16];
  //char hex_formatted_address[dont know the size];
  //i think this is how it's done
  while((line_count = hex_read(line)) != 0){
    //hex_format_offset the address 
    //hex_write_string(write the formatted address column, and write a space
    hex_write_string(":  ");

    //make sure line is null terminated
    line[line_count] = '\0';
    
    for(int i = 0; i < line_count; i++){
      //convert the sentence into a hex sentence
      hex_format_byte_as_hex(line[i], hex_sentence);
    }
    
    //write the sentence segment in hexadecimal
    hex_write_string(hex_sentence);
    hex_write_string("  ");

    //make sure all the sentence values are printable
    for(int i = 0; i < line_count; i++){
      //convert the sentence into a hex sentence
      hex_printable[i] = hex_to_printable(line[i]);
    }

    //print a line 
    hex_write_string(hex_printable);

    //write a newline
    hex_write_string("\n");
  
}

