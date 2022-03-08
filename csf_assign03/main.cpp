#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include <vector>

using std::string;
using std::cout;
using std::endl;
using std::vector;

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

   typedef struct Slot {
    //store the tag of the slot
    char* tag;

    //store the index of the slot
    char* index;

    //store the offset of the slot
    char* offset;

    //store the load timestamp of the slot
    unsigned load_stamp;

    //store the access timestamp of the slot
    unsigned access_stamp;


  } Slot;



  //order vector based off of load stamp or access stamp, depending on eviction type!

  //argv[1] is number of sets in cache, pos power of 2
  //argv[2] is number of blocks in set, pos power of 2
  //argv[3] number of bytes in each block, min 4
  //argv[4] is write-allocate or no-write -allocate
  //argv[5] is write-through or write-back
  //argv[6] is lru or fifo evictions
  //think about what else could go wrong with arguments (remove above later) 

  if(argc < 7){
     fprintf(stderr, "Must enter all six arguments.\n");
     return 1;
  }
  
   //check if argv[4] valid"
  if((strcmp(argv[4], "write-allocate") != 0) && (strcmp(argv[4], "no-write-allocate") != 0)){
     fprintf(stderr, "Cache miss paramters must be write-allocate or no-write allocate");
     return 1;
   }
 
 
   //check if argv[5] is "write-through" or "write-back"
   if((strcmp(argv[5], "write-through") != 0) && (strcmp(argv[5], "write-back") != 0)){
     fprintf(stderr, "Write type must be write-back or write-through");
     return 1;
   }

   //check if argv[6] is "lru" or "fifo"
   if((strcmp(argv[6], "lru") != 0) && (strcmp(argv[6], "fifo") != 0)){
     fprintf(stderr, "Evictions do not match lru or fifo.\n");
     return 1;
   }
  



  //convert the args representing integers into integers
  long setNum = strtol(argv[1], NULL, 10);
  long blockNum = strtol(argv[2], NULL, 10);
  long bytesPerBlock = strtol(argv[3], NULL, 10);

 
  //these params cant coexist
  if(strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0){ 
    fprintf(stderr, "Cannot be no-write allocate and write back.\n");
    return 1;
  }

  //make sure integer values are with range
  if(setNum < 0 || blockNum < 0){
    fprintf(stderr, "Set and block number must be positive.\n");
    return 1;
  }
  

   if(bytesPerBlock < 4){
    fprintf(stderr, "Number of bytes per block must be at least 4.\n");
    return 1;
  }

   //make sure integer values are powers of 2
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

   
    
   
   //started writing read from standard in
   char* trace_line = NULL;

   //one line of the memory trace is 13 characters, not counting the irrelvant characters and the end
   size_t len = 13;

   int lineSize;

   char* tag = NULL;

   char* index = NULL;

   char* offset = NULL;

   while((lineSize = getline(&trace_line, &len, stdin)) != 0){
     //the tag bits are represented by characters 4-6 of the line
     strncpy(tag, &trace_line[4], 3);

     //the index bits are represented by characters 7-9 of the line
     strncpy(index, &trace_line[7], 3);

     //the offset bits are represented by characters 10-11 of the line
     strncpy(offset, &trace_line[10], 2);

     //create a slot with these properties ( I think the timestamps both start at 0?)
     Slot slot = {tag, index, offset, 0, 0};

     //next I think we store the slot in the vector depending on the parameters and if it's load or store
     //need to think about it more
   }

  

  return 0; 
}


