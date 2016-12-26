Greer Glenn (greer112) and Danila Chenchik (chenchik) LAB 2 SUBMISSION
we agree to the UNC Honor Code

OUR APPROACH:

we wanted to keep this simple and straight forward. Following the HOARD model, we preferred to make free blocks of memory easily accesible by placing them at the beginning of the free list. 

When two whole superblocks were free, we traversed the list and removed the last instance of a free whole superblock, so that we could keep as many free whole blocks towards the front of the list as possible.

For poisoning, we poisoned all of the memory that was malloced. With the free poisoning, we wanted to maintain the free_list pointer, so we offset the poisoning start by 8 bytes, as suggested by piazza.

Files included are test.c, test, Makefile , readme.txt, th_alloc.c, th_alloc.sh



EXTRA CREDIT:

We attempted calloc and realloc. Approach for calloc was straightforward and relied on our malloc implementation and memset.
Realloc was designed to only reallocate new memory if the parameter size was larger than what the pointer currently pointed to. This way we wouldnt have to free the memory and call malloc again if the requested size would fall into what was already being utilized. If a null pointer was passed to realloc, then we just call malloc with the size and ignore the parameter pointer. 
