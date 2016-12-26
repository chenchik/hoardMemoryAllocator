# hoardMemoryAllocator
Simple Hoard memory allocator. Implements malloc(), free(), calloc(), realloc(), and poisoning for malloc() and free().

In order to run on a linux machine follow these commands:

If not using bash, type:

<code>bash</code><br>

Then, to compile the program using the Makefile, type:

<code>make</code><br>

To run the program type:

<code>LD_PRELOAD=./th_alloc.so ./your_command_or_binary</code>

Overrides default malloc() and free() functionality.

Following the HOARD model, we preferred to make free blocks of memory easily accesible by placing them at the beginning of the free list. 

When two whole superblocks were free, we traversed the list and removed the last instance of a free whole superblock, so that we could keep as many free whole blocks towards the front of the list as possible.

For poisoning, we poisoned all of the memory that was malloced. With the free poisoning, we wanted to maintain the free_list pointer, so we offset the poisoning start by 8 bytes.

Approach for calloc was straightforward and relied on our malloc implementation and memset.

Realloc was designed to only reallocate new memory if the parameter size was larger than what the pointer currently pointed to. This way we wouldnt have to free the memory and call malloc again if the requested size would fall into what was already being utilized. If a null pointer was passed to realloc, then we just call malloc with the size and ignore the parameter pointer. 
