#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdarg>
#include <cerrno>
#include <cstdint>
#include <elf.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include <fcntl.h>
#include <string>
#include "elf_names.h"
#include <stdint.h>

//ours
#include <iostream>

using std::cerr;
using std::string;
using std::cout; 

void printEndian(int endian) {
  int executable = false; 

  if(endian == 2) {
    cout << "Endianness: " << "Big endian" << "\n";
  } else {
    cout << "Endianness: " << "Little endian" << "\n";
  }

}



int main(int argc, char **argv) {
  size_t file_size;

  //open file
  int fd = open(argv[1], O_RDONLY);
  if(fd < 0) {
    cerr << "Cannot open file \n";
    return 1;
  }

  //get file byte size
  struct stat statbuf;
  int rc = fstat(fd, &statbuf);
  if (rc != 0) {
    cerr << "Error getting file size \n";
    return 2;
  } else {
    file_size = statbuf.st_size;
  }

  //map the file contents to a region of memory
  void *data = mmap(NULL, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if(data == ((void *)-1)) {
    cerr << "File cannot be mapped to memory \n";
    return 3;
  }

  unsigned char * char_data = (unsigned char *) data;

 
  //check if it is an elf file
  if(char_data[0] != 0x7f || char_data[1] != 'E' || char_data[2] != 'L' || char_data[3] != 'F' ){
    cerr << "Not an ELF file \n";
    return 3;
  }

  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  int endian = elf_header->e_ident[EI_DATA];
  
  string type = get_type_name(elf_header->e_type);
  string machine = get_machine_name(elf_header->e_machine);
  
  
  uint16_t numSectionHeaders = elf_header->e_shnum;
  //is this a pointer?
  Elf64_Ehdr *section_table = elf_header.e_shstrndx;

  //scan throught the section headers, idk what data type it is...
  for(size_t i = 0; i < section_table.length; i++) {
    //store how many bytes of the section header you have looked at 
    char looked = 0;
    //store an array of section info, is there more we need?
    char * section_name_arr;
    //is this even how you make an int array
    int * section_size_arr;

    //how do we get the section type? and size?

    int k = 0;
    //while there is space left to look at in the header, store the section 
    //names in an array
    while(looked < section_table[i].length) {
      section_name_arr[k] = section_table[i] + looked;
      //increment the number of bytes looked at by the space between 
      //the section names
      looked += section_table[i] + section_table->sh_name;
    }
    
  }

  cout << "Object file type: " << type << "\n";
  cout << "Instruction Set: " << machine << "\n";
  cout << "Endianness: " << printEndian(endian) << "\n";
  





  
}
