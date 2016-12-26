/*imple Hoard-style malloc/free implementation.
 * Not suitable for use for large allocatoins, or 
 * in multi-threaded programs.
 * 
 * to use: 
 * $ export LD_PRELOAD=/path/to/th_alloc.so <your command>
 */

/*Created By Danila Chenchik and Greer Glenn*/
/*We agree to the UNC honor pledge*/

#define SUPER_BLOCK_SIZE 4096
#define SUPER_BLOCK_MASK (~(SUPER_BLOCK_SIZE-1))
#define MIN_ALLOC 32 /* Smallest real allocation.  Round smaller mallocs up */
#define MAX_ALLOC 2048 /* Fail if anything bigger is attempted.  
		        * Challenge: handle big allocations */
#define RESERVE_SUPERBLOCK_THRESHOLD 2
#define EXP_OFFSET 5

#define FREE_POISON 0xab
#define ALLOC_POISON 0xcd

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <math.h>
#include <sys/mman.h>
#include <string.h>

#define assert(cond) if (!(cond)) __asm__ __volatile__ ("int $3")

/*DC METHODS*/
int intpow(int base, int power);
/* Object: One return from malloc/input to free. */
/*holds actual data for each variable you malloc*/
struct __attribute__((packed)) object {
  union {
    struct object *next; // For free list (when not in use)
    char * raw; // Actual data
  };
};

/* Super block bookeeping; one per superblock.  "steal" the first
 * object to store this structure
 */

struct __attribute__((packed)) superblock_bookkeeping {
  struct superblock_bookkeeping * next; // next super block
  struct object *free_list;
  // Free count in this superblock
  uint8_t free_count; // Max objects per superblock is 128-1, so a byte is sufficient
  /*free count is (4096/ intpow(2, level+5))-1;*/
  uint8_t level;//level on the big superpool array
};
  
/* Superblock: a chunk of contiguous virtual memory.
 * Subdivide into allocations of same power-of-two size. */
struct __attribute__((packed)) superblock {
  struct superblock_bookkeeping bkeep;
  void *raw;  // this isnt used for anything
};


/* The structure for one pool of superblocks.  
 * One of these per power-of-two */
struct superblock_pool {
  struct superblock_bookkeeping *next;
  uint64_t free_objects; // Total number of free objects across all superblocks
  uint64_t whole_superblocks; // Superblocks with all entries free
};

// 10^5 -- 10^11 == 7 levels
#define LEVELS 7
//consists of superblock_pools
//(next superblok pool, free objects, whole_superblocks)
static struct superblock_pool levels[LEVELS] = {{NULL, 0, 0},
						{NULL, 0, 0},
						{NULL, 0, 0},
						{NULL, 0, 0},
						{NULL, 0, 0},
						{NULL, 0, 0},
						{NULL, 0, 0}};

static inline int size2level (ssize_t size) {
  int exp = 5;
  int sizeConvert = intpow(2, exp);
  while(size > sizeConvert){
    exp++;
    sizeConvert = intpow(2, exp);
  }  
  return exp-5;
}

static inline
struct superblock_bookkeeping * alloc_super (int power) {

  void *page;
  struct superblock* sb;
  int free_objects = 0, bytes_per_object = 0;
  char *cursor;
  page = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (!page){
    fprintf(stderr,"%d\n", errno);
    exit(EXIT_FAILURE);
  }
  //fr level 0, 128 objects per 4 k bytes 4k/128
  sb = (struct superblock*) page;
  // Put this one the list.
  sb->bkeep.next = levels[power].next;
  levels[power].next = &sb->bkeep;
  levels[power].whole_superblocks++;
  sb->bkeep.level = power;
  sb->bkeep.free_list = NULL;
  //set appropriate values 
  sb->bkeep.free_count = (4096/ intpow(2, power+5))-1;
  levels[power].free_objects += ((4096/ intpow(2, power+5))-1);
  free_objects = ((4096/ intpow(2, power+5))-1);
  bytes_per_object = intpow(2, power+5);
  
  cursor = (char *) sb;
  // skip the first object
  for (cursor += bytes_per_object; free_objects--; cursor += bytes_per_object) {
    // Place the object on the free list
    struct object* tmp = (struct object *) cursor;
    tmp->next = sb->bkeep.free_list;
    sb->bkeep.free_list = tmp;
  }
  return &sb->bkeep;
}

void *malloc(size_t size) {
  struct superblock_pool *pool;
  struct superblock_bookkeeping *bkeep;
  void *rv = NULL;
  int power = size2level(size);
  // Check that the allocation isn't too big or too small
  if (size > MAX_ALLOC) {
    errno = -ENOMEM;
    return NULL;
  }else if(size <= 0){
    return NULL;
  }
  
  pool = &levels[power];

  if (!pool->free_objects) {
    bkeep = alloc_super(power); //add a new superblock
  } else
    bkeep = pool->next;

  while (bkeep != NULL) {
    if (bkeep->free_count) {
      struct object *next = bkeep->free_list;
      /* Remove an object from the free list. */
      rv = next;
      bkeep->free_list = next->next;
      bkeep->free_count--;
      levels[power].free_objects--;
      //we know that we just freed the first object out of a whole superblock
      if(bkeep->free_count == (4096/ intpow(2, power+5))-2){
        levels[power].whole_superblocks--;
      }
      break;
    }
  }

  // assert that rv doesn't end up being NULL at this point
  assert(rv != NULL);
  //poisoning
  memset(rv, ALLOC_POISON, intpow(2, power+5));

  return rv;
}



static inline
struct superblock_bookkeeping * obj2bkeep (void *ptr) {
  uint64_t addr = (uint64_t) ptr;
  addr &= SUPER_BLOCK_MASK;
  return (struct superblock_bookkeeping *) addr;
}

void free(void *ptr) {
  //getting bkeeping object associated with the object
  struct superblock_bookkeeping *bkeep = obj2bkeep(ptr);
  struct object * obj = (struct object * ) ptr;
  //poisoning ptr+8 to make room for the next pointer  
  memset(ptr+8, FREE_POISON, intpow(2, (bkeep->level)+5)-8);
  obj->next = bkeep->free_list;
  bkeep->free_list = obj; 
  bkeep->free_count++;
  int power = bkeep->level;
  //if the whole superblock is totally empty, increment the amount of free superblocks  
  if(bkeep->free_count == (4096/ intpow(2, power+5))-1){
    levels[bkeep->level].whole_superblocks++;
  }

  int empty_count = 0;
  struct superblock_bookkeeping *nextBkeep = levels[bkeep->level].next;
  struct superblock_bookkeeping *prevBkeep = NULL;
  //if we have >2 whole superblocks, find the farthest free superblock 
  //from the from of the list and get rid of itusing munmap
  while (levels[power].whole_superblocks > RESERVE_SUPERBLOCK_THRESHOLD) {
    if(nextBkeep->free_count == (4096/intpow(2,power + 5)) - 1)
        empty_count++;

    if(empty_count > RESERVE_SUPERBLOCK_THRESHOLD){
        //we found the third superblock from the front of the list
        prevBkeep->next = nextBkeep->next;
        int res = munmap(nextBkeep,4096);
        levels[power].whole_superblocks--;
        if(res == 0){
          break;
        }else{
          printf("ERROR: %s\n", strerror(errno));
          exit(EXIT_FAILURE);
        }
    }
     prevBkeep = nextBkeep;
     nextBkeep = nextBkeep->next;      
  }
}

// Do NOT touch this - this will catch any attempt to load this into a multi-threaded app
int pthread_create(void __attribute__((unused)) *x, ...) {
  exit(-ENOSYS);
}

void *calloc(size_t n, size_t size){
  void *p = malloc(n*size); //allocate n blocks each of the specified size
  assert(p != NULL);
  return memset(p, 0, n*size); //now set everything to 0
}

void *realloc(void *ptr, size_t size) {
  if (!ptr) {
    // just return malloc in this case
    return malloc(size);
  }

  struct superblock_bookkeeping *bkeep = obj2bkeep(ptr);
  if(intpow(2, 5+bkeep->level) >= size){
    // We already have enough space
    return ptr;
  }
 
  // Now we need to actually reallocate. Malloc new space and free old space.
  // Then copy old data to new space.
  void *new_ptr;
  new_ptr = malloc(size);
  if (!new_ptr) {
    return NULL;
  }
  memcpy(new_ptr, ptr, intpow(2,5 + bkeep->level));
  free(ptr);
  return new_ptr;
}

int intpow(int base, int power)
{
  int returnNum = 1;
  while (power){
    if (power & 1)
      returnNum *= base;
      power >>= 1;
      base *= base;
    }
  return returnNum;
}
