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
    shdr = (Elf64_Shdr *) (char_data + elf->e_shoff);
    //used to access each specific section accessed by indexing 
    shrtab = (unsigned char *)(char_data + shdr[elf->e_shstrndx].sh_offset);  
    unsigned char * section_name = &shrtab[shdr[i].sh_name];
    cout << "Section header " << i << ": " <<  "name=" << section_name << ", ";
    //converted sh.type to a long val
    printf("type= %lx, offset= %lx, size= %lx \n", (long unsigned int) shdr[i].sh_type, shdr[i].sh_offset, shdr[i].sh_size);

    
    
    //0x2 is the value that means symbol table
    if(shdr[i].sh_type == 0x2) {
      //print the information for the symbol table 

      //sh offset gives you the location the symbol table
      Elf64_Sym * symtab = (Elf64_Sym * ) &elf[shdr[i].sh_offset];

      //number of symbols in a section is the size of the section/size of each entry in that section
      int numSymbols = shdr[i].sh_size/shdr[i].sh_entsize;

      //how to properly get symbol table data from symbol table???
      for(int i = 0; i < numSymbols; i++) {
        cout << "Symbol " << i << ": name = " << symtab[i].st_name << ", ";
        printf("size= %lx, info= %lx, other= %lx \n", symtab[i].st_size, symtab[i].st_info, symtab[i].st_other);
      }
    }
    
  }
  
  
}


