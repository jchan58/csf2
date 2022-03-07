#include <iostream>
#include <string>
#include <string.h>


//function to check if a number is a power of 2
int isPowerOfTwo(long num){

  if(num == 1){
    return 0;
  }
  
  int numMod;

  while(num != 1){
    numMod = num % 2;
    
    if(numMod != 0){
      return 0;
    }
    
    num /= 2;
  }
  return 1;
}


int main(int argc, char* argv[]){


  //argv[1] is number of sets in cache, pos power of 2
  //argv[2] is number of blocks in set, pos power of 2
  //argv[3] number of bytes in each block, min 4
  //argv[4] is write-allocate or no-write -allocate
  //argv[5] is write-through or write-back
  //argv[6] is lru or fifo evictions

  
  if(argc < 7){
     fprintf(stderr, "Must enter all six arguments.\n");
     return 1;
  }
  
  long setNum = strtol(argv[1], NULL, 10);
  long blockNum = strtol(argv[2], NULL, 10);
  long bytesPerBlock = strtol(argv[3], NULL, 10);

 
  
  if(strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0){ 
    fprintf(stderr, "Cannot be no-write allocate and write back.\n");
    return 1;
  }

  
  if(setNum < 0 || blockNum < 0){
    fprintf(stderr, "Set and block number must be positive.\n");
    return 1;
  }
  

   if(bytesPerBlock < 4){
    fprintf(stderr, "Number of bytes per block must be at least 4.\n");
    return 1;
  }


   if(isPowerOfTwo(setNum) != 1){
     fprintf(stderr, "Number of sets should be a power of two.\n");
     return 1;
   }
   
   if(isPowerOfTwo(blockNum) != 1){
     fprintf(stderr, "Number of blocks should be a power of two.\n");
     return 1;
   }

   if(isPowerOfTwo(bytesPerBlock) != 1){
     fprintf(stderr, "Number of bytes per block should be a power of two.\n");
     return 1;
   }

    
   /*
   char* trace_line = NULL;

   //one line of the memory trace is 13 characters, not counting the irrelvant characters and the end
   int len = 13; 

   int lineSize;

   while((lineSize = readline(&trace_line, &len, stdin)) != 0){
     //do stuff

     }*/
   
  return 0; 
}

