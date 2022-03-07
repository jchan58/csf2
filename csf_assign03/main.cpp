#include <iostream>
#include <string>
#include <string.h>
#include <fstream>


int main(int argc, char* argv[]){

  //argv[0] is number of sets in cache, pos power of 2
  //argv[1] is number of blocks in set, pos power of 2
  //argv[2] number of bytes in each block, min 4
  //argv[3] is write-allocate or no-write -allocate
  //argv[4] is write-through or write-back
  //argv[5] is lru or fifo evictions

  int setNum = strtol(argv[0], NULL, 10);
  int blockNum = strtol(argv[1], NULL, 10);
  int bytesPerBlock = strtol(argv[2], NULL, 10);
  

  if(strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0){ 
    fprintf(stderr, "Cannot be no-write allocate and write back.");
    return 1;
  }

  if(setNum < 0 || blockNum < 0){
    fprintf(stderr, "Set and block number must be positive.");
    return 1;
  }

   if(bytesPerBlock < 4){
    fprintf(stderr, "Number of bytes per block must be at least 4.");
    return 1;
  }


  int setNumCopy = setNum;
  int setNumMod;
 
  int blockNumCopy = blockNum;
  int blockNumMod;
  
  while(setNumCopy != 1){
    setNumMod = setNumCopy % 2;

    if(setNumMod != 0){
      fprintf(stderr, "Set number should be a power of two.");
      return 1;
    }
  }

   while(blockNumCopy != 1){
    blockNumMod = blockNumCopy % 2;

    if(blockNumMod != 0){
      fprintf(stderr, "Block number should be a power of two.");
      return 1;
    }
  }
   
   //opening a file
   std::fstream file;
   file.open(argv[7], std::fstream::in | std::fstream::out);
   if(!file.is_open()){
    fprintf(stderr, "Error opening file.");
    return 1;
   }
    

  return 0; 
}
