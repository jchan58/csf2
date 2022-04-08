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

//ours
#include <iostream>

using std::cerr;
using std::string;
using std::cout; 

int main(int argc, char **argv) {
  size_t file_size;
  string file = argv[1];
  int length = file.length(); 
  bool executable = false; 
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

  if(file[length - 1] == 'o' && file[length - 2] == '.') {
    executable = false; 
  } else {
    executable = true; 
  }
  
  if(!executable) {
    cout << "Object file type: " << "ET_REL" << "\n";
  } else {
    cout << "Object file type: " << "ET_EXEC" << "\n";
  }
  cout << "Instruction Set: " << "\n";
  cout << "Endianness: " << "\n";














  
}
