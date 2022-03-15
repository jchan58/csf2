#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include <vector>
#include <iterator>

using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::iterator;
using std::fill;
using std::cin;

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



//void increment_accesses(Set * set);

int main(int argc, char* argv[]){

  typedef struct Slot {
    //store the tag of the slot
    unsigned tag;
    unsigned index; 
    
    //indicate whether the memory block is now dirty(?)
    //indicate if the slot is valid
    bool dirty;
    bool valid;
    
    
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

    CacheParams params;

    //where we store memory trace stats for the cache
    Stats stats;

    //use this to calculate the smaller timestamps (dunno why we would tho)
    unsigned global_timestamp;

  } Cache;

  bool set = false; 
  bool fully = false; 
  bool direct = false; 


  
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
   long set_num = strtol(argv[1], nullptr, 10);
   long block_num = strtol(argv[2], nullptr, 10);
   long bytes_per_block = strtol(argv[3], nullptr, 10);

 
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
  
  //create iterators for slots and blocks
  vector<Slot>::iterator slot_it_ptr;
  vector<Set>::iterator set_it_ptr;
  
  //create a cache
  Cache cache;
   
  //fill all the cache params
  (cache.params).num_sets = set_num;
  (cache.params).slots_per_set = block_num;
  (cache.params).block_size = bytes_per_block;

  //set all stats counters to 0
  (cache.stats) = {0, 0, 0, 0, 0, 0, 0};

  //set the global timestamp to 0
  cache.global_timestamp = 0;

   
   
  //initialize the empty cache
   
  //set the correct number of empty sets
  (cache.sets).resize((cache.params).num_sets);
  unsigned i = 0;
  //i indicates the set number which is the index 
  //set the correct number of blocks per set
  for(set_it_ptr = (cache.sets).begin(); set_it_ptr < (cache.sets).end(); set_it_ptr++){
    //set the size of each set
    (*set_it_ptr).blocks.resize((cache.params).slots_per_set); 
    for(slot_it_ptr = (*set_it_ptr).blocks.begin(); slot_it_ptr < (*set_it_ptr).blocks.end(); slot_it_ptr++){
      //fill the blocks as empty
      Slot empty = {0, i, false, true, 0, 0};
      *slot_it_ptr = empty;
    }
    i++;
  }

    
    //check to see if what type of mapping this
    if(set_num == 1 && block_num > 1) {
      fully = true; 
    } else if(set_num > 1 && block_num > 1) {
      set = true; 
    } else {
      direct = true; 
    }
     
    //started writing read from standard in (old)
    char* trace_line = NULL;

    //one line of the memory trace is 13 characters, not counting the irrelvant characters and the end
    size_t len = 13;

    //size_t line_size;

    //index of a new slot created ot represent the one loaded or stored
    //above is old read stuff

    //string trace_line;
    //getline(std::cin, trace_line);

    unsigned new_index;

    char load = 'l';

    char store = 's';

    bool store_hit;

    bool load_hit;

    unsigned current_tag; 

    unsigned current_index; 

  
    //if it is a hit, we will need to access the the slot found
    Slot * in_cache;
    //to check dirty and valid, we need to check valid to see if it is a hit
    //because tag will match always for direct for example
  
    int numLoaded = 0; 
    while(getline(&trace_line, &len, stdin) != -1){
    load_hit = false; 
    store_hit = false; 

     //convert the address part of the line (hex) to an integer, starts at index 4
     long address = strtol(&(trace_line[4]), NULL, 16);
 
    //determine the specific mapping and create tags and indexes according to it; 
     current_tag = address >> (num_offset_bits + num_index_bits);
     current_index = address << num_tag_bits;
     current_index = current_index >> (num_tag_bits + num_offset_bits); 

    if(fully) {
      current_tag = current_tag + current_index;
      current_index = 0; 
    }

     for(set_it_ptr = (cache.sets).begin(); set_it_ptr < (cache.sets).end(); set_it_ptr++){
       for(slot_it_ptr = (*set_it_ptr).blocks.begin(); slot_it_ptr < (*set_it_ptr).blocks.end(); slot_it_ptr++){
        if((*slot_it_ptr).tag == current_tag && (*slot_it_ptr).index == current_index) {
           in_cache = &(*slot_it_ptr);
           //increment the access stamp because first time accessing it 
           (*slot_it_ptr).access_stamp = (*slot_it_ptr).access_stamp++; 
           if(trace_line[0] == load) { //if this is a load and there is a hit  
             load_hit = true; 
           } else {
             store_hit = true; 
           }
           //this happens if the index is equal but the slot is not 
           
        } else if((*slot_it_ptr).tag != current_tag && (*slot_it_ptr).index == current_index && (*slot_it_ptr).valid == true){
           Slot new_slot = (*slot_it_ptr); 
          //replace the slot with incoming tag
          if(trace_line[0] == load) {
          (*slot_it_ptr).tag = current_tag; 
          (*slot_it_ptr).index = current_index;
          (*slot_it_ptr).valid = false; 
          (*slot_it_ptr).access_stamp = 1; 
          numLoaded++; 
          (*slot_it_ptr).load_stamp = numLoaded;
          }
        } 
      }
     }

  
      //see if this is a load in input address 
      if(trace_line[0] == load) {
        if (!load_hit) {

          //next use bit shifts and number of tag, index, and offset bits
         //a slot's tag is all the address bits not including the index and offset bits
         //should we add block is dirty according to the specific parameters? 
         //we do not want to change the cache 
         //depending on which mapping this is 
         //push back the slot according to the specific index of the block

        //calculate the miss penalty 
          (cache.stats).total_loads++;
          (cache.stats).load_misses++;
          (cache.stats).total_cycles += 100 * ((cache.params).block_size / 4);
        } else if(load_hit) {
          //have to update the access timestamp 
          //this is a hit depending on load or store 
          (cache.stats).total_loads++;
	        (cache.stats).load_hits++;
	        (cache.stats).total_cycles++;
        }
      } else {
        //if there is not a store_hit calculate data for that 
         if(!store_hit) {
           //update access stamp for that specific block 
          //if miss, still have to put block in cache and memory (same cycle update)
	        (cache.stats).total_stores++;
          (cache.stats).store_misses++;
          if(strcmp(argv[4], "no-write-allocate") == 0) {
            //no-write-allocate: store miss, don't put in cache; do put in memory ofc
            (cache.stats).total_cycles += 100 * ((cache.params).block_size / 4);
          } else {
            //write-allocate: store miss, put in cache; change memory ofc
            (cache.stats).total_cycles += 1 +(100 * ((cache.params).block_size / 4));
          }
        } else if (store_hit){
          (cache.stats).total_stores++;
          (cache.stats).store_hits++;
          if(strcmp(argv[5], "write-through") == 0) {
            //write-through: store writes to cache and to memory
            (cache.stats).total_cycles += 1 + 100 * ((cache.params).block_size / 4);
          } else {
            //write-back: write only to cache so block is dirty
            (cache.stats).total_cycles += 100 * ((cache.params).block_size / 4);
            //if dirty is true, it must be written to memory first (add later)
            (*in_cache).dirty = true;
          }
        }
      }
    }

      
     
     //find the matching set with the index of the slot you want to insert
     //new_index should be withing range

     //only do this if a set is multiblock?
     //is a pointer so we can change the actual set and slot
     //Set * match = &((cache.sets).at(new_index));
     //if match tag is block...

      //we are doing one block per set for now (direct mapping)
     //Slot * in_cache = &((*match).blocks.at(0));


   //when the while loop finishes, print the summary in the indicated format
   cout << "Total loads: " << (cache.stats).total_loads << "\n";
   cout << "Total stores: " << (cache.stats).total_stores << "\n";
   cout << "Load hits: " << (cache.stats).load_hits << "\n";
   cout << "Load misses: " << (cache.stats).load_misses << "\n";
   cout << "Store hits: " << (cache.stats).store_hits << "\n";
   cout << "Store misses: " << (cache.stats).store_misses << "\n";
   cout << "Total cycles: " << (cache.stats).total_cycles << "\n";

  return 0; 
}
