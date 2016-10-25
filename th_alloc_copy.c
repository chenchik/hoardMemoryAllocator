/*imple Hoard-style malloc/free implementation.
 * Not suitable for use for large allocatoins, or 
 * in multi-threaded programs.
 * 
 * to use: 
 * $ export LD_PRELOAD=/path/to/th_alloc.so <your command>
 */

/* Hard-code some system parameters */

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
 
#define assert(cond) if (!(cond)) __asm__ __volatile__ ("int $3")

/*DC METHODS*/
int intpow(int base, int exp);

/* Object: One return from malloc/input to free. */
//just some object named object with a next and data
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
  uint8_t level;
};
  
/* Superblock: a chunk of contiguous virtual memory.
 * Subdivide into allocations of same power-of-two size. */
struct __attribute__((packed)) superblock {
  struct superblock_bookkeeping bkeep;
  void *raw;  // Actual data here
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
  /* Your code here.
   * Convert the size to the correct power of two. 
   * Recall that the 0th entry in levels is really 2^5, 
   * the second level represents 2^6, etc.
   */
  //if(size < 0){
  //  fprintf(stderr, "ERROR: can't do negative memory allocation\n");
  //  exit(EXIT_FAILURE);
  //}  

  int exp = 5;
  int sizeConvert = intpow(2, exp);
  while(size > sizeConvert){
    exp++;
    sizeConvert = intpow(2, exp);
  }
  //fprintf(stderr, "sizeconert is: %d\n",sizeConvert);
    
  if(sizeConvert > MAX_ALLOC){
    fprintf(stderr, "ERROR: too large of a memory allocation\n");
    exit(EXIT_FAILURE);
  }
  //fprintf(stderr, "about to return %d", exp-5);
  return exp-5;
}

static inline
struct superblock_bookkeeping * alloc_super (int power) {

  void *page;
  struct superblock* sb;
  int free_objects = 0, bytes_per_object = 0;
  char *cursor;
  // Your code here  
  // Allocate a page of anonymous memory
  // WARNING: DO NOT use brk---use mmap, lest you face untold suffering
  //fprintf(stderr, "about to allocate a page\n");
  page = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  //fprintf(stderr, "%d\n", errno);
  if (!page){
    fprintf(stderr,"%d\n", errno);
    exit(EXIT_FAILURE);
  }
  //fr level 0, 128 objects per 4 k bytes 4k/128
  //fprintf(stderr, "just alloaced a page \n");
  sb = (struct superblock*) page;
  // Put this one the list.
  //fprintf(stderr, "created sb power is:%d, levels[pwer].next is: %s\n", power, levels[power].next);
  sb->bkeep.next = levels[power].next;
  //fprintf(stderr, "created sb1\n");
  levels[power].next = &sb->bkeep;
  //fprintf(stderr, "created sb2\n");
  levels[power].whole_superblocks++;
  //fprintf(stderr, "created sb3\n");
  sb->bkeep.level = power;
  //fprintf(stderr, "created sb4\n");
  sb->bkeep.free_list = NULL;
  
  //int freeCounter = 1;
  //struct superblock_bookkeeping * tempNext = levels[power].next;
  //while(tempNext != NULL){
    //maybe since max amount of free objects is 128, subtract 128-freeCount
  //  tempNext = tempNext.next;
  //  freeCounter++;
  //}
  //int freeAmount = (pow(2, 7)/pow(2, power)) - freeCounter
  //fprintf(stderr, "doing free amount stuff\n");
  //int freeAmount = (4096/pow(2, power+5))-1;
  sb->bkeep.free_count = (4096/ intpow(2, power+5))-1;
  levels[power].free_objects += ((4096/ intpow(2, power+5))-1);
  free_objects = ((4096/ intpow(2, power+5))-1);
  bytes_per_object = intpow(2, power+5);
  //fprintf(stderr, "done with free amount stuff\n");
  // Your code here: Calculate and fill the number of free objects in this superblock
  //  Be sure to add this many objects to levels[power]->free_objects, reserving
  //  the first one for the bookkeeping.

  // The following loop populates the free list with some atrocious
  // pointer math.  You should not need to change this, provided that you
  // correctly calculate free_objects.
  
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
  //fprintf(stderr, "power is %d\n", power);  
  // Check that the allocation isn't too big
  if (size > MAX_ALLOC) {
    errno = -ENOMEM;
    return NULL;
  }
  
  // Delete the following two lines
  //errno = -ENOMEM;
  //return rv;

  
  pool = &levels[power];

  if (!pool->free_objects) {
    bkeep = alloc_super(power);
  } else
    bkeep = pool->next;

  while (bkeep != NULL) {
    if (bkeep->free_count) {
      struct object *next = bkeep->free_list;
      /* Remove an object from the free list. */
      // Your code here
      //
      //fprintf(stderr, "changing da free list\n");
      //struct object *nextnext = next->next;
      //bkeep->free_list.next = obj.next;
      //
      //tmp->next = sb->bkeep.free_list;
      //sb->bkeep.free_list = tmp;
      
      //bkeep->free_list = next->next;
      rv = next;
      bkeep->free_list = next->next;
      bkeep->free_count--;
      levels[power].free_objects--;
      //fprintf(stderr, "done changing da free list\n");
      
      // NB: If you take the first object out of a whole
      //     superblock, decrement levels[power]->whole_superblocks
      break;
    }
  }

  // assert that rv doesn't end up being NULL at this point
  assert(rv != NULL);

  /* Exercise 3: Poison a newly allocated object to detect init errors.
   * Hint: use ALLOC_POISON
   */
  return rv;
}

static inline
struct superblock_bookkeeping * obj2bkeep (void *ptr) {
  uint64_t addr = (uint64_t) ptr;
  addr &= SUPER_BLOCK_MASK;
  return (struct superblock_bookkeeping *) addr;
}

void free(void *ptr) {
  struct superblock_bookkeeping *bkeep = obj2bkeep(ptr);

  // Your code here.
  //   Be sure to put this back on the free list, and update the
  //   free count.  If you add the final object back to a superblock,
  //   making all objects free, increment whole_superblocks.

  while (levels[bkeep->level].whole_superblocks > RESERVE_SUPERBLOCK_THRESHOLD) {
    // Exercise 4: Your code here
    // Remove a whole superblock from the level
    // Return that superblock to the OS, using mmunmap

    break; // hack to keep this loop from hanging; remove in ex 4
  }
  
  /* Exercise 3: Poison a newly freed object to detect use-after-free errors.
   * Hint: use FREE_POISON
   */
}

// Do NOT touch this - this will catch any attempt to load this into a multi-threaded app
int pthread_create(void __attribute__((unused)) *x, ...) {
  exit(-ENOSYS);
}

int intpow(int base, int exp)
{
  int result = 1;
  while (exp){
    if (exp & 1)
      result *= base;
      exp >>= 1;
      base *= base;
    }
  return result;
}
