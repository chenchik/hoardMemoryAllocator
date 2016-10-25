# hoardMemoryAllocator
Hoard memory allocator. Implements malloc() and free().

In order to run on a linux machine type:

<code>bash</code>
<code>LD_PRELOAD=./th_alloc.c ./yourProgram.c</code>

Overrides default malloc() and free() functionality.
