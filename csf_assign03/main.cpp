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
int is_power_of_two(long num){
  
  int num_mod;

  while(num != 1){
    num_mod = num % 2;
    
    if(num_mod != 0){
      return 0;
    }
    
    num /= 2;
  }
  return 1;
}

//return the power of a positive power of two
int get_power(long num){
  int power = 0;
  
  while(num != 1){
    num = num >> 1;
    power++;
  }

  return power;
}

int main(int argc, char* argv[]){

  typedef struct Slot {
    //store the tag of the slot
    unsigned tag;
    
    //indicate whether the memory block is now dirty(?)
    //indicate if the slot is valid
    bool dirty, valid;
    
    
    //store the load timestamp of the slot
    //store the access timestamp of the slot
    unsigned load_stamp, access_stamp;
    
  } Slot;

  //a set is a collection of blocks, order them based on lru or fifo (I think)
  typedef struct Set{

    vector<Slot> blocks;

  } Set;

  typedef struct CacheParams {
    
    //set fields data from command line arguments
    unsigned block_size, num_sets, slots_per_set;

  } CacheParams;

   typedef struct Stats {

    //store caches simulator statistics
    unsigned total_loads, total_stores, load_hits, load_misses, store_hits, store_misses, total_cycles;

  } Stats;

  typedef struct Cache {
    //or are we organizing sets based on lru or fifo? read over info
    vector<Set> sets;

    CacheParams * params;

    //where we store memory trace stats for the cache
    Stats * stats;

    //use this to calculate the smaller timestamps
    unsigned global_timestamp;

  } Cache;


  
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
  long set_num = strtol(argv[1], NULL, 10);
  long block_num = strtol(argv[2], NULL, 10);
  long bytes_per_block = strtol(argv[3], NULL, 10);

 
  //these params cant coexist
  if(strcmp(argv[4], "no-write-allocate") == 0 && strcmp(argv[5], "write-back") == 0){ 
    fprintf(stderr, "Cannot be no-write allocate and write back.\n");
    return 1;
  }

  //make sure integer values are with range
  if(set_num < 0 || block_num < 0){
    fprintf(stderr, "Set and block number must be positive.\n");
    return 1;
  }
  

   if(bytes_per_block < 4){
    fprintf(stderr, "Number of bytes per block must be at least 4.\n");
    return 1;
  }

   //make sure integer values are powers of 2
   if(is_power_of_two(set_num) != 1){
     fprintf(stderr, "Number of sets should be a power of two.\n");
     return 1;
   }
   
   if(is_power_of_two(block_num) != 1){
     fprintf(stderr, "Number of blocks should be a power of two.\n");
     return 1;
   }

   if(is_power_of_two(bytes_per_block) != 1){
     fprintf(stderr, "Number of bytes per block should be a power of two.\n");
     return 1;
   }

   //get the total number of blocks
   int total_blocks = set_num * block_num;

   //get number of offset and index bits
   int num_offset_bits = get_power(bytes_per_block);
   int num_index_bits = get_power(total_blocks);
   int num_tag_bits = 32 - (num_offset_bits + num_index_bits);

   //create a cache
   Cache cache;
   
   //fill all the cache params
   (cache.params)->num_sets = set_num;
   (cache.params)->slots_per_set = block_num;
   (cache.params)->block_size = bytes_per_block;

   //set all stats counters to 0
   (cache.stats) = {0};

   //set the global timestamp to 0
   cache.global_timestamp = 0;

   
   //started writing read from standard in
   char* trace_line = NULL;

   //one line of the memory trace is 13 characters, not counting the irrelvant characters and the end
   size_t len = 13;

   int line_size;

   //index of a new slot created ot represent the one loaded or stored
   unsigned new_index;

   char load = 'l';

   char store = 's';


   
   while((line_size = getline(&trace_line, &len, stdin)) != 0){
     //convert the address part of the line (hex) to an integer, starts at index 4
     long address = strtol(&(trace_line[4]), NULL, 16);

     //next use bit shifts and number of tag, index, and offset bits

     //create a new slot
     Slot new_slot;

     //a slot's tag is all the address bits not including the index and offset bits
     new_slot.tag = address >> (num_offset_bits + num_index_bits);

     //index is a combo of shifting left (tag bits off) and right (offset bits off)
     
     //not relevant yet? make sure there is space in the set
     if((cache.sets)->blocks.size() < (cache.params)->num_sets){
     
     //to get tag bits, shift off

     //to get tag index and offset (store tag in it's struct and index somewhere)

     
     

     //next I think we store the slot in the vector depending on the parameters and if it's load or store
     //need to think about it more

      if (trace_line[0] == load) {



     } else if (trace_line[0] == store) {

     }

   }

  

  return 0; 
}


