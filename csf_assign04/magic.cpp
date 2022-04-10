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

int main(int argc, char **argv) {
  size_t file_size;
  Elf64_Ehdr * elf; 
  Elf64_Shdr * shdr; 
  unsigned char * shrtab; 

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

  //pointer to the header of the elf file 
  Elf64_Ehdr *elf_header = (Elf64_Ehdr *) data;
  int endian = elf_header->e_ident[EI_DATA];
  
  //to help get access to the specific type and machine name of file
  string type = get_type_name(elf_header->e_type);
  string machine = get_machine_name(elf_header->e_machine);

  cout << "Object file type: " << type << "\n";
  cout << "Instruction Set: " << machine << "\n";
  if(endian == 2) {
    cout << "Endianness: " << "Big endian" << "\n";
  } else {
    cout << "Endianness: " << "Little endian" << "\n";
  }
  
  uint16_t numSectionHeaders = elf_header->e_shnum;
  //print out all the sections in the elf file 
  for(int i = 0; i < numSectionHeaders; i++) {
    elf = (Elf64_Ehdr *) data;
    //the section header that contains the section entries 
    shdr = (Elf64_Shdr *) (data + elf->e_shoff);
    //used to access each specific section accessed by indexing 
    shrtab = (unsigned char *)(data + shdr[elf->e_shstrndx].sh_offset);  
    unsigned char * section_name = &shrtab[shdr[i].sh_name];
    cout << "Section header " << i << ": " <<  "name=" << section_name << ", ";
    printf("type= %lx, offset= %lx, size= %lx \n",shdr[i].sh_type, shdr[i].sh_offset, shdr[i].sh_size);
  }
  

  /*
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
  */
  
}


